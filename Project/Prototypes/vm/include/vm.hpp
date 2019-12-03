/*
 *	vm.hpp
 *	Virtual Machine
 */

#ifndef __VIRTUAL_MACHINE_HPP
#define __VIRTUAL_MACHINE_HPP

#include <vector>
#include <class.hpp>
#include <variant.hpp>
#include <stack.hpp>

#define VirtualMachine		Vm::VirtualMachineSingleton::GetInstance()

namespace Vm
{
	class VirtualMachineSingleton
	{
	private:
		std::vector<Class> classes;

	public:
		void Execute(const Method& method, Util::Stack<Variant>& argumentStack);

		void AddClass(const Class& value);
		Class* GetClass(const std::string& value);

		static VirtualMachineSingleton& GetInstance();
	};
}

#endif
