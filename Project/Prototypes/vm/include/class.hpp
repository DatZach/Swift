/*
 *	class.hpp
 *	Virtual Machine Class
 *
 *	Note:
 *		Class doesn't accept all of its data in the ctor.
 *		This is because of the way it's loaded from the WAD.
 */

#ifndef __VIRTUAL_MACHINE_CLASS_HPP
#define __VIRTUAL_MACHINE_CLASS_HPP

#include <vector>
#include <string>
#include <method.hpp>
#include <field.hpp>

namespace Vm
{
	class Class
	{
	private:
		std::string name;
		std::vector<Method> methods;
		std::vector<Field> fields;

	public:
		Class();
		Class(const Class& _class);

		void SetName(const std::string& value);
		void AddMethod(const Method& method);
		void AddField(const Field& field);

		const std::string& GetName() const;
		const std::vector<Method>& GetMethods() const;
		std::vector<Field>& GetFields() const;
		Method* GetMethod(const std::string& value) const;
		Field* GetField(const std::string& value) const;
	};
}

#endif
