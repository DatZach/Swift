/*
 *	video.hpp
 *	Video Subsystem
 */

#ifndef __VIDEO_VIDEO_HPP
#define __VIDEO_VIDEO_HPP

#define GLEW_STATIC

#ifdef WINDOWS
	#include <Windows.h>
#endif

#include <exception>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#ifdef DEBUG
	#define GlCheckError() \
		Video::System::CheckError(__FILE__, __LINE__)
#else
	#define GlCheckError()
#endif

namespace Video
{
	class System
	{
	public:
		static void Initialize();
		static void InitializeGLEW();
		static void Shutdown();

		static void CheckError(const char* filename, unsigned int line);
	};

	class VideoException : public std::exception
	{
	private:
		std::string message;

	public:
		VideoException(const std::string& _message);

		virtual const char* what() const throw();
	};
}

#endif
