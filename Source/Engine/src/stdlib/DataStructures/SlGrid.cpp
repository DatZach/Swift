/*
 *	grid.cpp
 *	Grid
 */

#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace Grid
	{
		void ConstructorWidthHeight(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);

			int width = *stack.Pop();
			int height = *stack.Pop();

			if (width <= 0 || height <= 0)
			{
				Error << "Grid(): width and height must be larger than 0." << lendl;
				return;
			}

			parent->Field("width") = width;
			parent->Field("height") = height;

			Vm::Variant* gridHandle = new Vm::Variant[width * height];
			parent->Field("handle") = Vm::Variant(gridHandle, nullptr);

			// Initialize it to nil
			for(int y = 0; y < height; ++y)
			{
				for(int x = 0; x < width; ++x)
					gridHandle[y * width + x] = Vm::Variant();
			}
		}

		void Deconstructor(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			delete[] parent->Field("handle").EngineObject<Vm::Variant>();
		}

		void Get(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);

			int width = parent->Field("width");
			int height = parent->Field("height");

			int x = *stack.Pop();
			int y = *stack.Pop();

			if (x < 0 || y < 0 || x >= width || y >= height)
			{
				Error << "Grid.get(): Index ("
					  << x << ", "
					  << y << ") is out of bounds.";
				StackPush(Vm::Variant());
				return;
			}

			stack.Push(parent->Field("handle").EngineObject<Vm::Variant*>()[y * width + x]);
		}

		void Set(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectTypeNumber(0);
			ExpectTypeNumber(1);

			int width = parent->Field("width");
			int height = parent->Field("height");

			int x = *stack.Pop();
			int y = *stack.Pop();

			if (x < 0 || y < 0 || x >= width || y >= height)
			{
				Error << "Grid.set(): Index ("
					  << x << ", "
					  << y << ") is out of bounds.";
				StackPush(Vm::Variant());
				return;
			}

			parent->Field("handle").EngineObject<Vm::Variant*>()[y * width + x] = stack.Pop();
		}

		void GetWidth(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			StackPush(parent->Field("width"));
		}

		void GetHeight(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			StackPush(parent->Field("height"));
		}

		/*!
			class Grid

			properties
			Width			get int
			Height			get int

			methods
			__Constructor	2
			Get				2
			Set				3
		 */
		void Install()
		{
			// Grid class
			Vm::Class gridClass;

			// Fields
			gridClass["handle"] = nullptr;
			gridClass["width"] = 0;
			gridClass["height"] = 0;

			// Methods
			gridClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorWidthHeight, 2);
			gridClass.AddMethod(VM_CLASS_DTOR_NAME, Deconstructor, 0);
			gridClass.AddMethod("__get_Width", GetWidth, 0);
			gridClass.AddMethod("__get_Height", GetHeight, 0);
			gridClass.AddMethod("Get", Get, 2);
			gridClass.AddMethod("Set", Set, 3);

			VirtualMachine.AddClass("Grid", gridClass);
		}
	}
}
