/*
 *	stream.cpp
 *	Binary file stream helper
 */

#include <cstdlib>
#include <cstring>
#include <Util/Stream.hpp>
#include <zlib.h>

namespace Util
{
	Stream::Stream(const std::string& filename, StreamMode mode)
		: fileStream(),
		  buffer(),
		  streamMode(mode),
		  position(0)
	{
		if (streamMode == ModeMemory)
			return;

		std::fstream::openmode openMode = std::fstream::binary;
		openMode |= (streamMode == ModeRead)
						? std::fstream::in
						: std::fstream::out;

		fileStream.open(filename.c_str(), openMode);
		if (!fileStream)
		{
			std::stringstream ss;
			ss << "Cannot open file \""
			   << filename
			   << "\" for streaming.";
			throw StreamException(ss.str());
		}
	}

	Stream::Stream()
		: fileStream(),
		  buffer(),
		  streamMode(ModeMemory),
		  position(0)
	{

	}

	Stream::~Stream()
	{
		if (streamMode != ModeMemory)
			fileStream.close();
	}

	void Stream::ExpandMemoryStream(size_t length)
	{
		if (streamMode != ModeMemory)
			return;

		if (position + length < buffer.size())
			return;

		if (position + length > buffer.size() + length)
			buffer.resize(position + length, 0);
		else
			buffer.resize(buffer.size() + length, 0);
	}

	unsigned char* Stream::DeflateStream(const StreamBuffer& sourceBuffer, size_t *deflatedLength)
	{
		uLongf destSize = static_cast<uLongf>(sourceBuffer.size() * 1.001f + 12);
		Bytef* destBuffer = new Bytef[destSize];

		// Deflate
		int result = compress2(destBuffer, &destSize, sourceBuffer.data(), sourceBuffer.size(), Z_BEST_SPEED);
		if (result != Z_OK)
		{
			*deflatedLength = 0;
			return nullptr;
		}

		*deflatedLength = static_cast<size_t>(destSize);

		return destBuffer;
	}

	unsigned char* Stream::InflateStream(const StreamBuffer& sourceBuffer, size_t *inflatedLength)
	{
		const unsigned int CHUNK_SIZE = 2048;

		unsigned int destBufferSize = sourceBuffer.size();
		unsigned char* destBuffer = new unsigned char[destBufferSize];
		z_stream stream;
		int result;

		std::memset(&stream, 0, sizeof(z_stream));

		stream.next_in			= const_cast<Bytef*>(sourceBuffer.data());
		stream.avail_in			= sourceBuffer.size();
		stream.next_out			= destBuffer;
		stream.avail_out		= destBufferSize;

		// Begin inflating
		inflateInit(&stream);

		// Inflate blocks
		while(stream.avail_in && !(result = inflate(&stream, 0)))
		{
			int offset = stream.next_out - destBuffer;

			destBufferSize += CHUNK_SIZE;
			stream.avail_out += CHUNK_SIZE;

			destBuffer = static_cast<unsigned char*>(std::realloc(destBuffer, destBufferSize));
			if (destBuffer == nullptr)
				return nullptr;

			stream.next_out = destBuffer + offset;
		}

		if (result != Z_STREAM_END)
		{
			delete[] destBuffer;
			return nullptr;
		}

		// Trim excess memory allocated & end inflation
		destBufferSize = stream.total_out;
		destBuffer = static_cast<unsigned char*>(std::realloc(destBuffer, destBufferSize));
		inflateEnd(&stream);

		*inflatedLength = destBufferSize;

		return destBuffer;
	}

	void Stream::Rewind()
	{
		if (streamMode == ModeMemory)
			position = 0;
		else
		{
			fileStream.seekg(std::fstream::beg);
			fileStream.seekp(std::fstream::beg);
		}
	}

	void Stream::Seek(int offset, int origin)
	{
		switch(streamMode)
		{
			case ModeMemory:
			{
				switch(origin)
				{
					case SEEK_SET:
						position = origin;
						break;

					case SEEK_CUR:
						position += origin;
						break;

					case SEEK_END:
						position = GetLength() + origin;
						break;
				}

				// Clamp position to range
				if (position >= GetLength())
					position = GetLength();

				break;
			}

			case ModeRead:
				fileStream.seekg(offset, origin);
				break;

			case ModeWrite:
				fileStream.seekp(offset, origin);
				break;
		}
	}

	size_t Stream::GetPosition()
	{
		switch(streamMode)
		{
			case ModeMemory:
				return position;

			case ModeRead:
				return static_cast<size_t>(fileStream.tellg());

			case ModeWrite:
				return static_cast<size_t>(fileStream.tellp());
		}

		return 0;
	}

	size_t Stream::GetLength()
	{
		std::fstream::pos_type length = 0;

		if (streamMode == ModeMemory)
			return buffer.size();

		if (streamMode == ModeRead)
		{
			std::fstream::pos_type position = fileStream.tellg();
			fileStream.seekg(0, std::fstream::end);
			length = fileStream.tellg();
			fileStream.seekg(position, std::fstream::beg);
		}
		else
		{
			std::fstream::pos_type position = fileStream.tellp();
			fileStream.seekp(0, std::fstream::end);
			length = fileStream.tellp();
			fileStream.seekp(position, std::fstream::beg);
		}

		return static_cast<size_t>(length);
	}

