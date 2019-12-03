/*
*	Main.cpp
*	Research GC (Swift)
*	Original Source:
*		https://github.com/munificent/mark-sweep/blob/master/main.c
*	Tutorial:
*		http://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector/
*/

#include <iostream>
#include "VirtualMachine.hpp"

#ifdef DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

class MemoryLeakChecker
{
public:
	~MemoryLeakChecker()
	{
		 std::cout << (_CrtDumpMemoryLeaks() == 0 ? "No leaks!" : "Leaks detected!")
			       << std::endl;
		 std::cin.ignore();
	}
};

/*
 *	Note that this should work fine on Windows when compiled with MSVC.
 *	This takes advantage of a side effect of the CRT's implementation.
 *	"These terminators MUST be executed in reverse order (LIFO)!"
 *		crt0dat.c(540)
 */
MemoryLeakChecker checker;
#endif

static void assert(int condition, const char* message)
{
	if (!condition)
	{
		std::cout << "ASSERTION FAILED! " << message << std::endl;
		std::cin.ignore();
		exit(1);
	}
}

static void Test1()
{
	std::cout << "Test 1: Objects on stack are preserved." << std::endl;

	vm.stack.Push(new Variant(1));
	vm.stack.Push(new Variant(2));

	vm.GarbageCollect();

	assert(vm.gcObjectCount == 2, "Should have preserved objects.");
}

static void Test2()
{
	std::cout << "Test 2: Unreached objects are collected." << std::endl;

	vm.GarbageCollect();
	vm.stack.Push(new Variant(1));
	vm.stack.Push(new Variant("Hello, World!"));
	vm.stack.Pop();
	vm.GarbageCollect();
	vm.stack.Push(new Variant("Foobar"));
	vm.stack.Pop();
	vm.stack.Pop();

	Variant* variant = new Variant(1337);
	vm.stack.Push(new Variant(variant));
	vm.stack.Pop();

	vm.GarbageCollect();

	assert(vm.gcObjectCount == 0, "Should have collected objects.");
}

int main(int argc, char* argv[])
{
	//Test1();
	Test2();

	return 0;
}
