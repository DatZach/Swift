/*
 *	audio.cpp
 *	Audio Subsystem
 */

#include <Audio/Audio.hpp>
#include <Util/Logger.hpp>

namespace Audio
{
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;
	bool muteAll = false;

	void System::Initialize()
	{
		// Create device
		device = alcOpenDevice(nullptr);
		if (device == nullptr)
		{
			Warn << "Failed to initialize audio device." << lendl;
			return;
		}

		// Create context
		context = alcCreateContext(device, nullptr);
		if (context == nullptr)
		{
			Warn << "Failed to create audio context!" << lendl;
			return;
		}

		// And set out current context
		alcMakeContextCurrent(context);
	}

	void System::Shutdown()
	{
		alcMakeContextCurrent(nullptr);
		if (context)
			alcDestroyContext(context);

		if (device)
			alcCloseDevice(device);
	}

	bool System::IsInitialized()
	{
		return device != nullptr && context != nullptr;
	}

	void System::SetMuteAll(bool value)
	{
		muteAll = value;
	}

	bool System::GetMuteAll()
	{
		return muteAll;
	}

	unsigned int System::GetFormatFromChannelCount(unsigned int count)
	{
		switch(count)
		{
			case 1:
				return AL_FORMAT_MONO16;

			case 2:
				return AL_FORMAT_STEREO16;

			case 4:
				return alGetEnumValue("AL_FORMAT_QUAD16");

			case 6:
				return alGetEnumValue("AL_FORMAT_51CHN16");

			case 7:
				return alGetEnumValue("AL_FORMAT_61CHN16");

			case 8:
				return alGetEnumValue("AL_FORMAT_61CHN16");
		}

		return 0;
	}

	void System::CheckError(const char* filename, unsigned int line)
	{
		ALenum alError = alGetError();
		std::string message;

		switch(alError)
		{
			case AL_INVALID_NAME:
				message = "AL_INVALID_NAME";
				break;

			case AL_INVALID_ENUM:
				message = "AL_INVALID_ENUM";
				break;
			
			case AL_INVALID_VALUE:
				message = "AL_INVALID_VALUE";
				break;

			case AL_INVALID_OPERATION:
				message = "AL_INVALID_OPERATION";
				break;

			case AL_OUT_OF_MEMORY:
				message = "AL_OUT_OF_MEMORY";
				break;

			default:
				message = "Unknown Error";
		}

		if (alError != AL_NO_ERROR)
		{
			Error << "OpenAL Error " << alError << " (" << message << ")" << lendl
				  << ltab << filename << ' ' << line << lendl;
		}
	}

	AudioException::AudioException(const std::string& message)
		: message(message)
	{
		AlCheckError();
	}

	const char* AudioException::what() const throw()
	{
		return message.c_str();
	}
}
