/*
 *	Class.cpp
 *	VM Class
 */

#include <Vm/Class.hpp>

namespace Vm
{
	Class::Class()
		: name(""),
		  fields(),
		  methods()
	{

	}

	Class::Class(const Class& other)
		: name(other.name),
		  fields(other.fields),
		  methods(other.methods)
	{
		for(auto& method : methods)
			method.second.parent = this;
	}

	void Class::AddMethod(const std::string& name, const Vm::Method& method)
	{
		std::pair<std::string, size_t> key(name, method.ArgumentCount());

		methods[key] = method;
		methods[key].name = name;
		methods[key].parent = this;
	}

	void Class::AddMethod(std::string const& name, Method::HookedMethod method, size_t argumentCount)
	{
		AddMethod(name, Vm::Method(method, argumentCount));
	}

	void Class::AddMethod(std::string const& name, std::vector<Instruction> const& instructions, size_t argumentCount)
	{
		AddMethod(name, Vm::Method(instructions, argumentCount));
	}

	Variant& Class::Field(std::string const& name)
	{
		return fields[name];
	}

	Variant& Class::operator[](const std::string& name)
	{
		return fields[name];
	}

	Method& Class::Method(const std::string& name, size_t argumentCount)
	{
		return methods[std::pair<std::string, size_t>(name, argumentCount)];
	}

	std::string const& Class::Name() const
	{
		return name;
	}
}
