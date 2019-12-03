/*
 *	instruction.hpp
 *	Virtual Machine Instruction
 */

#ifndef __VIRTUAL_MACHINE_INSTRUCTION_HPP
#define __VIRTUAL_MACHINE_INSTRUCTION_HPP

#include <vector>
#include <variant.hpp>

namespace Vm
{
	class Instruction
	{
	public:
		typedef enum _Opcode
		{
			NoOperation					= 0x00,
			Push						= 0x02,
			Pop,
			Swap,
			Duplicate,
			SetVariable					= 0x10,
			GetVariable,
			SetArrayVariable,
			GetArrayVariable,
			GetVariableLength,
			GetArgument,
			GetThis,
			GetBase,
			GetBaseOf,
			Add							= 0x20,
			Subtract,
			Multiply,
			Divide,
			Modulo,
			LogicalNegate				= 0x30,
			BitwiseNegate,
			BitwiseNot,
			BitwiseOr,
			BitwiseXor,
			BitwiseShiftLeft,
			BitwiseShiftRight,
			IncrementVariable			= 0x40,
			IncrementField,
			CompareEqual				= 0x50,
			CompareNotEqual,
			CompareGreaterThan,
			CompareLesserThan,
			CompareGreaterThanOrEqual,
			CompareLesserThanOrEqual,
			IfTrue,
			IfFalse,
			Jump,
			Return,
			ClassSetFieldStatic			= 0x60,
			ClassGetFieldStatic,
			ClassSetField,
			ClassGetField,
			ClassCall,
			ClassCallStatic,
			ClassCallVirtual,
			New							= 0x70,
			CastVariant					= 0x80,
			Print						= 0xF0,
			Breakpoint
		} Opcode;

	private:
		Opcode opcode;
		std::vector<Variant> operands;

	public:
		Instruction(Opcode _opcode);
		Instruction(Opcode _opcode, Variant _operand);
		Instruction(Opcode _opcode, const std::vector<Variant>& _operands);

		Variant operator[](size_t i) const;

		Opcode GetOpcode() const;
		Variant GetOperand() const;
	};
}

#endif
