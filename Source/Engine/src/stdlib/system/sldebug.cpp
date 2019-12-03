/*
 *	console.cpp
 *	Swift Standard Library
 *	Console
 */

#include <iostream>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace Debug
	{
		void WriteLine(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::cout << stack.Pop()->Cast(Vm::Variant::Type::String).String() << std::endl;
		}

		void Write(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::cout << stack.Pop()->Cast(Vm::Variant::Type::String).String();
		}

		void Log(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			::Log << stack.Pop()->Cast(Vm::Variant::Type::String).String() << lendl;
		}

		void Trace(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			::Trace << stack.Pop()->Cast(Vm::Variant::Type::String).String() << lendl;
		}

		void Warn(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			::Warn << stack.Pop()->Cast(Vm::Variant::Type::String).String() << lendl;
		}

		void Error(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			::Error << stack.Pop()->Cast(Vm::Variant::Type::String).String() << lendl;
		}

		/*!
			class Debug

			methods
			WriteLine		1 static
			Write			1 static
			Log				1 static
			Trace			1 static
			Warn			1 static
			Error			1 static
		 */
		void Install()
		{
			// Debug class
			Vm::Class debugClass;

			// Methods
			debugClass.AddMethod("WriteLine", WriteLine, 1);
			debugClass.AddMethod("Write", Write, 1);
			debugClass.AddMethod("Log", Log, 1);
			debugClass.AddMethod("Trace", Trace, 1);
			debugClass.AddMethod("Warn", Warn, 1);
			debugClass.AddMethod("Error", Error, 1);

			VirtualMachine.AddClass("Debug", debugClass);
		}
	}
}
