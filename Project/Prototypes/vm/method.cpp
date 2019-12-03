/*
 *	method.cpp
 *	Virtual Machine Method
 */

#include <method.hpp>

namespace Vm
{
	Method::Method(const std::string& _name, const std::vector<Instruction>& _instructions, size_t _argumentCount)
		: name(_name),
		  type(Bytecode),
		  instructions(_instructions),
		  argumentCount(_argumentCount),
		  parent(NULL),
		  Execute(NULL)
	{

	}

	Method::Method(const std::string& _name, void (* _Execute)(Util::Stack<Variant>& stack, Class* parent), size_t _argumentCount)
		: name(_name),
		  type(Hooked),
		  instructions(),
		  argumentCount(_argumentCount),
		  parent(NULL),
		  Execute(_Execute)
	{

	}

	const std::string& Method::GetName() const
	{
		return name;
	}

	Method::MethodType Method::GetMethodType() const
	{
		return type;
	}

	const std::vector<Instruction>& Method::GetInstructions() const
	{
		return instructions;
	}

	size_t Method::GetArgumentCount() const
	{
		return argumentCount;
	}
}
