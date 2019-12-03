/*
 *	system.hpp
 *	Audio System
 */

#ifndef __AUDIO_SYSTEM_HPP
#define __AUDIO_SYSTEM_HPP

#include <exception>
#include <string>
#include <AL/al.h>
#include <AL/alc.h>

#ifdef DEBUG
	#define AlCheckError() \
		Audio::System::CheckError(__FILE__, __LINE__)
#else
	#define AlCheckError()
#endif

namespace Audio
{
	class System
	{
	public:
		static void Initialize();
		static void Shutdown();

		static bool IsInitialized();

		static void SetMuteAll(bool value);
		static bool GetMuteAll();

		static unsigned int GetFormatFromChannelCount(unsigned int count);

		static void CheckError(const char* filename, unsigned int line);
	};

	class AudioException : public std::exception
	{
	private:
		std::string message;

	public:
		AudioException(const std::string& message);

		virtual const char* what() const throw();
	};
}

#endif