	bool Stream::EndOfStream()
	{
		return GetPosition() >= GetLength();
	}

	const Stream::StreamBuffer& Stream::GetMemoryBuffer() const
	{
		return buffer;
	}

	const unsigned char* Stream::GetRawMemoryBuffer() const
	{
		if (streamMode != ModeMemory)
			return nullptr;

		return buffer.data();
	}

	size_t Stream::ReadData(const StreamBuffer& value)
	{
		if (streamMode == ModeMemory)
		{
			unsigned int size = std::max(static_cast<int>(value.size()), static_cast<int>(value.size() - buffer.size()));
			memcpy(const_cast<void*>(reinterpret_cast<const void*>(value.data())), buffer.data() + position, size);
			position += value.size();

			return size;
		}
		else
		{
			size_t start = static_cast<size_t>(fileStream.tellg());
			fileStream.read(reinterpret_cast<char*>(const_cast<unsigned char*>(value.data())),
							value.size() + GetPosition() > GetLength()
								? GetLength() - GetPosition()
								: value.size());
			size_t end = static_cast<size_t>(fileStream.tellg());
			
			return end - start;
		}

		return value.size();
	}

	bool Stream::ReadBoolean()
	{
		return ReadByte() != 0;
	}
	
	unsigned char Stream::ReadByte()
	{
		unsigned char value = 0;
	
		if (streamMode == ModeMemory)
		{
			value = *reinterpret_cast<unsigned char*>(buffer.data() + position);
			position += sizeof(unsigned char);
		}
		else
			fileStream.read(reinterpret_cast<char*>(&value), sizeof(unsigned char));

		return value;
	}

	unsigned short Stream::ReadWord()
	{
		unsigned short value = 0;

		if (streamMode == ModeMemory)
		{
			value = *reinterpret_cast<unsigned short*>(buffer.data() + position);
			position += sizeof(unsigned short);
		}
		else
			fileStream.read(reinterpret_cast<char*>(&value), sizeof(unsigned short));

		return value;
	}

	unsigned int Stream::ReadDword()
	{
		unsigned int value = 0;

		if (streamMode == ModeMemory)
		{
			value = *reinterpret_cast<unsigned int*>(buffer.data() + position);
			position += sizeof(unsigned int);
		}
		else
			fileStream.read(reinterpret_cast<char*>(&value), sizeof(unsigned int));

		return value;
	}

	unsigned long long Stream::ReadQword()
	{
		unsigned long long value = 0;

		if (streamMode == ModeMemory)
		{
			value = *reinterpret_cast<unsigned long long*>(buffer.data() + position);
			position += sizeof(unsigned long long);
		}
		else
			fileStream.read(reinterpret_cast<char*>(&value), sizeof(unsigned long long));

		return value;
	}

	double Stream::ReadDouble()
	{
		double value = 0.0;

		if (streamMode == ModeMemory)
		{
			value = *reinterpret_cast<double*>(buffer.data() + position);
			position += sizeof(double);
		}
		else
			fileStream.read(reinterpret_cast<char*>(&value), sizeof(double));

		return value;
	}

	float Stream::ReadSingle()
	{
		float value = 0.0f;

		if (streamMode == ModeMemory)
		{
			value = *reinterpret_cast<float*>(buffer.data() + position);
			position += sizeof(float);
		}
		else
			fileStream.read(reinterpret_cast<char*>(&value), sizeof(float));

		return value;
	}

	std::string Stream::ReadString()
	{
		unsigned int length = 0;
		unsigned char b = 0;

		do
		{
			b = ReadByte();
			length = (length << 7) | (b & 0x7F);
		}
		while((b & 0x80));

		if (streamMode == ModeMemory)
		{
			std::string value(reinterpret_cast<const char*>(buffer.data() + position), length);
			position += length;

			return value;
		}
		else
		{
			std::string value(length, '\0');
			fileStream.read(const_cast<char*>(value.data()), length);

			return value;
		}

		return "";
	}

	Vm::Variant Stream::ReadVariant()
	{
		unsigned char type = ReadByte();
		switch(type)
		{
			case Vm::Variant::Type::Nil:
				return Vm::Variant();

			case Vm::Variant::Type::Integer:
				return Vm::Variant(static_cast<int64_t>(ReadQword()));

			case Vm::Variant::Type::Double:
				return Vm::Variant(ReadDouble());

			case Vm::Variant::Type::String:
				return Vm::Variant(ReadString());

			case Vm::Variant::Type::List:
			{
				std::vector<Vm::Variant*> values;
				unsigned int count = ReadDword();
				while(count--)
					values.push_back(new Vm::Variant(ReadVariant()));

				return Vm::Variant(values);
			}

			case Vm::Variant::Type::Object:
				return Vm::Variant(nullptr);
		}

		return Vm::Variant();
	}

