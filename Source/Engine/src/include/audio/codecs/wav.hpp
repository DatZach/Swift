/*
 *	wav.hpp
 *	Wave Codec
 */

#ifndef __AUDIO_WAVE_CODEC_HPP
#define __AUDIO_WAVE_CODEC_HPP

#include <Audio/Codec.hpp>
#include <Util/Stream.hpp>

namespace Audio
{
	class CodecWav : public Codec
	{
	private:
		Util::Stream* stream;
		int bits;
		size_t offset;
		size_t sampleSize;

	public:
		CodecWav();
		~CodecWav();

		virtual bool Open(const std::string& filename);
		virtual size_t Read(short* data, size_t count);
		virtual void Seek(unsigned int offset);
	};
}

#endif
