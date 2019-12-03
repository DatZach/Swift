/*
 *	field.hpp
 *	Virtual Machine Field
 */

#ifndef __VIRTUAL_MACHINE_FIELD_HPP
#define __VIRTUAL_MACHINE_FIELD_HPP

#include <string>
#include <variant.hpp>

namespace Vm
{
	class Field
	{
	public:
		std::string name;
		Variant value;

	public:
		Field(const std::string& _name, const Variant& _value);
	};
}

#endif
