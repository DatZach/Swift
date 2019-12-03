/*
 *	stream.hpp
 *	Binary file stream helper
 */

#ifndef __UTIL_STREAM_HPP
#define __UTIL_STREAM_HPP

#include <fstream>
#include <vector>
#include <string>
#include <Vm/Variant.hpp>
#include <Video/Color.hpp>

namespace Util
{
	#define BuildByte(a, b, c, d, e, f, g, h)			((static_cast<unsigned int>((a) & 0x01) << 7) | \
														 (static_cast<unsigned int>((b) & 0x01) << 6) | \
														 (static_cast<unsigned int>((c) & 0x01) << 5) | \
														 (static_cast<unsigned int>((d) & 0x01) << 4) | \
														 (static_cast<unsigned int>((e) & 0x01) << 3) | \
														 (static_cast<unsigned int>((f) & 0x01) << 2) | \
														 (static_cast<unsigned int>((g) & 0x01) << 1) | \
														 (static_cast<unsigned int>((h) & 0x01) << 0))

	#define GetBit(a, b)								(((a) & (1 << (b))) != 0)

	#define BuildColor(r, g, b, a)						(0x00000000 | \
														 ((r) << 24) | \
														 ((g) << 16) | \
														 ((b) << 8)  | \
														  (a))

	#define ColorGetRed(x)								(((x)) & 0xFF)
	#define ColorGetGreen(x)							(((x) >> 8) & 0xFF)
	#define ColorGetBlue(x)								(((x) >> 16) & 0xFF)
	#define ColorGetAlpha(x)							(((x) >> 24) & 0xFF)

	class StreamException;

	class Stream
	{
	public:
		typedef std::vector<unsigned char> StreamBuffer;
		typedef enum _StreamMode
		{
			ModeRead,
			ModeWrite,
			ModeMemory
		} StreamMode;

	private:
		std::fstream fileStream;
		StreamBuffer buffer;
		StreamMode streamMode;
		size_t position;

		void ExpandMemoryStream(size_t length);

		// Compression
		unsigned char* DeflateStream(const StreamBuffer& sourceBuffer, size_t *deflatedLength);
		unsigned char* InflateStream(const StreamBuffer& sourceBuffer, size_t *inflatedLength);

	public:
		Stream(const std::string& filename, StreamMode mode);
		Stream();
		~Stream();

		// Operations
		void Rewind();
		void Seek(int offset, int origin);
		size_t GetPosition();
		size_t GetLength();
		bool EndOfStream();
		const StreamBuffer& GetMemoryBuffer() const;
		const unsigned char* GetRawMemoryBuffer() const;

		// Reading
		size_t ReadData(const StreamBuffer& value);
		bool ReadBoolean();	
		unsigned char ReadByte();
		unsigned short ReadWord();
		unsigned int ReadDword();
		unsigned long long ReadQword();
		double ReadDouble();
		float ReadSingle();
		std::string ReadString();
		Vm::Variant ReadVariant();
		Video::Color ReadColor();

		// Writing
		void WriteData(const StreamBuffer& value);
		void WriteBoolean(bool value);
		void WriteByte(unsigned char value);
		void WriteWord(unsigned short value);
		void WriteDword(unsigned int value);
		void WriteQword(unsigned long long value);
		void WriteDouble(double value);
		void WriteSingle(float value);
		void WriteString(const std::string& value);
 
		// Compression
		void Deflate();
		void Inflate();

		// Serialization
		Stream* Deserialize(bool decompress = true);
		void Serialize(Stream* stream, bool compress = true);
	};

	class StreamException : public std::exception
	{
	private:
		std::string message;

	public:
		StreamException(const std::string& _message);

		virtual const char* what() const throw();
	};
}

#endif
