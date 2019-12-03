/*
 *	Method.cpp
 *	VM Method
 */

#include <Vm/VirtualMachine.hpp>
#include <Vm/Method.hpp>

namespace Vm
{
	// This is to get around the STL map needing a default ctor when indexing
	Method::Method()
		: type(Type::Illegal),
		  instructions(),
		  method(nullptr),
		  parent(nullptr)
	{

	}

	Method::Method(const std::vector<Instruction>& instructions, size_t argumentCount)
		: type(Type::Bytecode),
		  instructions(instructions),
		  argumentCount(argumentCount),
		  method(nullptr),
		  parent(nullptr)
	{

	}

	Method::Method(HookedMethod method, size_t argumentCount)
		: type(Type::Hooked),
		  instructions(),
		  method(method),
		  argumentCount(argumentCount),
		  parent(nullptr)
	{

	}

	void Method::Execute(Util::Stack<Variant*>& stack, Class* parent)
	{
		switch(type)
		{
			case Type::Bytecode:
				VirtualMachine.ExecuteBytecode(*this, stack);
				break;

			case Type::Hooked:
				method(stack, parent);

				// Push nil if the method returned nothing
				if (stack.Count() == 0)
					stack.Push(VirtualMachine.CreateVariant(Variant()));
				break;

			default:
				throw VirtualMachineSingleton::Exception("Cannot execute this method.");
		}
	}

	std::string const& Method::Name() const
	{
		return name;
	}

	size_t Method::ArgumentCount() const
	{
		return argumentCount;
	}

	std::vector<Instruction> const& Method::Instructions() const
	{
		return instructions;
	}

	Method::HookedMethod Method::ExecutePointer() const
	{
		return method;
	}

	size_t Method::Size() const
	{
		return instructions.size();
	}

	Instruction const& Method::operator[](size_t i) const
	{
		return instructions[i];
	}
}