	Video::Color Stream::ReadColor()
	{
		unsigned int rgba = ReadDword();
		return Video::Color(ColorGetRed(rgba), ColorGetGreen(rgba), ColorGetBlue(rgba), ColorGetAlpha(rgba));
	}

	void Stream::WriteData(const StreamBuffer& value)
	{
		if (streamMode == ModeMemory)
		{
			ExpandMemoryStream(value.size());
			std::memcpy(buffer.data() + position, value.data(), value.size());
			position += value.size();
		}
		else
			fileStream.write(reinterpret_cast<const char*>(value.data()), value.size());
	}

	void Stream::WriteBoolean(bool value)
	{
		WriteByte(value != 0);
	}

	void Stream::WriteByte(unsigned char value)
	{
		if (streamMode == ModeMemory)
		{
			ExpandMemoryStream(sizeof(unsigned char));
			*reinterpret_cast<unsigned char*>(buffer.data() + position) = value;
			position += sizeof(unsigned char);
		}
		else
			fileStream.write(reinterpret_cast<const char*>(&value), sizeof(unsigned char));
	}

	void Stream::WriteWord(unsigned short value)
	{
		if (streamMode == ModeMemory)
		{
			ExpandMemoryStream(sizeof(unsigned short));
			*reinterpret_cast<unsigned short*>(buffer.data() + position) = value;
			position += sizeof(unsigned short);
		}
		else
			fileStream.write(reinterpret_cast<const char*>(&value), sizeof(unsigned short));
	}

	void Stream::WriteDword(unsigned int value)
	{
		if (streamMode == ModeMemory)
		{
			ExpandMemoryStream(sizeof(unsigned int));
			*reinterpret_cast<unsigned int*>(buffer.data() + position) = value;
			position += sizeof(unsigned int);
		}
		else
			fileStream.write(reinterpret_cast<const char*>(&value), sizeof(unsigned int));
	}

	void Stream::WriteQword(unsigned long long value)
	{
		if (streamMode == ModeMemory)
		{
			ExpandMemoryStream(sizeof(unsigned long long));
			*reinterpret_cast<unsigned long long*>(buffer.data() + position) = value;
			position += sizeof(unsigned long long);
		}
		else
			fileStream.write(reinterpret_cast<const char*>(&value), sizeof(unsigned long long));
	}

	void Stream::WriteDouble(double value)
	{
		if (streamMode == ModeMemory)
		{
			ExpandMemoryStream(sizeof(double));
			*reinterpret_cast<double*>(buffer.data() + position) = value;
			position += sizeof(double);
		}
		else
			fileStream.write(reinterpret_cast<const char*>(&value), sizeof(double));
	}

	void Stream::WriteSingle(float value)
	{
		if (streamMode == ModeMemory)
		{
			ExpandMemoryStream(sizeof(float));
			*reinterpret_cast<float*>(buffer.data() + position) = value;
			position += sizeof(float);
		}
		else
			fileStream.write(reinterpret_cast<const char*>(&value), sizeof(float));
	}

	void Stream::WriteString(const std::string& value)
	{
		// TODO 7bit integer
		WriteByte(value.length());

		for(size_t i = 0; i < value.length(); ++i)
			WriteByte(value[i]);
	}

	void Stream::Deflate()
	{
		size_t deflatedLength = 0;

		unsigned char* deflatedBuffer = DeflateStream(buffer, &deflatedLength);
		if (deflatedLength == 0)
		{
			delete[] deflatedBuffer;
			return;
		}

		buffer.resize(deflatedLength);
		memcpy(buffer.data(), deflatedBuffer, deflatedLength);
		position = 0;

		delete[] deflatedBuffer;
	}
	
	void Stream::Inflate()
	{
		size_t inflatedLength = 0;

		unsigned char* inflatedBuffer = InflateStream(buffer, &inflatedLength);
		if (inflatedLength == 0)
		{
			delete[] inflatedBuffer;
			return;
		}

		buffer.resize(inflatedLength);
		memcpy(buffer.data(), inflatedBuffer, inflatedLength);
		position = 0;

		delete[] inflatedBuffer;
	}

	Stream* Stream::Deserialize(bool decompress)
	{
		Stream* value = new Stream();

		unsigned int length = ReadDword();
		StreamBuffer tmpBuffer(length, 0);
		ReadData(tmpBuffer);

		value->WriteData(tmpBuffer);
		value->Rewind();

		if (decompress)
			value->Inflate();

		return value;
	}

	void Stream::Serialize(Stream* stream, bool compress)
	{
		if (stream != nullptr)
		{
			if (compress)
				stream->Deflate();

			WriteDword(stream->GetLength());
			WriteData(stream->GetMemoryBuffer());
		}
		else
			WriteDword(0);
	}

	StreamException::StreamException(const std::string& _message)
		: message(_message)
	{

	}

	const char* StreamException::what() const throw()
	{
		return message.c_str();
	}
}
