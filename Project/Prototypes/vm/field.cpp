/*
 *	field.cpp
 *	Virtual Machine Field
 */

#include <field.hpp>

namespace Vm
{
	Field::Field(const std::string& _name, const Variant& _value)
		: name(_name),
		  value(_value)
	{

	}
}
