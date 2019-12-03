/*
 *	VirtualMachine.hpp
 *	Virtual Machine
 */

#ifndef __VIRTUAL_MACHINE_HPP
#define __VIRTUAL_MACHINE_HPP

#include "Variant.hpp"
#include "Stack.hpp"

#define vm		VirtualMachine::GetInstance()

class VirtualMachine
{
private:
	static const int GC_MAX_OBJECT_COUNT = 8;

public:
	Stack<Variant*> stack;

	// Garbage Collector information
	Variant* gcFirstObject;
	int gcObjectCount;
	int gcMaxObjectCount;
	bool gcActive;

public:
	VirtualMachine();

	void GarbageCollect();

	static VirtualMachine& GetInstance();
};

#endif
