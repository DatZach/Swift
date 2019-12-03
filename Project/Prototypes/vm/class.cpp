/*
 *	class.cpp
 *	Virtual Machine Class
 */

#include <class.hpp>

namespace Vm
{
	Class::Class()
		: name(""),
		  methods(),
		  fields()
	{

	}

	Class::Class(const Class& _class)
		: name(_class.name),
		  methods(_class.methods),
		  fields(_class.fields)
	{

	}

	void Class::SetName(const std::string& value)
	{
		if (name != "")
			return;

		name = value;
	}

	void Class::AddMethod(const Method& method)
	{
		methods.push_back(method);
		methods.back().parent = this;
	}

	void Class::AddField(const Field& field)
	{
		fields.push_back(field);
	}

	const std::string& Class::GetName() const
	{
		return name;
	}

	const std::vector<Method>& Class::GetMethods() const
	{
		return methods;
	}

	std::vector<Field>& Class::GetFields() const
	{
		return const_cast<std::vector<Field>&>(fields);
	}

	Method* Class::GetMethod(const std::string& value) const
	{
		for(size_t i = 0; i < methods.size(); ++i)
		{
			if (methods[i].GetName() == value)
				return (Method*)&methods[i];
		}

		return NULL;
	}

	Field* Class::GetField(const std::string& value) const
	{
		for(auto& field : fields)
		{
			if (field.name == value)
				return const_cast<Field*>(&field);
		}

		return nullptr;
	}
}
