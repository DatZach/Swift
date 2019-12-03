/*
 *	wav.cpp
 *	Wave Codec
 */

#include <Audio/Codecs/Wav.hpp>
#include <Util/Logger.hpp>

#define WAV_MAGIC_VALUE(a, b, c, d)	\
	(((unsigned int)(a)      ) |	\
	 ((unsigned int)(b) <<  8) |	\
	 ((unsigned int)(c) << 16) |	\
	 ((unsigned int)(d) << 24))

#define WAV_MAGIC_RIFF			WAV_MAGIC_VALUE('R', 'I', 'F', 'F')
#define WAV_MAGIC_WAVE			WAV_MAGIC_VALUE('W', 'A', 'V', 'E')
#define WAV_MAGIC_FORMAT		WAV_MAGIC_VALUE('f', 'm', 't', ' ')
#define WAV_MAGIC_DATA			WAV_MAGIC_VALUE('d', 'a', 't', 'a')

namespace Audio
{
	CodecWav::CodecWav()
		: Codec(),
		  stream(nullptr),
		  bits(0),
		  offset(0),
		  sampleSize(0)
	{

	}

	CodecWav::~CodecWav()
	{
		delete stream;
	}

	bool CodecWav::Open(const std::string& filename)
	{
		try
		{
			stream = new Util::Stream(filename.c_str(), Util::Stream::ModeRead);
			if (stream == nullptr)
				return false;
		}
		catch(Util::StreamException&)
		{
			return false;
		}

		// Check 'RIFF'
		if (stream->ReadDword() != WAV_MAGIC_RIFF)
			return false;

		stream->Seek(sizeof(unsigned int), SEEK_CUR);

		// Check 'WAVE'
		if (stream->ReadDword() != WAV_MAGIC_WAVE)
			return false;

		bool parse = true;
		while(parse)
		{
			unsigned int tag = stream->ReadDword();
			switch(tag)
			{
				case WAV_MAGIC_FORMAT:
				{
					int length = stream->ReadDword();
					if (length < 16)
						return false;

					// PCM should be 1
					short pcm = stream->ReadWord();
					if (pcm != 1)
						return false;

					channelCount = stream->ReadWord();
					if (channelCount != 1 && channelCount != 2)
						return false;

					sampleRate = stream->ReadDword();

					stream->Seek(6, SEEK_CUR);

					bits = stream->ReadWord();
					if (bits != 8 && bits != 16)
						return false;

					stream->Seek(length - 16, SEEK_CUR);

					sampleSize = channelCount * bits / 8;
					break;
				}

				case WAV_MAGIC_DATA:
					samples = stream->ReadDword();
					if (channelCount == 2)
						samples = (samples + 1) / 2;

					if (bits == 16)
						samples /= 2;

					offset = stream->GetPosition();

					parse = false;
					break;

				default:
				{
					stream->Seek(stream->ReadDword(), SEEK_CUR);

					VerboseTrace << "Audio::CodecWav: Ignoring chunk \""
								 << (char)(tag >> 24)
								 << (char)(tag >> 16)
								 << (char)(tag >> 8)
								 << (char)tag
								 << "\"."
								 << lendl;
				}
			}
		}

		return true;
	}

	size_t CodecWav::Read(short* data, size_t count)
	{
		Util::Stream::StreamBuffer buffer(count);
		size_t bytesRead = stream->ReadData(buffer);
		memcpy(data, buffer.data(), bytesRead);

		return bytesRead;
	}

	void CodecWav::Seek(unsigned int streamOffset)
	{
		stream->Seek(offset + streamOffset * sampleRate, std::ios::beg);
	}
}
