/*
 *	ogg.hpp
 *	OGG Codec
 */

#ifndef __AUDIO_CODEC_OGG_HPP
#define __AUDIO_CODEC_OGG_HPP

#include <Audio/Codec.hpp>
#include <dumb/dumbogg.h>

namespace Audio
{
	class CodecOgg : public Codec
	{
	private:
		DUH_SIGRENDERER* renderer;
		DUH* duhFile;

	public:
		CodecOgg();
		~CodecOgg();

		virtual bool Open(const std::string& filename);
		virtual size_t Read(short* data, size_t count);
		virtual void Seek(unsigned int offset);
	};
}

#endif
