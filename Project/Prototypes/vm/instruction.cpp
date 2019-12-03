/*
 *	instruction.cpp
 *	Virtual Machine Instruction
 */

#include <instruction.hpp>

namespace Vm
{
	Instruction::Instruction(Opcode _opcode)
		: opcode(_opcode),
		  operands()
	{

	}

	Instruction::Instruction(Opcode _opcode, Variant _operand)
		: opcode(_opcode),
		  operands()
	{
		operands.push_back(_operand);
	}

	Instruction::Instruction(Opcode _opcode, const std::vector<Variant>& _operands)
		: opcode(_opcode),
		  operands(_operands)
	{

	}

	Variant Instruction::operator[](size_t i) const
	{
		return operands[i];
	}

	Instruction::Opcode Instruction::GetOpcode() const
	{
		return opcode;
	}

	Variant Instruction::GetOperand() const
	{
		return operands[0];
	}
}
