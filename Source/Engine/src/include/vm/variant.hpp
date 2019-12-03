/*
 *	Variant.hpp
 *	VM Variant type
 */

#ifndef __VIRTUAL_MACHINE_VARIANT_HPP
#define __VIRTUAL_MACHINE_VARIANT_HPP

#include <memory>
#include <exception>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace Vm
{
	class Class;

	class Variant
	{
	public:
		class Exception : public std::exception
		{
		private:
			std::string message;

		public:
			Exception(const std::string& message);
			~Exception() throw() final;

			virtual const char* what() const throw() final;
		};

		enum class Type
		{
			Nil,
			Integer,
			Double,
			String,
			List,
			Object,
			EngineObject
		};

	private:
		struct
		{
			bool marked : 1;
			Type type	: 4;
		};

		Variant* next;

		union
		{
			int64_t					integerValue;
			double					doubleValue;
			Class*					objectValueUnmanaged;
			void*					engineObjectValue;
		};

		std::shared_ptr<std::vector<Variant*>> listValue;
		std::shared_ptr<std::string> stringValue;
		std::shared_ptr<Class> objectValueManaged;

	public:
		Variant();
		Variant(int value);
		Variant(int64_t value);
		Variant(double value);
		Variant(const char* value);
		Variant(const std::string& value);
		Variant(const std::vector<Variant*>& value);
		Variant(Class* value, bool manage);
		Variant(nullptr_t);
		Variant(void* value, void (*del)(void*));

		// Garbage Collector
		void Mark();
		void Unmark();

		Variant Cast(Type toType) const;
		Variant*& ListAt(int i) const;
		char& StringAt(int i) const;

		// Explicit values
		Type TypeOf() const;
		int64_t Integer() const;
		double Double() const;
		double Number() const;
		bool Boolean() const;
		std::string& String() const;
		std::vector<Variant*>& List() const;
		Class* Object() const;
		template <class T>
		T* EngineObject() const { return static_cast<T*>(TypeOf() == Type::EngineObject ? engineObjectValue : nullptr); }

		void SetEngineObject(void* value)
		{
			type = Type::EngineObject;
			engineObjectValue = value;
		}

		// Implicit casting
		operator int64_t() const;
		operator unsigned int() const;
		operator int() const;
		operator float() const;
		operator double() const;
		operator bool() const;
		operator std::string&() const;
		operator std::vector<Variant*>&() const;
		operator Class*() const;

		// Metadata
		Variant*& Next();
		bool Marked() const;
		int64_t Length() const;

		Variant*& operator[](int i) const;

		static const char* TypeName(Type type);
	};

	// Unary operators
	Variant operator -(const Variant& right);
	Variant operator ~(const Variant& right);
	Variant operator !(const Variant& right);

	// Arithmetic operators
	Variant operator +(const Variant& left, const Variant& right);
	Variant operator -(const Variant& left, const Variant& right);
	Variant operator *(const Variant& left, const Variant& right);
	Variant operator /(const Variant& left, const Variant& right);
	Variant operator %(const Variant& left, const Variant& right);

	// Bitwise operators
	Variant operator &(const Variant& left, const Variant& right);
	Variant operator |(const Variant& left, const Variant& right);
	Variant operator ^(const Variant& left, const Variant& right);
	Variant operator >>(const Variant& left, const Variant& right);
	Variant operator <<(const Variant& left, const Variant& right);

	// Comparison operators
	bool operator <(const Variant& left, const Variant& right);
	bool operator >(const Variant& left, const Variant& right);
	bool operator <=(const Variant& left, const Variant& right);
	bool operator >=(const Variant& left, const Variant& right);
	bool operator ==(const Variant& left, const Variant& right);
	bool operator !=(const Variant& left, const Variant& right);

	// Unary operators
	inline Variant operator +(const Variant& left, const Variant& right)
	{
		switch(left.TypeOf())
		{
			case Variant::Type::Integer:
				// Int + Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Integer() + right.Integer());

				// Int + Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Integer() + right.Double());
				break;

			case Variant::Type::Double:
				// Double + Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Double() + right.Integer());

				// Double + Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Double() + right.Double());
				break;

			case Variant::Type::String:
				// String + String
				if (right.TypeOf() == Variant::Type::String)
					return Variant(left.String() + right.String());
				break;

			case Variant::Type::List:
				// NOTE We do not manage children variants in lists, only the list variant themselves
				left.List().push_back(new Variant(right));
				return left;
		}

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot + variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator -(const Variant& right)
	{
		// -Int
		if (right.TypeOf() == Variant::Type::Integer)
			return Variant(-right.Integer());

		// -Double
		if (right.TypeOf() == Variant::Type::Double)
			return Variant(-right.Double());

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot unary - variant type '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator ~(const Variant& right)
	{
		// ~Int
		if (right.TypeOf() == Variant::Type::Integer)
			return Variant(~right.Integer());

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot unary ~ variant type '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator !(const Variant& right)
	{
		// !Int
		if (right.TypeOf() == Variant::Type::Integer)
			return Variant(!right.Integer());

		
		// Illegal operation
		std::stringstream ess;
		ess << "Cannot unary ! variant type '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	// Arithmetic operators
	inline Variant operator -(const Variant& left, const Variant& right)
	{
		switch(left.TypeOf())
		{
			case Variant::Type::Integer:
				// Int - Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Integer() - right.Integer());

				// Int - Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Integer() - right.Double());
				break;

			case Variant::Type::Double:
				// Double - Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Double() - right.Integer());

				// Double - Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Double() - right.Double());
				break;
		}

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot - variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator *(const Variant& left, const Variant& right)
	{
		switch(left.TypeOf())
		{
			case Variant::Type::Integer:
				// Int * Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Integer() * right.Integer());

				// Int * Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Integer() * right.Double());
				break;

			case Variant::Type::Double:
				// Double * Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Double() * right.Integer());

				// Double * Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Double() * right.Double());
				break;

			case Variant::Type::String:
				// String * Int
				if (right.TypeOf() == Variant::Type::Integer)
				{
					std::stringstream ss;
					
					int64_t count = right.Integer();
					while(count--)
						ss << left.String();

					return Variant(ss.str());
				}
				break;
		}

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot * variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator /(const Variant& left, const Variant& right)
	{
		switch(left.TypeOf())
		{
			case Variant::Type::Integer:
				// Int / Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Integer() / right.Integer());

				// Int / Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Integer() / right.Double());
				break;

			case Variant::Type::Double:
				// Double / Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Double() / right.Integer());

				// Double / Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(left.Double() / right.Double());
				break;
		}

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot / variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator %(const Variant& left, const Variant& right)
	{
		switch(left.TypeOf())
		{
			case Variant::Type::Integer:
				// Int % Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(left.Integer() % right.Integer());

				// Int % Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(fmod(left.Integer(), right.Double()));
				break;

			case Variant::Type::Double:
				// Double % Int
				if (right.TypeOf() == Variant::Type::Integer)
					return Variant(fmod(left.Double(), right.Integer()));

				// Double % Double
				if (right.TypeOf() == Variant::Type::Double)
					return Variant(fmod(left.Double(), right.Double()));
				break;
		}

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot % variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	// Bitwise operators
	inline Variant operator &(const Variant& left, const Variant& right)
	{
		// Int & Int
		if (left.TypeOf() == Variant::Type::Integer && right.TypeOf() == Variant::Type::Integer)
			return Variant(left.Integer() & right.Integer());

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot & variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator |(const Variant& left, const Variant& right)
	{
		// Int | Int
		if (left.TypeOf() == Variant::Type::Integer && right.TypeOf() == Variant::Type::Integer)
			return Variant(left.Integer() | right.Integer());

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot | variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator ^(const Variant& left, const Variant& right)
	{
		// Int ^ Int
		if (left.TypeOf() == Variant::Type::Integer && right.TypeOf() == Variant::Type::Integer)
			return Variant(left.Integer() ^ right.Integer());

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot ^ variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator >>(const Variant& left, const Variant& right)
	{
		// Int >> Int
		if (left.TypeOf() == Variant::Type::Integer && right.TypeOf() == Variant::Type::Integer)
			return Variant(left.Integer() >> right.Integer());

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot >> variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline Variant operator <<(const Variant& left, const Variant& right)
	{
		// Int << Int
		if (left.TypeOf() == Variant::Type::Integer && right.TypeOf() == Variant::Type::Integer)
			return Variant(left.Integer() << right.Integer());

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot << variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' and '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	// Comparison operators
	inline bool operator <(const Variant& left, const Variant& right)
	{
		switch(left.TypeOf())
		{
			case Variant::Type::Integer:
				// Int < Int
				if (right.TypeOf() == Variant::Type::Integer)
					return left.Integer() < right.Integer();

				// Int < Double
				if (right.TypeOf() == Variant::Type::Double)
					return left.Integer() < right.Double();
				break;

			case Variant::Type::Double:
				// Double < Int
				if (right.TypeOf() == Variant::Type::Integer)
					return left.Double() < right.Integer();

				// Double < Double
				if (right.TypeOf() == Variant::Type::Double)
					return left.Double() < right.Double();
				break;

			case Variant::Type::String:
				// String < String
				if (right.TypeOf() == Variant::Type::String)
				{
					const std::string& lString = left.String();
					const std::string& rString = right.String();

					return lexicographical_compare(lString.cbegin(), lString.cend(), rString.cbegin(), rString.cend(),
												   [=](char a, char b) { return a < b; });
				}
				break;
		}

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot compare variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' against type '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline bool operator >(const Variant& left, const Variant& right)
	{
		return operator <(right, left);
	}

	inline bool operator <=(const Variant& left, const Variant& right)
	{
		return !operator >(left, right);
	}

	inline bool operator >=(const Variant& left, const Variant& right)
	{
		return !operator <(left, right);
	}

	inline bool operator ==(const Variant& left, const Variant& right)
	{
		switch(left.TypeOf())
		{
			case Variant::Type::Nil:
				// Nil == Nil
				return right.TypeOf() == Variant::Type::Nil;

			case Variant::Type::Integer:
				// Int == Int
				if (right.TypeOf() == Variant::Type::Integer)
					return left.Integer() == right.Integer();

				// Int == Double
				if (right.TypeOf() == Variant::Type::Double)
					return left.Integer() == right.Double();
				break;

			case Variant::Type::Double:
				// Double == Int
				if (right.TypeOf() == Variant::Type::Integer)
					return left.Double() == right.Integer();

				// Double == Double
				if (right.TypeOf() == Variant::Type::Double)
					return fabs(left.Double() - right.Double()) < std::numeric_limits<double>::epsilon();
				break;

			case Variant::Type::String:
				// String == String
				if (right.TypeOf() == Variant::Type::String)
					return left.String() == right.String();
				break;

			case Variant::Type::List:
				// List == List
				if (right.TypeOf() == Variant::Type::List)
				{
					// Fetch lists
					const std::vector<Variant*>& leftList = left.List();
					const std::vector<Variant*>& rightList = right.List();

					// Compare sizes first of all
					if (leftList.size() != rightList.size())
						return false;

					// Compare each element
					size_t i = leftList.size();
					while(i--)
					{
						if (*leftList[i] != *rightList[i])
							return false;
					}

					return true;
				}
				break;

			case Variant::Type::Object:
				// Object == Object
				if (right.TypeOf() == Variant::Type::Object)
					return left.Object() == right.Object();
				break;
		}

		// Illegal operation
		std::stringstream ess;
		ess << "Cannot compare variant type '"
			<< Variant::TypeName(left.TypeOf())
			<< "' against type '"
			<< Variant::TypeName(right.TypeOf())
			<< "'.";

		throw Variant::Exception(ess.str());
	}

	inline bool operator !=(const Variant& left, const Variant& right)
	{
		return !operator ==(left, right);
	}
}

#endif
