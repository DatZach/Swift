/*
 *	Variant.cpp
 *	VM Variant type
 */

#include <iostream>
#include <sstream>
#include <Vm/VirtualMachine.hpp>
#include <Vm/Variant.hpp>

namespace Vm
{
	Variant::Variant()
		: type(Type::Nil),
		  marked(false),
		  next(nullptr)
	{

	}

	Variant::Variant(int value)
		: type(Type::Integer),
		  integerValue(value),
		  next(nullptr),
		  marked(false)
	{

	}

	Variant::Variant(int64_t value)
		: type(Type::Integer),
		  integerValue(value),
		  next(nullptr),
		  marked(false)
	{

	}

	Variant::Variant(double value)
		: type(Type::Double),
		  doubleValue(value),
		  next(nullptr),
		  marked(false)
	{

	}

	Variant::Variant(char const* value)
		: type(Type::String),
		  stringValue(new std::string(value)),
		  next(nullptr),
		  marked(false)
	{

	}

	Variant::Variant(const std::string& value)
		: type(Type::String),
		  stringValue(new std::string(value)),
		  next(nullptr),
		  marked(false)
	{

	}

	Variant::Variant(const std::vector<Variant*>& value)
		: type(Type::List),
		  listValue(std::shared_ptr<std::vector<Variant*>>(
						new std::vector<Variant*>(value),
						[](std::vector<Variant*>* parent)
						{
							// We don't have the GC manage any variants in the list
							//	only the list variant itself, so the parent list must
							//	free children elements on its own
							for(Variant* value : *parent)
								delete value;

							delete parent;
						},
						std::allocator<std::vector<Variant*>*>())),
		  next(nullptr),
		  marked(false)
	{

	}

	Variant::Variant(nullptr_t)
		: type(Type::Object),
		  objectValueUnmanaged(nullptr),
		  objectValueManaged(),
		  next(nullptr),
		  marked(false)
	{
		
	}

	Variant::Variant(Class* value, bool manage)
		: type(Type::Object),
		  objectValueUnmanaged(nullptr),
		  next(nullptr),
		  marked(false)
	{
		/*
			If class has a co object we know for a fact that the Engine is managing
			that object outside of the VM. If an object is being managed outside of
			the VM we CAN NOT manage it with a smart pointer because we will be leaking
			a reference.

			If value->co
				DO NOT MANAGE POINTER
			else
				MANAGE POINTER VIA SHARED_POINTER
		 */

		if (value->co != nullptr || !manage)
			objectValueUnmanaged = value;
		else
			objectValueManaged = std::shared_ptr<Class>(value,
				[](Class* parent)
				{
					// Call destructor if one exists
					Util::Stack<Variant*> stack;
					parent->GetMethod(VM_CLASS_DTOR_NAME, 0).Execute(stack, parent);

					// Delete class
					delete parent;
				}, std::allocator<Class*>());
	}

	Variant::Variant(void* value, void (*del)(void*))
		: type(Type::EngineObject),
		  engineObjectValue(value),
		  next(nullptr),
		  marked(false)
	{
		
	}

	void Variant::Mark()
	{
		marked = true;
	}

	void Variant::Unmark()
	{
		marked = false;
	}

	Variant Variant::Cast(Type toType) const
	{
		if (type == toType)
			return *this;

		switch(type)
		{
			case Type::Nil:
				// Nil -> String
				if (toType == Type::String)
					return "nil";
				break;

			case Type::Integer:
				// Int -> Double
				if (toType == Type::Double)
					return static_cast<double>(integerValue);

				// Int -> String
				if (toType == Type::String)
				{
					std::stringstream ss;
					ss << integerValue;

					return ss.str();
				}
				break;

			case Type::Double:
				// Double -> Int
				if (toType == Type::Integer)
					return static_cast<int64_t>(doubleValue);

				// Double -> String
				if (toType == Type::String)
				{
					std::stringstream ss;
					ss << std::fixed << doubleValue;

					return ss.str();
				}
				break;

			case Type::String:
				// String -> Int
				if (toType == Type::Integer)
				{
					int64_t value;
					std::stringstream(*stringValue) >> value;

					return value;
				}

				// String -> Double
				if (toType == Type::Double)
				{
					double value;
					std::stringstream(*stringValue) >> value;

					return value;
				}
				break;

			case Type::List:
				// List -> String
				if (toType == Type::String)
				{
					std::stringstream ss;
					ss << "[ ";

					// Iterate over the list
					size_t count = listValue->size();
					for(size_t i = 0; i < count; ++i)
					{
						// Grab item from list
						Variant* x = listValue->at(i);

						// Add it to string representation
						if (x->TypeOf() == Type::String)
							ss << '\"' << x->String() << '\"';
						else if (x == this)
							ss << "<...>";
						else
							ss << x->Cast(Type::String).String();

						// Add comma if we're not at the end of the list
						if (i != count - 1)
							ss << ", ";
					}

					ss << " ]";

					return ss.str();
				}
				break;

			case Type::Object:
				// Object -> String
				if (toType == Type::String)
					return "object";

			case Type::EngineObject:
				// EngineObject -> String
				if (toType == Type::String)
					return "<engine-object>";
				break;
		}

		// Illegal cast
		std::stringstream ess;
		ess << "Cannot cast type '"
			<< TypeName(type)
			<< "' to type '"
			<< TypeName(toType)
			<< "'.";

		throw Exception(ess.str());
	}

