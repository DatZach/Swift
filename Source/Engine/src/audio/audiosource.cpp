/*
 *	soundsource.cpp
 *	Sound Source
 */

#include <iostream>
#include <Audio/AudioSource.hpp>
#include <Audio/Audio.hpp>

namespace Audio
{
	AudioSource::AudioSource()
		: id(0)
	{
		alGenSources(1, &id);
		alSourcei(id, AL_BUFFER, 0);

		if (System::GetMuteAll())
			SetVolume(0.0f);
	}

	AudioSource::~AudioSource()
	{
		alSourcei(id, AL_BUFFER, 0);
		alDeleteSources(1, &id);
	}

	AudioSource::Status AudioSource::GetStatus() const
	{
		ALint status;
		alGetSourcei(id, AL_SOURCE_STATE, &status);

		switch(status)
		{
			case AL_INITIAL:
			case AL_STOPPED:
				return Stopped;

			case AL_PAUSED:
				return Paused;

			case AL_PLAYING:
				return Playing;
		}

		return Stopped;
	}

	void AudioSource::SetPitch(float value) const
	{
		alSourcef(id, AL_PITCH, value);
	}

	void AudioSource::SetVolume(float value) const
	{
		alSourcef(id, AL_GAIN, value);
	}

	void AudioSource::SetPan(float value) const
	{
		alSource3f(id, AL_POSITION, value, 0.0f, 0.0f);
		alSourcei(id, AL_SOURCE_RELATIVE, AL_TRUE);
	}

	float AudioSource::GetPitch() const
	{
		ALfloat value;
		alGetSourcef(id, AL_PITCH, &value);

		return value;
	}

	float AudioSource::GetVolume() const
	{
		ALfloat value;
		alGetSourcef(id, AL_GAIN, &value);

		return value;
	}

	float AudioSource::GetPan() const
	{
		ALfloat value;
		alGetSource3f(id, AL_POSITION, &value, nullptr, nullptr);

		return value;
	}
}
