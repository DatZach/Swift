/*
 *	Instruction.cpp
 *	VM Instruction
 */

#include <Vm/Instruction.hpp>

namespace Vm
{
	Instruction::Instruction(Opcode opcode)
		: opcode(opcode),
		  operands()
	{
		
	}

	Instruction::Instruction(Opcode opcode, const Variant& operand)
		: opcode(opcode),
		  operands()
	{
		operands.push_back(operand);
	}

	Instruction::Instruction(Opcode opcode, const std::vector<Variant>& operands)
		: opcode(opcode),
		  operands(operands)
	{

	}

	Instruction::Opcode Instruction::Op() const
	{
		return opcode;
	}

	Variant const& Instruction::Operand() const
	{
		return operands[0];
	}

	Instruction::operator Opcode() const
	{
		return opcode;
	}

	Variant const& Instruction::operator[](size_t i) const
	{
		return operands[i];
	}

	size_t Instruction::GetOperandCount(Opcode opcode)
	{
		switch(opcode)
		{
			case Push:
			case SetVariable:
			case GetVariable:
			case IfTrue:
			case IfFalse:
			case Jump:
			case ClassSetFieldStatic:
			case ClassGetFieldStatic:
			case ClassSetField:
			case ClassGetField:
			case New:
			case CastVariant:
				return 1;

			case ClassCall:
			case ClassCallStatic:
			case IncrementVariable:
			case IncrementField:
				return 2;

			default:
				return 0;
		}

		return 0;
	}

	Instruction::operator const Variant&() const
	{
		return Operand();
	}
}
