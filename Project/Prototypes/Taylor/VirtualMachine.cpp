/*
 *	VirtualMachine.cpp
 *	Taylor Swift VM
 */

#include <iostream>
#include <iomanip>
#include <Vm/VirtualMachine.hpp>

namespace Vm
{
	VirtualMachineSingleton::VirtualMachineSingleton()
		: classes(),
		  debugMethodStack(),
		  ip(0),
		  gcFirstObject(nullptr),
		  gcObjectCount(0),
		  gcMaxObjectCount(GC_MAX_OBJECT_COUNT)
	{

	}

	VirtualMachineSingleton::~VirtualMachineSingleton()
	{
		Variant* object = gcFirstObject;
		while(object)
		{
			Variant* next = object->Next();
			delete object;
			object = next;
		}
	}

	void VirtualMachineSingleton::ExecuteBytecode(Method const& method, Util::Stack<Variant*>& argumentStack)
	{
		std::unordered_map<std::string, Variant*> variables;
		Util::Stack<Variant*> stack(argumentStack, argumentStack.Count());

		size_t size = method.Size();
		ip = 0;

		while(ip < size)
		{
			const Instruction& instruction = method[ip];

			switch(instruction)
			{
				// Stack
				case Instruction::Push:
					stack.Push(CreateVariant(instruction));
					break;

				case Instruction::Pop:
					stack.Pop();
					break;

				case Instruction::Swap:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();
					
					stack.Push(a);
					stack.Push(b);
					break;
				}

				case Instruction::Duplicate:
					stack.Push(CreateVariant(*stack.Peek()));
					break;

				// Variables
				case Instruction::SetVariable:
					variables[instruction.Operand()] = stack.Pop();
					break;

				case Instruction::GetVariable:
					stack.Push(CreateVariant(*variables[instruction.Operand()]));
					break;

				case Instruction::SetIndex:
				{
					// Grab index
					Variant* index = stack.Pop();
					if (index->TypeOf() != Variant::Type::Integer)
					{
						std::stringstream ess;
						ess << "Cannot index a list or string with type '"
							<< Variant::TypeName(index->TypeOf())
							<< "' (set).";

						throw Exception(ess.str());
					}

					// Get list/string
					Variant* value = stack.Pop();
					size_t idx = static_cast<size_t>(index->Integer());

					// Index that shit, yo
					switch(value->TypeOf())
					{
						case Variant::Type::String:
							value->StringAt(idx) = stack.Pop()->StringAt(0);
							break;

						case Variant::Type::List:
							value->ListAt(idx) = stack.Pop();
							break;

						default:
						{
							std::stringstream ess;
							ess << "Cannot index type '"
								<< Variant::TypeName(value->TypeOf())
								<< "' (set).";

							throw Exception(ess.str());
						}
					}
					break;
				}

				case Instruction::GetIndex:
				{
					// Grab index
					Variant* index = stack.Pop();
					if (index->TypeOf() != Variant::Type::Integer)
					{
						std::stringstream ess;
						ess << "Cannot index a list or string with type '"
							<< Variant::TypeName(index->TypeOf())
							<< "' (get).";

						throw Exception(ess.str());
					}

					// Get list/string
					Variant* value = stack.Pop();
					size_t idx = static_cast<size_t>(index->Integer());

					// Index that shit, yo
					switch(value->TypeOf())
					{
						case Variant::Type::String:
							stack.Push(CreateVariant(value->StringAt(idx)));
							break;

						case Variant::Type::List:
							stack.Push(value->ListAt(idx));
							break;

						default:
						{
							std::stringstream ess;
							ess << "Cannot index type '"
								<< Variant::TypeName(value->TypeOf())
								<< "' (get).";

							throw Exception(ess.str());
						}
					}
					break;
				}

				case Instruction::GetVariableLength:
					stack.Push(CreateVariant(stack.Pop()->Length()));
					break;

				// Classes
				case Instruction::GetThis:
					stack.Push(CreateVariant(method.parent));
					break;

				case Instruction::GetBase:
					throw Exception("Opcode 'GetBase' not implemented.");

				case Instruction::GetBaseOf:
					throw Exception("Opcode 'GetBaseOf' not implemented.");

				// Arithmetic
				case Instruction::Add:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b + *a));
					break;
				}

				case Instruction::Subtract:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b - *a));
					break;
				}

				case Instruction::Multiply:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b * *a));
					break;
				}

				case Instruction::Divide:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b / *a));
					break;
				}

				case Instruction::Modulo:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b % *a));
					break;
				}

				// Bitwise
				case Instruction::LogicalNegate:
					stack.Push(CreateVariant(-*stack.Pop()));
					break;

				case Instruction::BitwiseNegate:
					stack.Push(CreateVariant(~*stack.Pop()));
					break;

				case Instruction::BitwiseNot:
					stack.Push(CreateVariant(!*stack.Pop()));
					break;

				case Instruction::BitwiseAnd:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b & *a));
					break;
				}

				case Instruction::BitwiseOr:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b | *a));
					break;
				}

				case Instruction::BitwiseXor:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b ^ *a));
					break;
				}

				case Instruction::BitwiseShiftLeft:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b << *a));
					break;
				}

				case Instruction::BitwiseShiftRight:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b >> *a));
					break;
				}

				// Optimized arithmetic
				case Instruction::IncrementVariable:
					variables[instruction[0]] = variables[instruction[0]] + instruction[1];
					break;

				case Instruction::IncrementField:
				{
					// Get class pointer
					Class* parent = stack.Pop()->Object();
					if (parent == nullptr)
						throw Exception("Cannot resolve field of null instance.");

					// Resolve field
					parent->Field(instruction[0]) = parent->Field(instruction[0]) + instruction[1];
					break;
				}

				// Flow control
				case Instruction::CompareEqual:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b == *a));
					break;
				}

				case Instruction::CompareNotEqual:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b != *a));
					break;
				}

				case Instruction::CompareGreaterThan:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b > *a));
					break;
				}

				case Instruction::CompareLesserThan:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b < *a));
					break;
				}

				case Instruction::CompareGreaterThanOrEqual:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b >= *a));
					break;
				}

				case Instruction::CompareLesserThanOrEqual:
				{
					Variant* a = stack.Pop();
					Variant* b = stack.Pop();

					stack.Push(CreateVariant(*b <= *a));
					break;
				}

				case Instruction::IfTrue:
					if (stack.Pop()->Integer())
					{
						ip = static_cast<size_t>(instruction.Operand().Integer());
						continue;
					}
					break;

				case Instruction::IfFalse:
					if (!stack.Pop()->Integer())
					{
						ip = static_cast<size_t>(instruction.Operand().Integer());
						continue;
					}
					break;

				case Instruction::Jump:
					ip = static_cast<size_t>(instruction.Operand().Integer());
					continue;

				case Instruction::Return:
					// Update argument stack
					argumentStack.Clear();
					argumentStack.Push(stack.Count() ? stack.Pop() : CreateVariant(Variant()));

					// Pop debug method stack
					debugMethodStack.Pop();

					// Run GC
					CollectGarbage(stack);

					// Return from this execution loop
					return;

				// Classes
				case Instruction::ClassSetFieldStatic:
				{
					std::string sClass, sField;

					// Parse "Class Field"
					std::stringstream ss(instruction[0].String());
					ss >> sClass >> sField;

					// Resolve parent class
					Class& parent = classes[sClass];
					if (parent.Name().empty())
						throw Exception("Static class '" + sClass + "' doesn't exist.");

					// Resolve field
					parent[sField] = *stack.Pop();
					break;
				}

				case Instruction::ClassGetFieldStatic:
				{
					std::string sClass, sField;

					// Parse "Class Field"
					std::stringstream ss(instruction[0].String());
					ss >> sClass >> sField;

					// Resolve parent class
					Class& parent = classes[sClass];
					if (parent.Name().empty())
						throw Exception("Static class '" + sClass + "' doesn't exist.");

					// Resolve field
					stack.Push(CreateVariant(parent[sField]));
					break;
				}

				case Instruction::ClassSetField:
				{
					// Get class pointer
					Class* parent = stack.Pop()->Object();
					if (parent == nullptr)
						throw Exception("Cannot call instance method of null instance.");

					// Resolve field
					parent->Field(instruction.Operand()) = *stack.Pop();
					break;
				}

				case Instruction::ClassGetField:
				{
					// Get class pointer
					Class* parent = stack.Pop()->Object();
					if (parent == nullptr)
						throw Exception("Cannot call instance method of null instance.");

					// Resolve field
					stack.Push(CreateVariant(parent->Field(instruction.Operand())));
					break;
				}

				case Instruction::ClassCall:
				{
					// Get class pointer
					Class* parent = stack.Pop()->Object();
					if (parent == nullptr)
						throw Exception("Cannot call instance method of null instance.");

					// Resolve method
					std::string sMethod = instruction[0];
					Method& method = parent->Method(sMethod, static_cast<size_t>(instruction[1].Integer()));
					if (method.Type() == Method::Type::Illegal)
						throw Exception("Static class method '" + sMethod + "' doesn't exist.");

					// Prepare debugging information
					debugMethodStack.Push(DebugScope(parent->Name(), method.Name(), ip));

					// Execute method
					size_t reentryIp = ip;

					Util::Stack<Variant*> argStack(stack, method.ArgumentCount());
					method.Execute(argStack, parent);
					stack.Push(argStack.Pop());

					ip = reentryIp;

					// This is kind of a hack, whatever
					if (method.Type() == Method::Type::Hooked)
						debugMethodStack.Pop();

					break;
				}

				case Instruction::ClassCallStatic:
				{
					std::string sClass, sMethod;

					// Parse "Class Method"
					std::stringstream ss(instruction[0].String());
					ss >> sClass >> sMethod;

					// Resolve parent class
					Class& parent = classes[sClass];
					if (parent.Name().empty())
						throw Exception("Static class '" + sClass + "' doesn't exist.");

					// Resolve method
					Method& method = parent.Method(sMethod, static_cast<size_t>(instruction[1].Integer()));
					if (method.Type() == Method::Type::Illegal)
						throw Exception("Static class method '" + sMethod + "' doesn't exist.");

					// Prepare debugging information
					debugMethodStack.Push(DebugScope(parent.Name(), method.Name(), ip));

					// Execute method
					size_t reentryIp = ip;

					Util::Stack<Variant*> argStack(stack, method.ArgumentCount());
					method.Execute(argStack, &parent);
					stack.Push(argStack.Pop());

					ip = reentryIp;

					// This is kind of a hack, whatever
					if (method.Type() == Method::Type::Hooked)
						debugMethodStack.Pop();

					break;
				}

				case Instruction::ClassCallVirtual:
					throw Exception("Opcode 'ClassCallVirtual' not implemented.");

				// Types
				case Instruction::New:
					stack.Push(CreateVariant(new Class(classes[instruction.Operand()])));
					break;

				case Instruction::TypeOf:
				{
					Variant* value = stack.Pop();

					switch(value->TypeOf())
					{
						case Variant::Type::Object:
							if (value->Object() != nullptr)
							{
								stack.Push(CreateVariant(value->Object()->Name()));
								break;
							}

						default:
							stack.Push(CreateVariant(Variant::TypeName(value->TypeOf())));
					}
					break;
				}

				// Casting
				case Instruction::CastVariant:
				{
					Variant::Type toType = static_cast<Variant::Type>(instruction.Operand().Integer());
					stack.Push(CreateVariant(stack.Pop()->Cast(toType)));
				}

				// Debugging
				case Instruction::Breakpoint:
					throw Exception("<Breakpoint>");
			}

			// Increment IP
			++ip;
		}
	}

	void VirtualMachineSingleton::Execute(const Method& method, Util::Stack<Variant*>& argumentStack)
	{
		// Execute method
		try
		{
			// Check if this method exists
			if (method.Type() == Method::Type::Illegal)
				throw Exception("Internal Error: Method doesn't exist.");

			// Update debug stack
			debugMethodStack.Push(DebugScope(method.parent->Name(), method.Name(), 0));

			ExecuteBytecode(method, argumentStack);
		}
		catch(Exception& e)
		{
			// VM Exception
			std::stringstream ess;
			ess << "VirtualMachine Exception (Vm)";

			if (!debugMethodStack.Empty())
			{
				const DebugScope& scope = debugMethodStack.Peek();
				ess << " @ '" << scope.MethodName() << "'; IP = "
					<< std::hex << std::setw(4) << std::setfill('0') << std::uppercase
					<< ip;
			}

			ess << std::endl
				<< "\t" << e.what()
				<< std::endl << std::endl
				<< StackTrace() << std::endl;

			std::cerr << ess.str();
		}
		catch(Variant::Exception& e)
		{
			// Variant Exception
			std::stringstream ess;
			ess << "VirtualMachine Exception (Variant)";

			if (!debugMethodStack.Empty())
			{
				const DebugScope& scope = debugMethodStack.Peek();
				ess << " @ '" << scope.MethodName() << "'; IP = "
					<< std::hex << std::setw(4) << std::setfill('0') << std::uppercase
					<< ip;
			}

			ess << std::endl
				<< "\t" << e.what()
				<< std::endl << std::endl
				<< StackTrace() << std::endl;

			std::cerr << ess.str();
		}
	}

	void VirtualMachineSingleton::Execute(const Method& method)
	{
		Execute(method, Util::Stack<Variant*>());
	}

	void VirtualMachineSingleton::CollectGarbage(Util::Stack<Variant*>& stack)
	{
		// Do not run if we're not scheduled to
		if (gcObjectCount < gcMaxObjectCount)
		{
			std::cout << "GC is not currently scheduled." << std::endl;
			return;
		}

		int initialObjectCount = gcObjectCount;

		// Step 1: Mark
		int i = stack.Count();
		while(i--)
			stack[i]->Mark();

		// Step 2: Sweep
		Variant** object = &gcFirstObject;
		while(*object)
		{
			if (!(*object)->Marked())
			{
				// This object wasn't reached, so remove it from the list and free it
				Variant* unreached = *object;
				*object = unreached->Next();
				delete unreached;

				--gcObjectCount;
			}
			else
			{
				// This object was reached, unmark it for the next GC sweep
				(*object)->Unmark();
				object = &(*object)->Next();
			}
		}

		// Step 3: Update max object count
		gcMaxObjectCount = (gcMaxObjectCount + 1) << 1;

		// Print results
		std::cout << "GC Collected "
			<< (initialObjectCount - gcObjectCount)
			<< " objects, "
			<< gcObjectCount
			<< " remaining."
			<< std::endl;
	}

	Variant* VirtualMachineSingleton::CreateVariant(const Variant& value)
	{
		/*
		 *	FUTURE OPTIMIZATIONS
		 *	When the GC runs it should keep a seperate list of objects that
		 *	it should delete and this method should pull from that pool if
		 *	at all possible to avoid the heap allocation overhead.
		 */

		// Allocate a new variant
		Variant* variant = new Variant(value);

		// Track in the GC
		variant->Next() = gcFirstObject;
		gcFirstObject = variant;
		++gcObjectCount;

		// Return our new tracked object
		return variant;
	}

	std::string VirtualMachineSingleton::StackTrace()
	{
		const int Padding = 36;

		std::stringstream ss;

		ss << "===================== Stack Trace ======================" << std::endl;
		if (debugMethodStack.Count() > 0)
		{
			size_t correctedIp = 0;

			while(debugMethodStack.Count())
			{
				const DebugScope& scope = debugMethodStack.Pop();

				std::string qualifiedName = scope.ClassName() + '.' + scope.MethodName();
				ss << ' ' << qualifiedName;

				for(int i = Padding - qualifiedName.length(); i > 0; --i)
					ss << ' ';

				ss << "@ 0x"
				   << std::hex << std::setw(4) << std::setfill('0') << std::uppercase
				   << correctedIp
				   << std::endl;

				correctedIp = scope.Ip();
			}

			ss << "    ~~~~~~~~~~~~~~~ BOTTOM OF STACK ~~~~~~~~~~~~~~~~" << std::endl;
		}
		else
			ss << "\t<No symbols loaded>" << std::endl;

		return ss.str();
	}

	Class& VirtualMachineSingleton::operator[](const std::string& name)
	{
		return classes[name];
	}

	void VirtualMachineSingleton::AddClass(std::string const& name, Class& value)
	{
		value.name = name;
		classes[name] = value;
	}

	VirtualMachineSingleton& VirtualMachineSingleton::GetInstance()
	{
		static VirtualMachineSingleton instance;
		return instance;
	}

	// VM Exception
	VirtualMachineSingleton::Exception::Exception(const std::string& message)
		: message(message)
	{

	}

	VirtualMachineSingleton::Exception::~Exception() throw()
	{
		// This is here to prevent GCC from throwing up
	}

	const char* VirtualMachineSingleton::Exception::what() const throw()
	{
		return message.c_str();
	}

	VirtualMachineSingleton::DebugScope::DebugScope()
		: methodName("<Error>"),
		  ip(0xFFFF)
	{

	}

	VirtualMachineSingleton::DebugScope::DebugScope(const std::string& className, std::string const& methodName, size_t ip)
		: className(className),
		  methodName(methodName),
		  ip(ip)
	{

	}

	std::string const& VirtualMachineSingleton::DebugScope::ClassName() const
	{
		return className;
	}

	std::string const& VirtualMachineSingleton::DebugScope::MethodName() const
	{
		return methodName;
	}

	size_t VirtualMachineSingleton::DebugScope::Ip() const
	{
		return ip;
	}
}
