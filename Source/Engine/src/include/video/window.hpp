/*
 *	window.hpp
 *	Window
 */

#ifndef __VIDEO_WINDOW_HPP
#define __VIDEO_WINDOW_HPP

#include <string>
#include <Engine/Event.hpp>
#include <Video/RenderTarget.hpp>
#include <Util/Vector2.hpp>
#include <GLFW/glfw3.h>

namespace Video
{
	class Window : public RenderTarget
	{
	public:
		static const int DefaultWidth	= 800;
		static const int DefaultHeight	= 600;

	private:
		GLFWwindow* window;

	public:
		Window(const std::string& title, const Vector2u& size);
		~Window();

		void Show() const;
		void Hide() const;

		void AttachContext() const;
		void DetachContext() const;

		void WaitEvents() const;
		void PollEvents() const;
		void SwapBuffers() const;

		void SetSize(Vector2i size);
		void SetTitle(const std::string& value);
		void SetClipboardText(const std::string& value);

		void GetCursorPos(double* x, double* y) const;

		Vector2i GetSize() const;
		std::string GetClipboardText() const;
		GLFWwindow* GetGLFWWindowHandle() const;
		bool ShouldClose() const;
	};
}

#endif
