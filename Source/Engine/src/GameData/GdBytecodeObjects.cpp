/*
 *	gdbytecodeobjects.cpp
 *	Bytecode Objects
 */

#include <GameData/BytecodeObjects.hpp>
#include <Vm/VirtualMachine.hpp>
#include <Util/Logger.hpp>

namespace GameData
{
	void BytecodeObjects::Read(Util::Stream* stream)
	{
		size_t count = stream->ReadDword();
		while(count--)
		{
			Vm::Class target;

			unsigned char type = stream->ReadByte();
			if (type == 0)
				Warn << "BytecodeObject.Type == Undefined, not sure what that means exactly, probably bad." << lendl;

			std::string targetName = stream->ReadString();

			// Fields
			unsigned short count = stream->ReadWord();
			while(count--)
			{
				std::string name = stream->ReadString();
				target[name] = stream->ReadVariant();
			}

			// Methods
			count = stream->ReadWord();
			while(count--)
			{
				std::string name = stream->ReadString();
				unsigned char argumentCount = stream->ReadByte();
				std::vector<Vm::Instruction> instructions;

				unsigned int length = stream->ReadDword();
				instructions.reserve(length);
				while(length--)
				{
					Vm::Instruction::Opcode opcode = static_cast<Vm::Instruction::Opcode>(stream->ReadByte());
					std::vector<Vm::Variant> operands;
					operands.reserve(1);

					size_t operandCount = Vm::Instruction::GetOperandCount(opcode);
					while(operandCount--)
						operands.push_back(stream->ReadVariant());

					instructions.push_back(Vm::Instruction(opcode, operands));
				}

				target.AddMethod(name, instructions, argumentCount);
			}

			VirtualMachine.AddClass(targetName, target);
		}
	}
}
