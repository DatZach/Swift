/*
 *	Class.hpp
 *	VM Class
 */

#ifndef __VIRTUAL_MACHINE_CLASS_HPP
#define __VIRTUAL_MACHINE_CLASS_HPP

#include <unordered_map>
#include <string>
#include <Vm/Method.hpp>
#include <Vm/Variant.hpp>

#define VM_CLASS_CTOR_NAME				"__Constructor"
#define VM_CLASS_DTOR_NAME				"__Destructor"

namespace Vm
{
	class VirtualMachineSingleton;

	class Class
	{
	private:
		class MethodHead
		{
		public:
			std::string		name;
			size_t			argumentCount;
			Method			method;

		public:
			MethodHead(const std::string& name, size_t argumentCount, const Method& method)
				: name(name),
				  argumentCount(argumentCount),
				  method(method)
			{
				
			}
		};

	private:
		std::string name;
		std::unordered_map<std::string, Variant> fields;
		std::vector<MethodHead> methods;

		friend class VirtualMachineSingleton;

	public:
		void* co;

	public:
		Class();
		Class(const Class& other);

		// Methods
		void AddMethod(const std::string& name, const Method& method);
		void AddMethod(const std::string& name, Method::HookedMethod method, size_t argumentCount);
		void AddMethod(const std::string& name, const std::vector<Instruction>& instructions, size_t argumentCount);

		// Fields
		Variant& Field(const std::string& name);
		Variant& operator[](const std::string& name);

		// Methods
		Method GetMethod(const std::string& name, size_t argumentCount);
		Method GetMethod(const std::string& name);

		// Metadata
		template <typename T>
		T* GetCo()
		{
			return reinterpret_cast<T*>(co);
		}

		const std::string& Name() const;
	};
}

#endif
