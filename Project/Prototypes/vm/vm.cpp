/*
 *	vm.cpp
 *	Virtual Machine
 */

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vm.hpp>
#include <stack.hpp>

namespace Vm
{
	void VirtualMachineSingleton::Execute(const Method& method, Util::Stack<Variant>& argumentStack)
	{
		// Precache some information about the class
		std::vector<Instruction> instructions = method.GetInstructions();
		std::vector<Field>& fields = method.parent->GetFields();

		Util::Stack<Variant> stack;
		std::map<std::string, Variant> variables;
		size_t ip = 0;
		size_t count = instructions.size();
		
		while(ip < count)
		{
			// Grab instruction
			Instruction instruction = instructions[ip];

			// Execute it
			switch(instruction.GetOpcode())
			{
				case Instruction::Push:
					stack.Push(instruction.GetOperand());
					break;

				case Instruction::Swap:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(a);
					stack.Push(b);
					break;
				}

				case Instruction::Duplicate:
					stack.Push(stack.Peek());
					break;

				case Instruction::SetVariable:
					variables[instruction.GetOperand().stringValue] = stack.Pop();
					break;

				case Instruction::GetVariable:
				{
					const std::string& value = instruction.GetOperand().stringValue;

					if (variables.find(value) == variables.cend())
						break; // TODO Throw exception

					stack.Push(variables[value]);
					break;
				}

				case Instruction::SetArrayVariable:
					break;

				case Instruction::GetArrayVariable:
					break;

				case Instruction::GetVariableLength:
					break;

				case Instruction::GetArgument:
					stack.Push(argumentStack.Pop());
					break;

				case Instruction::GetThis:
					break;

				case Instruction::GetBase:
					break;

				case Instruction::GetBaseOf:
					break;

				case Instruction::Add:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b + a);
					break;
				}

