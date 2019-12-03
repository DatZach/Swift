/*
 *	VirtualMachine.cpp
 *	Virtual Machine
 */

#include <iostream>
#include "VirtualMachine.hpp"

VirtualMachine::VirtualMachine()
	: stack(),
	  gcFirstObject(nullptr),
	  gcObjectCount(0),
	  gcMaxObjectCount(GC_MAX_OBJECT_COUNT),
	  gcActive(true)
{
		
}

void VirtualMachine::GarbageCollect()
{
	int initialObjects = gcObjectCount;

	// Step 1: Mark
	int i = stack.GetCount();
	while(i--)
		stack[i]->Mark();

	// Step 2: Sweep
	Variant** object = &gcFirstObject;
	while(*object)
	{
		if (!(*object)->GetMarked())
		{
			// This object wasn't reached, so remove it from the list and free it
			Variant* unreached = *object;
			*object = unreached->next;
			delete unreached;

			gcObjectCount--;
		}
		else
		{
			// This object was reached, unmark it for the next GC iteration
			(*object)->Unmark();

			object = &(*object)->next;
		}
	}

	/*
	 *	Step 3: Update max object count
	 *	NOTE
	 *	At the end of every InternalExecute() if gcObjectCount > gcMaxObjectCount
	 *	then run GarbageCollection().
	 *	This will run the GC after an appropriate number of allocations.
	 */
	gcMaxObjectCount = gcObjectCount * 2;

	// Print results
	std::cout << "[GC] Collected "
			  << (initialObjects - gcObjectCount)
			  << " objects, "
			  << gcObjectCount
			  << " remaining."
			  << std::endl;
}

VirtualMachine& VirtualMachine::GetInstance()
{
	static VirtualMachine instance;
	return instance;
}
