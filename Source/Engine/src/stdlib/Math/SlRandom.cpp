/*
 *	random.cpp
 *	Swift Standard Library
 *	Random
 */

#include <iostream>
#include <ctime>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace Random
	{
		void SetSeed(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);
			srand(*stack.Pop());
		}

		void Next(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			StackPush(static_cast<double>(rand()));
		}

		void NextMax(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			long long max = *stack.Pop();

			StackPush(static_cast<double>(rand() % max));
		}

		void NextRange(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);
			ExpectType(1, Vm::Variant::Type::Integer);

			long long min = *stack.Pop();
			long long max = *stack.Pop();

			StackPush(rand() % (max - min) + min);
		}

		void Choose(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			// TODO Choose(<List>) for VA args

			Vm::Variant* x = stack.Pop();
			Vm::Variant* y = stack.Pop();

			stack.Push((rand() % 2 == 0) ? x : y);
		}

		/*!
			class Random

			methods
			SetSeed		1 static
			Next		0 static
			Next		1 static
			Next		2 static
			Choose		2 static
		 */
		void Install()
		{
			// Set random seed
			srand(static_cast<unsigned int>(time(nullptr)));

			// Random class
			Vm::Class randomClass;

			// Methods
			randomClass.AddMethod("Next", Next, 0);
			randomClass.AddMethod("Next", NextMax, 1);
			randomClass.AddMethod("Next", NextRange, 2);
			randomClass.AddMethod("Choose", Choose, 2);

			VirtualMachine.AddClass("Random", randomClass);
		}
	}
}
