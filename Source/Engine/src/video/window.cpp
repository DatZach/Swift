/*
 *	window.cpp
 *	Window
 */

#include <Engine/Game.hpp>
#include <Video/Window.hpp>
#include <Video/Video.hpp>
#include <Util/Logger.hpp>

namespace Video
{
	void EventFocusChangeCallback(GLFWwindow*, int value)
	{
		Engine::Event event;
		event.type					= Engine::Event::FocusChange;
		event.focusChanged.focused	= value == GL_TRUE;

		Game.DispatchEvent(event);
	}

	void EventKeyCallback(GLFWwindow*, int keyCode, int, int action, int mods)
	{
		Engine::Event event;
		event.type			= Engine::Event::Key;
		event.key.keyCode	= keyCode;
		event.key.action	= action;
		event.key.mods		= mods;

		Game.DispatchEvent(event);
	}

	void EventMouseButtonCallback(GLFWwindow* window, int button, int action, int)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		
		Engine::Event event;
		event.type					= Engine::Event::MouseButton;
		event.mouseButton.button	= button;
		event.mouseButton.action	= action;
		event.mouseButton.x			= static_cast<int>(x);
		event.mouseButton.y			= static_cast<int>(y);

		Game.DispatchEvent(event);
	}

	void EventMousePositionCallback(GLFWwindow*, double x, double y)
	{
		Engine::Event event;
		event.type				= Engine::Event::MousePosition;
		event.mousePosition.x	= static_cast<int>(x);
		event.mousePosition.y	= static_cast<int>(y);

		Game.DispatchEvent(event);
	}

	void EventMouseScrollCallback(GLFWwindow*, double x, double y)
	{
		Engine::Event event;
		event.type			= Engine::Event::MouseScroll;
		event.mouseScroll.x	= x;
		event.mouseScroll.y	= y;

		Game.DispatchEvent(event);
	}

	Window::Window(const std::string& title, const Vector2u& size)
		: window(nullptr)
	{
		// Get monitor information
		GLFWmonitor* primaryDisplay = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(primaryDisplay);

		// Create window
		window = Assets.config.Fullscreen
					? glfwCreateWindow(videoMode->width, videoMode->height, title.c_str(), primaryDisplay, nullptr)
					: glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		
		if (window == nullptr)
			throw VideoException("Cannot create window.");

		// Center window and set window title
		glfwSetWindowPos(window, videoMode->width / 2 - size.x / 2, videoMode->height / 2 - size.y / 2);

		// Set context
		AttachContext();

		// Ensure that GLEW is initialized now that we have an OGL context
		System::InitializeGLEW();

		// Initialize render target
		InitializeTarget();
		SetView(Vector2f(size));

		// Set callbacks
		glfwSetWindowFocusCallback(window, EventFocusChangeCallback);
		glfwSetKeyCallback(window, EventKeyCallback);
		glfwSetMouseButtonCallback(window, EventMouseButtonCallback);
		glfwSetCursorPosCallback(window, EventMousePositionCallback);
		glfwSetScrollCallback(window, EventMouseScrollCallback);

#ifdef DEBUG
		VerboseTrace << "OpenGL v"
					 << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR)
					 << "."
					 << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR)
					 << "."
					 << glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION)
					 << lendl;
#endif

		GlCheckError();
	}

	Window::~Window()
	{
		glfwDestroyWindow(window);
	}

	void Window::Show() const
	{
		glfwShowWindow(window);
	}

	void Window::Hide() const
	{
		glfwHideWindow(window);
	}

	void Window::AttachContext() const
	{
		glfwMakeContextCurrent(window);
	}
	
	void Window::DetachContext() const
	{
		// Note GLFW throws an OGL error here, nothing I can do
		glfwMakeContextCurrent(nullptr);
	}

	void Window::WaitEvents() const
	{
		glfwWaitEvents();
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(window);
	}

	void Window::SetSize(Vector2i size)
	{
		glfwSetWindowSize(window, size.x, size.y);
	}

	void Window::SetTitle(const std::string& value)
	{
		glfwSetWindowTitle(window, value.c_str());
	}

	void Window::SetClipboardText(const std::string& value)
	{
		glfwSetClipboardString(window, value.c_str());
	}

	Vector2i Window::GetSize() const
	{
		Vector2i size;
		glfwGetWindowSize(window, &size.x, &size.y);

		return size;
	}

	std::string Window::GetClipboardText() const
	{
		return glfwGetClipboardString(window);
	}

	void Window::GetCursorPos(double* x, double* y) const
	{
		glfwGetCursorPos(window, x, y);
	}

	GLFWwindow* Window::GetGLFWWindowHandle() const
	{
		return window;
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window) == 1;
	}
}