	Variant*& Variant::ListAt(int i) const
	{
		// Check if this is an illegal operation
		if (type != Type::List)
		{
			std::stringstream ess;
			ess << "Cannot index type '"
				<< TypeName(type)
				<< "'.";

			throw Exception(ess.str());
		}

		// Verify we're in range
		int listSize = static_cast<int>(listValue->size());
		if (!(i < listSize  && i >= -listSize))
		{
			std::stringstream ess;
			ess << "Index " << i << " out of range.";
			
			throw Exception(ess.str());
		}

		// Return variant in list
		return listValue->at(i >= 0 ? i : listSize + i);
	}

	char& Variant::StringAt(int i) const
	{
		// Check if this is an illegal operation
		if (type != Type::String)
		{
			std::stringstream ess;
			ess << "Cannot index type '"
				<< TypeName(type)
				<< "'.";

			throw Exception(ess.str());
		}

		// Verify we're in range
		int stringSize = static_cast<int>(stringValue->size());
		if (!(i < stringSize && i >= -stringSize))
		{
			std::stringstream ess;
			ess << "Index " << i << " out of range.";

			throw Exception(ess.str());
		}

		return stringValue->at(i >= 0 ? i : stringSize + i);
	}

	Variant::Type Variant::TypeOf() const
	{
		return type;
	}

	int64_t Variant::Integer() const
	{
		return integerValue;
	}

	double Variant::Double() const
	{
		return doubleValue;
	}

	double Variant::Number() const
	{
		return type == Type::Double ? doubleValue : static_cast<double>(integerValue);
	}

	bool Variant::Boolean() const
	{
		return static_cast<int>(Number()) != 0;
	}

	std::string& Variant::String() const
	{
		return *stringValue;
	}

	std::vector<Variant*>& Variant::List() const
	{
		return *listValue;
	}

	Class* Variant::Object() const
	{
		return TypeOf() == Type::Object
				? objectValueUnmanaged == nullptr
					? objectValueManaged.get()
					: objectValueUnmanaged
				: nullptr;
	}

	Variant::operator int64_t() const
	{
		return Integer();
	}

	Variant::operator unsigned int() const
	{
		return static_cast<unsigned int>(Integer());
	}

	Variant::operator int() const
	{
		return static_cast<int>(Integer());
	}

	Variant::operator float() const
	{
		return static_cast<float>(Double());
	}

	Variant::operator double() const
	{
		return Double();
	}

	Variant::operator bool() const
	{
		return Boolean();
	}

	Variant::operator std::string&() const
	{
		return String();
	}

	Variant::operator std::vector<Variant*>&() const
	{
		return List();
	}

	Variant::operator Class*() const
	{
		return Object();
	}

	Variant*& Variant::Next()
	{
		return next;
	}

	bool Variant::Marked() const
	{
		return marked;
	}

	int64_t Variant::Length() const
	{
		// Return the number of elements in this type
		switch(type)
		{
			case Type::Integer:
			case Type::Double:
			case Type::Object:
			case Type::Nil:
			case Type::EngineObject:
				return 1;

			case Type::String:
				return String().length();
			
			case Type::List:
				return List().size();
		}

		return 0;
	}

	Variant*& Variant::operator[](int i) const
	{
		return ListAt(i);
	}

	char const* Variant::TypeName(Type type)
	{
		const char* typeNameTable[6] =
		{
			"nil", "int", "double", "str", "list", "object"
		};

		unsigned int i = static_cast<unsigned int>(type);
		return i > sizeof(typeNameTable)
				? "<Illegal>"
				: typeNameTable[i];
	}

	Variant::Exception::Exception(const std::string& message)
		: message(message)
	{

	}

	Variant::Exception::~Exception() throw()
	{
		// This is here to prevent GCC from throwing up
	}

	const char* Variant::Exception::what() const throw()
	{
		return message.c_str();
	}
}
