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
		  methods(),
		  co(nullptr)
	{

	}

	Class::Class(const Class& other)
		: name(other.name),
		  fields(other.fields),
		  methods(other.methods),
		  co(nullptr)
	{
		for(auto& method : methods)
			method.method.parent = this;
	}

	void Class::AddMethod(const std::string& name, const Method& method)
	{
		methods.push_back(MethodHead(name, method.ArgumentCount(), method));
		methods.back().method.name = name;
		methods.back().method.parent = this;
	}

	void Class::AddMethod(std::string const& name, Method::HookedMethod method, size_t argumentCount)
	{
		AddMethod(name, Method(method, argumentCount));
	}

	void Class::AddMethod(std::string const& name, std::vector<Instruction> const& instructions, size_t argumentCount)
	{
		AddMethod(name, Method(instructions, argumentCount));
	}

	Variant& Class::Field(std::string const& name)
	{
		return fields[name];
	}

	Variant& Class::operator[](const std::string& name)
	{
		return fields[name];
	}

	Method Class::GetMethod(const std::string& name, size_t argumentCount)
	{
		for(MethodHead& method : methods)
		{
			if (method.argumentCount == argumentCount && method.name == name)
				return method.method;
		}

		return Method();
	}

	Method Class::GetMethod(const std::string& name)
	{
		for(MethodHead& method : methods)
		{
			if (method.name == name)
				return method.method;
		}

		return Method();
	}

	std::string const& Class::Name() const
	{
		return name;
	}
}
