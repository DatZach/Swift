/*
 *	Class.hpp
 *	VM Class
 */

#ifndef __VIRTUAL_MACHINE_CLASS_HPP
#define __VIRTUAL_MACHINE_CLASS_HPP

#include <unordered_map>
#include <map>
#include <string>
#include <Vm/Method.hpp>
#include <Vm/Variant.hpp>

#define VM_CLASS_CTOR_NAME				"__Constructor"
#define VM_CLASS_DTOR_NAME				"__Destructor"

namespace std
{
	// Declare hash functor for methods map
	template<>
	struct hash<pair<string, size_t>>
	{
		size_t operator()(const pair<string, size_t>& x) const
		{
			return (hash<const char*>()(x.first.c_str()) << 4) | (x.second & 0x0F);
		}
	};
}

namespace Vm
{
	class VirtualMachineSingleton;

	class Class
	{
	private:
		std::string name;
		std::unordered_map<std::string, Variant> fields;
		std::unordered_map<std::pair<std::string, size_t>, Method> methods;

		friend class VirtualMachineSingleton;

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
		Method& Method(const std::string& name, size_t argumentCount);

		// Metadata
		const std::string& Name() const;
	};
}

#endif
