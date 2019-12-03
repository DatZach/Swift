/*
 *	Variant.hpp
 *	Effectively a variant
 */

#ifndef __VARIANT_HPP
#define __VARIANT_HPP

#include <string>

class Variant
{
public:
	enum class ObjectType
	{
		Int,
		String,
		Object
	};

private:
	ObjectType type;
	bool marked;

	union
	{
		int intValue;
		std::string* stringValue;
		Variant* objectValue;
	};

	void InitGc();

public:
	// I guess this needs to be public...
	Variant* next;

public:
	Variant(int value);
	Variant(const std::string* value);
	Variant(const std::string& value);
	Variant(Variant* value);
	~Variant();

	void Mark();
	void Unmark();

	std::string Value() const;
	ObjectType GetType() const;
	bool GetMarked() const;
	Variant* GetNext() const;
};

#endif
