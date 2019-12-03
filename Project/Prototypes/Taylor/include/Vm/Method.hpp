/*
 *	Method.hpp
 *	VM Method
 */

#ifndef __VIRTUAL_MACHINE_METHOD_HPP
#define __VIRTUAL_MACHINE_METHOD_HPP

#include <vector>
#include <Vm/Variant.hpp>
#include <Vm/Instruction.hpp>
#include <Vm/Stack.hpp>

namespace Vm
{
	class Class;

	class Method
	{
	public:
		typedef void (* HookedMethod)(Util::Stack<Variant*>& stack, Class* parent);

		enum class Type
		{
			Illegal,
			Hooked,
			Bytecode
		};

	public:
		Class* parent;

	private:
		std::string name;
		size_t argumentCount;
		std::vector<Instruction> instructions;
		HookedMethod method;
		Type type;

		friend class Class;

	public:
		Method();
		Method(const std::vector<Instruction>& instructions, size_t argumentCount);
		Method(HookedMethod method, size_t argumentCount);

		void Execute(Util::Stack<Variant*>& stack, Class* parent);

		const std::string& Name() const;
		size_t ArgumentCount() const;
		const std::vector<Instruction>& Instructions() const;
		HookedMethod ExecutePointer() const;
		Type Type() const { return type; } // Needs to be defined here...
		size_t Size() const;

		const Instruction& operator[](size_t i) const;
	};
}

#endif
