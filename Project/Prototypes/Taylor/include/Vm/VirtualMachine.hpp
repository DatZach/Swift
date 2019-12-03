/*
 *	VirtualMachine.hpp
 *	Taylor Swift VM
 */

#ifndef __VIRTUAL_MACHINE_HPP
#define __VIRTUAL_MACHINE_HPP

#include <exception>
#include <unordered_map>
#include <string>
#include <Vm/Class.hpp>
#include <Vm/Stack.hpp>

#define VirtualMachine		Vm::VirtualMachineSingleton::GetInstance()
#define VmException			Vm::VirtualMachineSingleton::Exception

namespace Vm
{
	class VirtualMachineSingleton
	{
	private:
		static const int GC_MAX_OBJECT_COUNT = 8;

		class DebugScope
		{
		private:
			std::string className;
			std::string methodName;
			size_t ip;

		public:
			DebugScope();
			DebugScope(const std::string& className, const std::string& methodName, size_t ip);

			const std::string& ClassName() const;
			const std::string& MethodName() const;
			size_t Ip() const;
		};

	public:
		class Exception : public std::exception
		{
		private:
			std::string message;

		public:
			Exception(const std::string& message);
			~Exception() throw() final;

			virtual const char* what() const throw() final;
		};

	private:
		// Universe data
		std::unordered_map<std::string, Class> classes;
		Util::Stack<DebugScope> debugMethodStack;
		size_t ip;

		// Garbage Collector
		Variant* gcFirstObject;
		int gcObjectCount;
		int gcMaxObjectCount;

	private:
		void ExecuteBytecode(const Method& method, Util::Stack<Variant*>& argumentStack);
		std::string StackTrace();

		friend class Method;

	public:
		VirtualMachineSingleton();
		~VirtualMachineSingleton();

		// Execution
		void Execute(const Method& method, Util::Stack<Variant*>& argumentStack);
		void Execute(const Method& method);

		// Garbage Collector
		void CollectGarbage(Util::Stack<Variant*>& stack);
		Variant* CreateVariant(const Variant& value);

		// Class cache
		void AddClass(const std::string& name, Class& value);
		Class& operator[](const std::string& name);

		// Singleton instance
		static VirtualMachineSingleton& GetInstance();
	};
}

#endif
