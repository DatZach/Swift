/*
 *	ogg.cpp
 *	OGG Codec
 */

#include <Audio/Codecs/Ogg.hpp>

namespace Audio
{
	CodecOgg::CodecOgg()
		: Codec(),
		  renderer(nullptr),
		  duhFile(nullptr)
	{
		static bool dumbInitialized = false;

		if (dumbInitialized)
			return;
		
		dumb_register_stdfiles();
		dumbInitialized = true;
	}

	CodecOgg::~CodecOgg()
	{
		if (renderer)
		{
			duh_end_sigrenderer(renderer);
			renderer = nullptr;
		}

		if (duhFile)
		{
			unload_duh(duhFile);
			duhFile = nullptr;
		}
	}

	bool CodecOgg::Open(const std::string& filename)
	{
		// Clean up before loading something new
		if (renderer)
		{
			duh_end_sigrenderer(renderer);
			renderer = nullptr;
		}

		if (duhFile)
		{
			unload_duh(duhFile);
			duhFile = nullptr;
		}

		// Try this codec
		duhFile = dumb_load_ogg(filename.c_str(), 0);
		if (duhFile == nullptr)
			return false;

		// Parse audio information for rendering
		channelCount = 1;
		sampleRate = 44100;

		DUMB_IT_SIGDATA* sigData = duh_get_it_sigdata(duhFile);
		samples = dumb_it_sd_get_n_samples(sigData);

		return true;
	}

	size_t CodecOgg::Read(short* data, size_t count)
	{
		if (renderer == nullptr)
			return 0;
		
		return duh_render(renderer, 16, 0, 100.0f, 0.0f, count, data);
	}

	void CodecOgg::Seek(unsigned int offset)
	{
		if (renderer)
		{
			duh_end_sigrenderer(renderer);
			renderer = nullptr;
		}

		renderer = duh_start_sigrenderer(duhFile, 0, channelCount, offset << 16);
	}
}
