/*
 *	sound.cpp
 *	Sound
 */

#include <Audio/Sound.hpp>

namespace Audio
{
	Sound::Sound()
		: AudioStream(),
		  file(),
		  samples(),
		  mutex()
	{

	}

	Sound::~Sound()
	{
		mutex.lock();
		Stop();
		mutex.unlock();
	}

	bool Sound::OnGetData(Chunk& chunk)
	{
		mutex.lock();

		chunk.samples	= samples.data();
		chunk.count		= file.Read(samples.data(), samples.size());

		mutex.unlock();

		return chunk.count == samples.size();
	}

	void Sound::OnSeek(unsigned int offset)
	{
		mutex.lock();

		file.Seek(offset);

		mutex.unlock();
	}

	bool Sound::Open(const std::string& filename)
	{
		Stop();

		if (!file.Open(filename))
			return false;

		unsigned int sampleRate = file.GetSampleRate();
		unsigned int channelCount = file.GetChannelCount();
		samples.resize(channelCount * sampleRate * sizeof(short));

		return Initialize(channelCount, sampleRate);
	}
}
