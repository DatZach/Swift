/*
 *	Main.cpp
 *	Experimental Virtual Machine for Swift
 *	Taylor (Swift)
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <stack>
#include <Vm/VirtualMachine.hpp>

#include <unordered_map>
#include <string>

void WriteLine(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
{
	//throw VmException("Expected argument 0 to be of type 'str'.");

	std::cout << stack.Pop()->Cast(Vm::Variant::Type::String).String() << std::endl;
}

void Destructor(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
{
	std::cout << "DESTRUCTED" << std::endl;
}

class Sarah
{
public:
	Sarah()
	{
		std::cout << "Sarah()" << std::endl;
	}

	~Sarah()
	{
		std::cout << "~Sarah()" << std::endl;
	}

	void Speak()
	{
		std::cout << "WOO!" << std::endl;
	}
};

int main(int argc, char* argv[])
{
	std::vector<Vm::Instruction> instructions;
	/*instructions.push_back(Vm::Instruction(Vm::Instruction::Push, "Hello, World!"));
	//instructions.push_back(Vm::Instruction(Vm::Instruction::Push, 1337));
	instructions.push_back(Vm::Instruction(Vm::Instruction::SetVariable, "x"));
	instructions.push_back(Vm::Instruction(Vm::Instruction::Push, "Y"));
	instructions.push_back(Vm::Instruction(Vm::Instruction::GetVariable, "x"));
	instructions.push_back(Vm::Instruction(Vm::Instruction::Push, 0));
	instructions.push_back(Vm::Instruction(Vm::Instruction::SetIndex));
	instructions.push_back(Vm::Instruction(Vm::Instruction::GetVariable, "x"));
	instructions.push_back(Vm::Instruction(Vm::Instruction::Push, 0));
	instructions.push_back(Vm::Instruction(Vm::Instruction::GetIndex));
	instructions.push_back(Vm::Instruction(Vm::Instruction::GetVariable, "x"));
	std::vector<Vm::Variant> args1;
	args1.push_back("Debug WriteLine");
	args1.push_back(1);
	instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCallStatic, args1));
	instructions.push_back(Vm::Instruction(Vm::Instruction::Push, 1337));
	std::vector<Vm::Variant> args2;
	args2.push_back("Foo Magic");
	args2.push_back(1);
	instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCallStatic, args2));
	//instructions.push_back(Vm::Instruction(Vm::Instruction::Print));
	//instructions.push_back(Vm::Instruction(Vm::Instruction::Breakpoint));
	*/
	
	/*instructions.push_back(Vm::Instruction(Vm::Instruction::Push, "Oi! This is a string."));
	instructions.push_back(Vm::Instruction(Vm::Instruction::GetThis));
	instructions.push_back(Vm::Instruction(Vm::Instruction::ClassSetField, "x"));
	instructions.push_back(Vm::Instruction(Vm::Instruction::GetThis));
	instructions.push_back(Vm::Instruction(Vm::Instruction::ClassGetField, "x"));
	std::vector<Vm::Variant> args1;
	args1.push_back("Debug WriteLine");
	args1.push_back(1);
	instructions.push_back(Vm::Instruction(Vm::Instruction::ClassCallStatic, args1));
	instructions.push_back(Vm::Instruction(Vm::Instruction::Return));*/

	instructions.push_back(Vm::Instruction(Vm::Instruction::New, "Charlie"));
	instructions.push_back(Vm::Instruction(Vm::Instruction::Return));

	std::vector<Vm::Instruction> magicInstr;
	magicInstr.push_back(Vm::Instruction(Vm::Instruction::Return));

	Vm::Class foo;
	foo["x"] = 1337;
	foo.AddMethod("Bar", instructions, 0);
	foo.AddMethod("Magic", magicInstr, 1);

	VirtualMachine.AddClass("Foo", foo);

	Vm::Class Debug;
	Debug.AddMethod("WriteLine", WriteLine, 1);
	VirtualMachine.AddClass("Debug", Debug);

	Vm::Class Charlie;
	Charlie["s"] = Vm::Variant(new Sarah(), [](void* x) { delete reinterpret_cast<Sarah*>(x); });
	Charlie["s"].EngineObject<Sarah>()->Speak();
	
	/*struct D {
		void operator()(void* p) {
			delete reinterpret_cast<Sarah*>(p);
		}
	};

	Charlie["s"] = Vm::Variant(new Sarah(), D());*/
	Charlie.AddMethod(VM_CLASS_DTOR_NAME, Destructor, 0);
	VirtualMachine.AddClass("Charlie", Charlie);

	auto start = std::chrono::high_resolution_clock::now();

	VirtualMachine.Execute(VirtualMachine["Foo"].Method("Bar", 0), Util::Stack<Vm::Variant*>());
	
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Executed in " << duration.count() << " seconds." << std::endl;

	std::cin.ignore();

	return 0;
}
