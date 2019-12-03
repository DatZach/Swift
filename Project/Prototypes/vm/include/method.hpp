/*
 *	method.hpp
 *	Virtual Machine Method
 *
 *	Note:
 *		Methods can only accept data in the ctor.
 *		This is because of the way it's loaded from the WAD.
 */

#ifndef __VIRTUAL_MACHINE_METHOD_HPP
#define __VIRTUAL_MACHINE_METHOD_HPP

#include <string>
#include <vector>
#include <instruction.hpp>
#include <stack.hpp>

namespace Vm
{
	class Class;

	class Method
	{
	public:
		typedef enum _MethodType
		{
			Hooked,
			Bytecode
		} MethodType;

	private:
		std::string name;
		MethodType type;
		std::vector<Instruction> instructions;
		size_t argumentCount;

	public:
		Class* parent;
		void (* Execute)(Util::Stack<Variant>& stack, Class* parent);

	public:
		Method(const std::string& _name, const std::vector<Instruction>& _instructions, size_t _argumentCount);
		Method(const std::string& _name, void (* _Execute)(Util::Stack<Variant>& stack, Class* parent), size_t _argumentCount);

		const std::string& GetName() const;
		MethodType GetMethodType() const;
		const std::vector<Instruction>& GetInstructions() const;
		size_t GetArgumentCount() const;
	};
}

#endif
