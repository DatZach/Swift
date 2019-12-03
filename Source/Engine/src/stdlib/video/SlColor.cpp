/*
 *	stdlibcolor.cpp
 *	Swift Standard Library
 *	Color
 */

#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace Color
	{
		/*
		 *	class Color;
		 *	ctor()
		 *	ctor(R, G, B, A)
		 *	
		 *	R		0 - 255
		 *	G		0 - 255
		 *	B		0 - 255
		 *	A		0 - 255
		 */

		void ConstructorDefault(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			//StackPush(parent);
		}

		void ConstructorRGBA(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Vm::Variant* r = stack.Pop();
			Vm::Variant* g = stack.Pop();
			Vm::Variant* b = stack.Pop();
			Vm::Variant* a = stack.Pop();

			if (r->TypeOf() != Vm::Variant::Type::Integer || r->Integer() < 0 || r->Integer() > 255)
				Error << "Color(): Argument 1 must be an integer between 0 and 255." << lendl;
			else if (g->TypeOf() != Vm::Variant::Type::Integer || g->Integer() < 0 || g->Integer() > 255)
				Error << "Color(): Argument 2 must be an integer between 0 and 255." << lendl;
			else if (b->TypeOf() != Vm::Variant::Type::Integer || b->Integer() < 0 || b->Integer() > 255)
				Error << "Color(): Argument 3 must be an integer between 0 and 255." << lendl;
			else if (a->TypeOf() != Vm::Variant::Type::Integer || a->Integer() < 0 || a->Integer() > 255)
				Error << "Color(): Argument 4 must be an integer between 0 and 255." << lendl;
			else
			{
				parent->Field("r") = *r;
				parent->Field("g") = *g;
				parent->Field("b") = *b;
				parent->Field("a") = *a;
			}

			//StackPush(parent);
		}

		void ConstructorRGB(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Vm::Variant* r = stack.Pop();
			Vm::Variant* g = stack.Pop();
			Vm::Variant* b = stack.Pop();

			if (r->TypeOf() != Vm::Variant::Type::Integer || r->Integer() < 0 || r->Integer() > 255)
				Error << "Color(): Argument 1 must be an integer between 0 and 255." << lendl;
			else if (g->TypeOf() != Vm::Variant::Type::Integer || g->Integer() < 0 || g->Integer() > 255)
				Error << "Color(): Argument 2 must be an integer between 0 and 255." << lendl;
			else if (b->TypeOf() != Vm::Variant::Type::Integer || b->Integer() < 0 || b->Integer() > 255)
				Error << "Color(): Argument 3 must be an integer between 0 and 255." << lendl;
			else
			{
				parent->Field("r") = *r;
				parent->Field("g") = *g;
				parent->Field("b") = *b;
				parent->Field("a") = 255;
			}

			//StackPush(parent);
		}

		/*!
			class Color

			fields
			r				int
			g				int
			b				int
			a				int

			methods
			__Constructor	0
			__Constructor	4
			__Constructor	3
		 */
		void Install()
		{
			// Color class
			Vm::Class colorClass;

			// Fields
			colorClass["r"] = 0;
			colorClass["g"] = 0;
			colorClass["b"] = 0;
			colorClass["a"] = 255;

			// Methods
			colorClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorDefault, 0);
			colorClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorRGBA, 4);
			colorClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorRGB, 3);

			VirtualMachine.AddClass("Color", colorClass);
		}
	}
}