				case Instruction::Subtract:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b - a);
					break;
				}

				case Instruction::Multiply:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b * a);
					break;
				}

				case Instruction::Divide:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b / a);
					break;
				}

				case Instruction::Modulo:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b % a);
					break;
				}

				case Instruction::LogicalNegate:
					stack.Push(-stack.Pop());
					break;

				case Instruction::BitwiseNegate:
					stack.Push(~stack.Pop());
					break;

				case Instruction::BitwiseNot:
					stack.Push(!stack.Pop());
					break;

				case Instruction::BitwiseOr:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b | a);
					break;
				}

				case Instruction::BitwiseXor:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b ^ a);
					break;
				}

				case Instruction::BitwiseShiftLeft:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b << a);
					break;
				}

				case Instruction::BitwiseShiftRight:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(b >> a);
					break;
				}

				case Instruction::IncrementVariable:
					break;

				case Instruction::IncrementField:
					break;

				case Instruction::CompareEqual:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(a == b);
					break;
				}

				case Instruction::CompareNotEqual:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(a != b);
					break;
				}

				case Instruction::CompareGreaterThan:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(a > b);
					break;
				}

				case Instruction::CompareLesserThan:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(a < b);
					break;
				}

				case Instruction::CompareGreaterThanOrEqual:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(a >= b);
					break;
				}

				case Instruction::CompareLesserThanOrEqual:
				{
					Variant a = stack.Pop();
					Variant b = stack.Pop();
					stack.Push(a <= b);
					break;
				}

				case Instruction::IfTrue:
					if (stack.Pop().intValue == 1)
					{
						ip = static_cast<size_t>(instruction.GetOperand().intValue);
						continue;
					}
					break;

				case Instruction::IfFalse:
					if (stack.Pop().intValue == 0)
					{
						ip = static_cast<size_t>(instruction.GetOperand().intValue);
						continue;
					}
					break;

				case Instruction::Jump:
					ip = static_cast<size_t>(instruction.GetOperand().intValue);
					continue;

				case Instruction::Return:
					// Handle argument stack
					argumentStack.Clear();
					argumentStack.Push((stack.GetCount() > 0) ? stack.Pop() : Variant());

					// Return and move up on the call stack
					return;

				case Instruction::ClassSetFieldStatic:
					break;

				case Instruction::ClassGetFieldStatic:
					break;

				case Instruction::ClassSetField:
				{
					for(size_t i = 0; i < fields.size(); ++i)
					{
						if (fields[i].name == instruction.GetOperand().stringValue)
							fields[i].value = stack.Pop();
					}
					break;
				}

				case Instruction::ClassGetField:
				{
					for(size_t i = 0; i < fields.size(); ++i)
					{
						if (fields[i].name == instruction.GetOperand().stringValue)
							stack.Push(fields[i].value);
					}
					break;
				}

				case Instruction::ClassCall:
				{
					// TODO Rewrite with method.parent->GetMethod()...
					std::vector<Method> methods = method.parent->GetMethods();
					std::string methodName = instruction.GetOperand().stringValue;

					Util::Stack<Variant> argStack;

					for(size_t i = 0; i < methods.size(); ++i)
					{
						if (methods[i].GetName() == methodName)
						{
							for(size_t j = 0; j < methods[i].GetArgumentCount(); ++j)
								argStack.Push(stack.Pop());

							if (methods[i].GetMethodType() == Method::Bytecode)
								Execute(methods[i], argStack);
							else
							{
								methods[i].Execute(argStack, method.parent);

								// Special exception for cleaner code in hooked methods
								if (argStack.GetCount() == 0)
									argStack.Push(Variant());
							}

							// There will always be a return value
							// TODO Add checks for an empty arg stack (missing return instruction)
							stack.Push(argStack.Pop());
							break;
						}
					}
					break;
				}

				case Instruction::ClassCallStatic:
				{
					std::stringstream ss(instruction.GetOperand().stringValue);
					std::string strClass, strMethod;
					ss >> strClass >> strMethod;

					Class* sParent = GetClass(strClass);
					if (sParent == NULL) // TODO Throw exception!
						break;

					Method* sMethod = sParent->GetMethod(strMethod);
					if (sMethod == NULL) // TODO Throw exception!
						break;

					Util::Stack<Variant> argStack;
					for(size_t j = 0; j < sMethod->GetArgumentCount(); ++j)
						argStack.Push(stack.Pop());

					if (sMethod->GetMethodType() == Method::Bytecode)
						Execute(*sMethod, argStack);
					else
					{
						sMethod->Execute(argStack, sParent);

						// Special exception for cleaner code in hooked methods
						if (argStack.GetCount() == 0)
							argStack.Push(Variant());
					}

					// There will always be a return value
					// TODO Add checks for an empty arg stack (missing return instruction)
					stack.Push(argStack.Pop());
					
					break;
				}

				case Instruction::ClassCallVirtual:
					break;

				case Instruction::New:
					break;

				case Instruction::CastVariant:
					stack.Push(stack.Pop().Cast(static_cast<Variant::VariantType>(instruction.GetOperand().intValue)));
					break;

				case Instruction::Print:
				{
					Variant value = stack.Pop();
					std::stringstream ss;
					
					switch(value.type)
					{
						case Variant::Integer:
							ss << value.intValue;
							break;

						case Variant::Double:
							ss << value.doubleValue;
							break;

						case Variant::String:
							ss << value.stringValue;
							break;

						default:
							ss << "[ Too lazy to evaluate this ]";
					}

					std::cout << ss.str() << std::endl;
					break;
				}

				case Instruction::Breakpoint:
					// TODO This is a shitty breakpoint
					std::cout << "[Breakpoint]" << std::endl;
					break;

				case Instruction::NoOperation:
					break;
			}

			// Increment IP, not reached by opcodes that continue instead of break
			++ip;
		}
	}

	void VirtualMachineSingleton::AddClass(const Class& value)
	{
		classes.push_back(value);
	}
	
	Class* VirtualMachineSingleton::GetClass(const std::string& value)
	{
		for(size_t i = 0; i < classes.size(); ++i)
		{
			if (classes[i].GetName() == value)
				return &classes[i];
		}

		return NULL;
	}

	VirtualMachineSingleton& VirtualMachineSingleton::GetInstance()
	{
		static VirtualMachineSingleton instance;
		return instance;
	}
}
