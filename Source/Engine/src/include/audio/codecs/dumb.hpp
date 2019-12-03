/*
 *	dumb.hpp
 *	DUMB Codec (IT, MOD, XM, S3M)
 */

#ifndef __AUDIO_CODEC_DUMB_HPP
#define __AUDIO_CODEC_DUMB_HPP

#include <Audio/Codec.hpp>
#include <dumb/dumb.h>

namespace Audio
{
	class CodecDumb : public Codec
	{
	private:
		DUH_SIGRENDERER* renderer;
		DUH* duhFile;
		float delta;

	public:
		CodecDumb();
		~CodecDumb();

		virtual bool Open(const std::string& filename);
		virtual size_t Read(short* data, size_t count);
		virtual void Seek(unsigned int offset);
	};
}

#endif
