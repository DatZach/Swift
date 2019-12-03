/*
 *	main.cpp
 *	Swift Virtual Machine
 */

#include <iostream>
#include <vector>
#include <string>
#include <vm.hpp>

/*
	Example Class

	class Baz;
		
	private var foo = 123456;
	private var bar = 654321;

	method FooBar()
	{
		foo = foo + bar;
		print foo;
			
		Carl();
		Bob();

		print A() + 4321;
	}

	method Carl()
	{
		print "Called a bytecode method!";
	}

	method Bob()
	{
		// Hooked (std::cout)
	}

	method A(var a)
	{
		return a * 2;
	}
*/

void BobExecute(Util::Stack<Vm::Variant>& stack, Vm::Class* parent)
{
	std::cout << "Called a hooked method!" << std::endl;
}

#include <cmath>

void MathCos(Util::Stack<Vm::Variant>& stack, Vm::Class* parent)
{
	parent->GetField("bar")->value.intValue = 0xDEADBEEF;
	double value = cos(stack.Pop().doubleValue);
	stack.Push(Vm::Variant(value));
}

void HelloWorld(Util::Stack<Vm::Variant>& stack, Vm::Class* parent)
{
	std::cout << "Hello, World!" << std::endl;
}

int main(int argc, char* argv[])
{
	Vm::Class baz;
	baz.SetName("Baz");

	// Add fields
	baz.AddField(Vm::Field("foo", Vm::Variant(123456LL)));
	baz.AddField(Vm::Field("bar", Vm::Variant(654321LL)));

	// Add method "FooBar"
	{
		std::vector<Vm::Instruction> instructions;
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassGetField, Vm::Variant("foo")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassGetField, Vm::Variant("bar")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Add));
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassSetField, Vm::Variant("foo")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassGetField, Vm::Variant("foo")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Print));
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCall, Vm::Variant("Carl")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCall, Vm::Variant("Bob")));

		instructions.push_back(Vm::Instruction(Vm::Instruction::Push, Vm::Variant(1234LL)));
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCall, Vm::Variant("A")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Push, Vm::Variant(4321LL)));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Add));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Print));

		instructions.push_back(Vm::Instruction(Vm::Instruction::Push, Vm::Variant(12.34)));
		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCall, Vm::Variant("cos")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Print));

		instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCallStatic, Vm::Variant("World Hello")));

		instructions.push_back(Vm::Instruction(Vm::Instruction::Push, Vm::Variant(12.34)));
		instructions.push_back(Vm::Instruction(Vm::Instruction::SetVariable, Vm::Variant("a")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Push, Vm::Variant(43.21)));
		instructions.push_back(Vm::Instruction(Vm::Instruction::SetVariable, Vm::Variant("b")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::GetVariable, Vm::Variant("a")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::GetVariable, Vm::Variant("b")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Add));
		instructions.push_back(Vm::Instruction(Vm::Instruction::SetVariable, Vm::Variant("a")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::GetVariable, Vm::Variant("a")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Print));

		instructions.push_back(Vm::Instruction(Vm::Instruction::Return));
		
		baz.AddMethod(Vm::Method("FooBar", instructions, 0));
	}

	// Add method "Carl"
	{
		std::vector<Vm::Instruction> instructions;
		instructions.push_back(Vm::Instruction(Vm::Instruction::Push, Vm::Variant("Called a bytecode method!")));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Print));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Return));

		baz.AddMethod(Vm::Method("Carl", instructions, 0));
	}

	// Add method "Bob"
	baz.AddMethod(Vm::Method("Bob", BobExecute, 0));

	// Add method "A"
	{
		std::vector<Vm::Instruction> instructions;
		instructions.push_back(Vm::Instruction(Vm::Instruction::GetArgument));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Push, Vm::Variant(2LL)));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Multiply));
		instructions.push_back(Vm::Instruction(Vm::Instruction::Return));

		baz.AddMethod(Vm::Method("A", instructions, 1));
	}

	// Add cosine method
	baz.AddMethod(Vm::Method("cos", MathCos, 1));

	Vm::Class hello;
	hello.SetName("World");
	hello.AddMethod(Vm::Method("Hello", HelloWorld, 0));

	VirtualMachine.AddClass(hello);

	// Execute
	VirtualMachine.Execute(*baz.GetMethod("FooBar"), Util::Stack<Vm::Variant>());

	// We can get fields from instances
	std::cout << "foo = " << baz.GetField("foo")->value.intValue << std::endl;
	std::cout << "bar = 0x" << std::hex << baz.GetField("bar")->value.intValue << std::endl;

	std::cin.get();

	return 0;
}
