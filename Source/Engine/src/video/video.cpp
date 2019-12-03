/*
 *	video.cpp
 *	Video Subsystem
 */

#include <Engine/Game.hpp>
#include <Video/Video.hpp>
#include <Util/Logger.hpp>

namespace Video
{
	void VideoGLFWErrorCallback(int code, const char* what)
	{
		Error << "GLFW Error code " << code << ": "
			  << what << lendl;
	}

	void System::Initialize()
	{
		glfwSetErrorCallback(VideoGLFWErrorCallback);

		if (!glfwInit())
			throw VideoException("Failed to initialize GLFW");

		// Request OpenGL 3.2 or better
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

		// Anti Aliasing
		glfwWindowHint(GLFW_SAMPLES, Assets.config.AntiAliasingLevel);

#if DEBUG
		int glfwMajor, glfwMinor, glfwRevision;
		glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

		VerboseTrace << "GLFW v" << glfwMajor << "." << glfwMinor << "." << glfwRevision << lendl;
#endif
	}

	void System::InitializeGLEW()
	{
		static bool glewInitialized = false;
		
		// Don't initialize GLEW if we've already done so
		if (glewInitialized)
			return;

		// Initialize GLEW
		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
			throw VideoException("Failed to initialize GLEW");

		if (glGenVertexArrays == nullptr)
			throw VideoException("GLEW didn't initialize what we needed.");

		// Ignore GLEW throwing GL errors, known bug, should be harmless
		// TODO I should really report this as a bug report
		glGetError();

#ifdef DEBUG
		VerboseTrace << "GLEW v"
					 << reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))
					 << lendl;
#endif
	}

	void System::Shutdown()
	{
		glfwTerminate();
	}

	void System::CheckError(const char* filename, unsigned int line)
	{
		GLenum glError = glGetError();
		std::string message;

		switch(glError)
		{
			case GL_INVALID_ENUM:
				message = "GL_INVALID_ENUM";
				break;

			case GL_INVALID_VALUE:
				message = "GL_INVALID_VALUE";
				break;

			case GL_INVALID_OPERATION:
				message = "GL_INVALID_OPERATION";
				break;

			case GL_STACK_OVERFLOW:
				message = "GL_STACK_OVERFLOW";
				break;

			case GL_STACK_UNDERFLOW:
				message = "GL_STACK_UNDERFLOW";
				break;

			case GL_OUT_OF_MEMORY:
				message = "GL_OUT_OF_MEMORY";
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				message = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;

			case GL_TABLE_TOO_LARGE:
				message = "GL_TABLE_TOO_LARGE";
				break;

			default:
				message = "Unknown Error";
		}

		if (glError != GL_NO_ERROR)
		{
			Error << "OpenGL Error " << glError << " (" << message << ")" << lendl
				  << ltab << filename << " " << line << lendl;
		}
	}

	VideoException::VideoException(const std::string& _message)
		: message(_message)
	{
		GlCheckError();
	}

	const char* VideoException::what() const throw()
	{
		return message.c_str();
	}
}
