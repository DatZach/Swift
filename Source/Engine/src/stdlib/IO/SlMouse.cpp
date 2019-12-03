/*
 *	mouse.cpp
 *	Swift Standard Library
 *	Mouse
 */

#include <StdLib/SlApi.hpp>
#include <Engine/Game.hpp>
#include <GLFW/glfw3.h>

namespace StdLib
{
	namespace Mouse
	{
		void Pressed(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			unsigned int mouseButton = *stack.Pop();

			GLFWwindow* window = Game.GetWindow()->GetGLFWWindowHandle();
			bool current = Game.GetMouseButtonState(mouseButton);
			bool previous = Game.GetPreviousMouseButtonState(mouseButton);

			StackPush(current && !previous);
		}

		void Held(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			GLFWwindow* window = Game.GetWindow()->GetGLFWWindowHandle();
			long long value = glfwGetMouseButton(window, *stack.Pop());
			StackPush(value);
		}

		void Released(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			unsigned int mouseButton = *stack.Pop();

			GLFWwindow* window = Game.GetWindow()->GetGLFWWindowHandle();
			bool current = Game.GetMouseButtonState(mouseButton);
			bool previous = Game.GetPreviousMouseButtonState(mouseButton);

			StackPush(!current && previous);
		}

		void GetX(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			double x, y;

			GLFWwindow* window = Game.GetWindow()->GetGLFWWindowHandle();
			glfwGetCursorPos(window, &x, &y);
			StackPush(x);
		}

		void GetY(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			double x, y;

			GLFWwindow* window = Game.GetWindow()->GetGLFWWindowHandle();
			glfwGetCursorPos(window, &x, &y);
			StackPush(y);
		}

		/*!
			class Mouse

			properties
			X				get static
			Y				get static

			methods
			Pressed			1 static
			Held			1 static
			Released		1 static

			enum Button
			First			0
			Second			1
			Third			2
			Fourth			3
			Fifth			4
			Sixth			5
			Seventh			6
			Eighth			7
			Left			0
			Right			1
			Middle			2
			endenum
		 */
		void Install()
		{
			// Mouse class
			Vm::Class mouseClass;

			// Methods
			mouseClass.AddMethod("Pressed", Pressed, 1);
			mouseClass.AddMethod("Held", Held, 1);
			mouseClass.AddMethod("Released", Released, 1);
			mouseClass.AddMethod("__get_X", GetX, 0);
			mouseClass.AddMethod("__get_Y", GetY, 0);

			VirtualMachine.AddClass("Mouse", mouseClass);
		}
	}
}
