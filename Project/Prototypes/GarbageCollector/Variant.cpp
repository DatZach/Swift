/*
 *	Object.cpp
 *	Effectively a variant
 */

#include <sstream>
#include "Variant.hpp"
#include "VirtualMachine.hpp"

Variant::Variant(int value)
	: type(ObjectType::Int),
	  marked(false),
	  intValue(value),
	  next(nullptr)
{
	InitGc();
}

Variant::Variant(const std::string* value)
	: type(ObjectType::String),
	  marked(false),
	  stringValue(new std::string(*value)),
	  next(nullptr)
{
	InitGc();
}

Variant::Variant(const std::string& value)
	: type(ObjectType::String),
	  marked(false),
	  stringValue(new std::string(value)),
	  next(nullptr)
{
	InitGc();
}

Variant::Variant(Variant* value)
	: type(ObjectType::Object),
	  marked(false),
	  objectValue(value),
	  next(nullptr)
{
	InitGc();
}

Variant::~Variant()
{
	switch(type)
	{
		case ObjectType::String:
			delete stringValue;
			break;

		case ObjectType::Object:
			// NOTE: DO NOT delete object values, the GC will do this for us!
			break;
	}
}

void Variant::InitGc()
{
	// If we're still loading instructions then don't track them in the GC
	if (!vm.gcActive)
		return;

	next = vm.gcFirstObject;
	marked = false;

	vm.gcFirstObject = this;
	++vm.gcObjectCount;
}

void Variant::Mark()
{
	// Avoid infinite recursion
	if (marked)
		return;

	marked = true;

	switch(type)
	{
		case ObjectType::Object:
			if (objectValue != nullptr)
				objectValue->Mark();
			break;
	}
}

void Variant::Unmark()
{
	marked = false;
}

std::string Variant::Value() const
{
	std::stringstream ss;

	switch(type)
	{
		case ObjectType::Int:
			ss << intValue;
			break;

		case ObjectType::String:
			ss << *stringValue;
			break;

		case ObjectType::Object:
			ss << "<object>";
	}

	return ss.str();
}

Variant::ObjectType Variant::GetType() const
{
	return type;
}

bool Variant::GetMarked() const
{
	return marked;
}

Variant* Variant::GetNext() const
{
	return next;
}
