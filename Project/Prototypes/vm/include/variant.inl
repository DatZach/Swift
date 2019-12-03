/*
 *	variant.inl
 *	Variant Inline Declarations
 */

inline Variant operator -(const Variant& right)
{
	if (right.type == Variant::Integer)
		return Variant(-right.intValue);

	if (right.type == Variant::Double)
		return Variant(-right.doubleValue);

	std::stringstream ss;
	ss << "Cannot unary - variant type \""
	   << Variant::TypeName(right.type)
	   << "\"";
	
	throw VariantException(ss.str());
}

inline Variant operator ~(const Variant& right)
{
	if (right.type == Variant::Integer)
		return Variant(~right.intValue);

	std::stringstream ss;
	ss << "Cannot unary ~ variant type \""
	   << Variant::TypeName(right.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator !(const Variant& right)
{
	if (right.type == Variant::Integer)
		return Variant(static_cast<long long>(!right.intValue));

	std::stringstream ss;
	ss << "Cannot unary ! variant type \""
	   << Variant::TypeName(right.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator +(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	switch(left.type)
	{
		case Variant::Integer:
			return Variant(left.intValue + right.intValue);

		case Variant::Double:
			return Variant(left.doubleValue + right.doubleValue);

		case Variant::String:
			return Variant(left.stringValue + right.stringValue);

		case Variant::List:
			// TODO Implement
			return Variant(left.listValue);
	}

	std::stringstream ss;
	ss << "Cannot + variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator -(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	switch(left.type)
	{
		case Variant::Integer:
			return Variant(left.intValue - right.intValue);

		case Variant::Double:
			return Variant(left.doubleValue - right.doubleValue);
	}

	std::stringstream ss;
	ss << "Cannot - variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator *(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	switch(left.type)
	{
		case Variant::Integer:
			return Variant(left.intValue * right.intValue);

		case Variant::Double:
			return Variant(left.doubleValue * right.doubleValue);
	}

	std::stringstream ss;
	ss << "Cannot * variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator /(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	switch(left.type)
	{
		case Variant::Integer:
			return Variant(left.intValue / right.intValue);

		case Variant::Double:
			return Variant(left.doubleValue / right.doubleValue);
	}

	std::stringstream ss;
	ss << "Cannot / variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator %(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	switch(left.type)
	{
		case Variant::Integer:
			return Variant(left.intValue % right.intValue);

		case Variant::Double:
			return Variant(std::fmod(left.doubleValue, right.doubleValue));
	}

	std::stringstream ss;
	ss << "Cannot % variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator &(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	if (left.type == Variant::Integer)
		return Variant(left.intValue & right.intValue);

	std::stringstream ss;
	ss << "Cannot & variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator |(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	if (left.type == Variant::Integer)
		return Variant(left.intValue | right.intValue);

	std::stringstream ss;
	ss << "Cannot | variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator ^(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	if (left.type == Variant::Integer)
		return Variant(left.intValue ^ right.intValue);

	std::stringstream ss;
	ss << "Cannot ^ variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator <<(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	if (left.type == Variant::Integer)
		return Variant(left.intValue << right.intValue);

	std::stringstream ss;
	ss << "Cannot << variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator >>(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	if (left.type == Variant::Integer)
		return Variant(left.intValue >> right.intValue);

	std::stringstream ss;
	ss << "Cannot >> variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator <(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	if (left.type == Variant::Integer)
		return Variant(static_cast<long long>(left.intValue < right.intValue));

	std::stringstream ss;
	ss << "Cannot compare variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator >(const Variant& left, const Variant& right)
{
	return operator <(left, right);
}

inline Variant operator <=(const Variant& left, const Variant& right)
{
	return !operator >(left, right);
}

inline Variant operator >=(const Variant& left, const Variant& right)
{
	return !operator <(left, right);
}

inline Variant operator ==(const Variant& left, const Variant& right)
{
	if (left.type != right.type)
	{
		std::stringstream ss;
		ss << "Variable types do not match ("
		   << Variant::TypeName(left.type)
		   << " != "
		   << Variant::TypeName(right.type)
		   << ")";

		throw VariantException(ss.str());
	}

	switch(left.type)
	{
		case Variant::Integer:
			return Variant(static_cast<long long>(left.intValue < right.intValue));

		case Variant::Double:
			return Variant(static_cast<long long>(std::fabs(left.doubleValue - right.doubleValue) < std::numeric_limits<double>::epsilon()));

		case Variant::String:
			return Variant(static_cast<long long>(left.stringValue == right.stringValue));

		case Variant::List:
		case Variant::Object:
			// TODO Implement
			return Variant(static_cast<long long>(false));
	}

	std::stringstream ss;
	ss << "Cannot compare variant type \""
	   << Variant::TypeName(left.type)
	   << "\"";

	throw VariantException(ss.str());
}

inline Variant operator !=(const Variant& left, const Variant& right)
{
	return !operator ==(left, right);
}
