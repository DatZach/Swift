/*
 *	variant.cpp
 *	Virtual Machine Variant Variable
 */

#include <variant.hpp>

namespace Vm
{
	Variant::Variant()
		: type(Nil),
		  intValue(0),
		  doubleValue(0.0),
		  stringValue(""),
		  objectValue(NULL),
		  listValue()
	{

	}

	Variant::Variant(long long value)
		: type(Integer),
		  intValue(value),
		  doubleValue(0.0),
		  stringValue(""),
		  objectValue(NULL),
		  listValue()
	{

	}

	Variant::Variant(double value)
		: type(Double),
		  intValue(0),
		  doubleValue(value),
		  stringValue(""),
		  objectValue(NULL),
		  listValue()
	{

	}

	Variant::Variant(const std::string& value)
		: type(String),
		  intValue(0),
		  doubleValue(0.0),
		  stringValue(value),
		  objectValue(NULL),
		  listValue()
	{

	}

	Variant::Variant(const char* value)
		: type(String),
		  intValue(0),
		  doubleValue(0.0),
		  stringValue(value),
		  objectValue(NULL),
		  listValue()
	{

	}

	Variant::Variant(void* value)
		: type(Object),
		  intValue(0),
		  doubleValue(0.0),
		  stringValue(""),
		  objectValue(value),
		  listValue()
	{

	}

	Variant::Variant(const std::vector<Variant>& value)
		: type(List),
		  intValue(0),
		  doubleValue(0.0),
		  stringValue(""),
		  objectValue(NULL),
		  listValue(value)
	{

	}

	Variant Variant::Cast(VariantType toType) const
	{
		switch(type)
		{
			case Integer:
				if (toType == String)
				{
					std::stringstream ss;
					ss << intValue;

					return Variant(ss.str());
				}

				if (toType == Double)
					return Variant(static_cast<double>(intValue));
					
				break;

			case Double:
				if (toType == String)
				{
					std::stringstream ss;
					ss << doubleValue;

					return Variant(ss.str());
				}

				if (toType == Double)
					return Variant(static_cast<long long>(intValue));
					
				break;

			case String:
				if (toType == Integer)
				{
					long long value;
					std::stringstream(stringValue) >> value;

					return Variant(value);
				}

				if (toType == Double)
				{
					double value;
					std::stringstream(stringValue) >> value;

					return Variant(value);
				}

				break;

			case List:
				// TODO Implement
				break;
		}

		std::stringstream ess;
		ess << "Cannot cast type \""
			<< TypeName(type)
			<< "\" to type \""
			<< TypeName(toType)
			<< "\"";

		throw VariantException(ess.str());

		return Variant();
	}

	size_t Variant::GetLength() const
	{
		switch(type)
		{
			case Integer:
			case Double:
				return 8;

			case String:
				return stringValue.length();

			case List:
				return listValue.size();
		}

		return 0;
	}

	const char* Variant::TypeName(VariantType type)
	{
		const char* typeNameTable[6] =
		{
			"Nil", "Integer", "Double", "String", "List", "Object"
		};

		unsigned int i = static_cast<unsigned int>(type);
		return (i > 6) ? "" : typeNameTable[i];
	}

	VariantException::VariantException(const std::string& _message)
		: message(_message)
	{

	}

	const char* VariantException::what() const throw()
	{
		return message.c_str();
	}
}
