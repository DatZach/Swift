/*
 *	dumb.cpp
 *	DUMB Codec
 */

#include <vector>
#include <Audio/Codecs/Dumb.hpp>
#include <Util/Logger.hpp>

namespace Audio
{
	CodecDumb::CodecDumb()
		: Codec(),
		  renderer(nullptr),
		  duhFile(nullptr),
		  delta(0.0f)
	{
		static bool dumbInitialized = false;

		if (dumbInitialized)
			return;
		
		dumb_register_stdfiles();
		dumbInitialized = true;
	}

	CodecDumb::~CodecDumb()
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

	bool CodecDumb::Open(const std::string& filename)
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

		DUMBFILE* dumbFile;
		std::vector<DUH* (*)(DUMBFILE*)> codecs;
		codecs.push_back(dumb_read_it);
		codecs.push_back(dumb_read_xm);
		codecs.push_back(dumb_read_mod);
		codecs.push_back(dumb_read_s3m);

		for(auto& codec : codecs)
		{
			// Open file
			dumbFile = dumbfile_open(filename.c_str());
			if (dumbFile == nullptr)
				return false;

			// Try this codec
			duhFile = codec(dumbFile);
			if (duhFile != nullptr)
				break;

			dumbfile_close(dumbFile);
		}

		if (duhFile == nullptr)
			return false;

		// Parse audio information for rendering
		channelCount = 1;

		// TODO You really do need to resolve this
		sampleRate = 44100;
		
		DUMB_IT_SIGDATA* sigData = duh_get_it_sigdata(duhFile);
		samples = dumb_it_sd_get_n_samples(sigData);

		// This is 65536.0f according to the DUMB documentation
		delta = 65536.0f / static_cast<float>(sampleRate);

		dumbfile_close(dumbFile);

		return true;
	}

	size_t CodecDumb::Read(short* data, size_t count)
	{
		if (renderer == nullptr)
			return 0;
		
		return duh_render(renderer, 16, 0, 1.0f, delta, count, data);
	}

	void CodecDumb::Seek(unsigned int offset)
	{
		if (renderer)
		{
			duh_end_sigrenderer(renderer);
			renderer = nullptr;
		}

		renderer = duh_start_sigrenderer(duhFile, 0, channelCount, offset << 16);
	}
}
