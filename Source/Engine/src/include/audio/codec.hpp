/*
 *	codec.hpp
 *	Audio Codec
 */

#ifndef __AUDIO_CODEC_HPP
#define __AUDIO_CODEC_HPP

#include <vector>
#include <string>

namespace Audio
{
	class Codec
	{
	protected:
		size_t samples, channelCount, sampleRate;

		Codec()
			: samples(0),
			  channelCount(0),
			  sampleRate(0)
		{

		}

	public:
		virtual ~Codec()
		{

		}

		virtual bool Open(const std::string& filename) = 0;
		virtual size_t Read(short* data, size_t count) = 0;
		virtual void Seek(unsigned int offset) = 0;

		size_t GetSampleCount() const { return samples; }
		size_t GetChannelCount() const { return channelCount; }
		size_t GetSampleRate() const { return sampleRate; }
	};
}

#endif
