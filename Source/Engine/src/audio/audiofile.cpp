/*
 *	AudioFile.cpp
 *	Audio File
 */

#include <Audio/Audio.hpp>
#include <Audio/AudioFile.hpp>
#include <Audio/Codecs/Ogg.hpp>
#include <Audio/Codecs/Dumb.hpp>
#include <Audio/Codecs/Wav.hpp>
#include <Util/Logger.hpp>

namespace Audio
{
	AudioFile::AudioFile()
		: codecs(),
		  currentCodec(nullptr)
	{
		codecs.push_back(new CodecOgg());
		codecs.push_back(new CodecDumb());
		codecs.push_back(new CodecWav());
	}

	AudioFile::~AudioFile()
	{
		for(Codec* codec : codecs)
			delete codec;
	}

	bool AudioFile::Open(const std::string& filename)
	{
		// Don't do anything if the audio system isn't initialized
		if (!System::IsInitialized())
		{
			Warn << "Cannot open audio file because the audio system isn't initialized." << lendl;
			return false;
		}

		// Try to open this file with all known codecs
		for(Codec* codec : codecs)
		{
			if (!codec->Open(filename))
				continue;

			currentCodec = codec;
			return true;
		}

		// TODO Might as well delete unused codecs here

		currentCodec = nullptr;
		return false;
	}

	size_t AudioFile::Read(short* data, size_t count)
	{
		if (currentCodec == nullptr || data == nullptr || !count)
			return 0;

		return currentCodec->Read(data, count);
	}

	void AudioFile::Seek(unsigned int offset)
	{
		if (currentCodec == nullptr)
			return;

		currentCodec->Seek(offset);
	}

	size_t AudioFile::GetSampleCount() const
	{
		if (currentCodec == nullptr)
			return 0;

		return currentCodec->GetSampleCount();
	}

	size_t AudioFile::GetChannelCount() const
	{
		if (currentCodec == nullptr)
			return 0;

		return currentCodec->GetChannelCount();
	}

	size_t AudioFile::GetSampleRate() const
	{
		if (currentCodec == nullptr)
			return 0;

		return currentCodec->GetSampleRate();
	}
}
