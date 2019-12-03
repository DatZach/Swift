/*
 *	variant.hpp
 *	Virtual Machine Variant Variable
 */

#ifndef __VIRTUAL_MACHINE_VARIANT_HPP
#define __VIRTUAL_MACHINE_VARIANT_HPP

#include <exception>
#include <sstream>
#include <string>
#include <vector>

namespace Vm
{
	class Variant
	{
	public:
		typedef enum _VariantType
		{
			Nil,
			Integer,
			Double,
			String,
			List,
			Object
		} VariantType;

	public:
		VariantType				type;
		long long				intValue;
		double					doubleValue;
		std::string				stringValue;
		void*					objectValue;
		std::vector<Variant>	listValue;

	public:
		Variant();
		explicit Variant(long long value);
		explicit Variant(double value);
		explicit Variant(const std::string& value);
		explicit Variant(const char* value);
		explicit Variant(void* value);
		explicit Variant(const std::vector<Variant>& value);

		Variant Cast(VariantType toType) const;
		size_t GetLength() const;

		static const char* TypeName(VariantType type);
	};

	class VariantException : public std::exception
	{
	private:
		std::string message;

	public:
		VariantException(const std::string& _message);

		const char* what() const throw();
	};

	// Unary operators
	Variant operator -(const Variant& right);
	Variant operator ~(const Variant& right);
	Variant operator !(const Variant& right);

	// Arithmetic
	Variant operator +(const Variant& left, const Variant& right);
	Variant operator -(const Variant& left, const Variant& right);
	Variant operator *(const Variant& left, const Variant& right);
	Variant operator /(const Variant& left, const Variant& right);
	Variant operator %(const Variant& left, const Variant& right);

	// Binary operators
	Variant operator &(const Variant& left, const Variant& right);
	Variant operator |(const Variant& left, const Variant& right);
	Variant operator ^(const Variant& left, const Variant& right);
	Variant operator >>(const Variant& left, const Variant& right);
	Variant operator <<(const Variant& left, const Variant& right);

	// Comparison operators
	Variant operator <(const Variant& left, const Variant& right);
	Variant operator >(const Variant& left, const Variant& right);
	Variant operator <=(const Variant& left, const Variant& right);
	Variant operator >=(const Variant& left, const Variant& right);
	Variant operator ==(const Variant& left, const Variant& right);
	Variant operator !=(const Variant& left, const Variant& right);

#include <variant.inl>
}

#endif
