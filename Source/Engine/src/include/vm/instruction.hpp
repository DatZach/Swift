/*
 *	Instruction.hpp
 *	VM Instruction
 */

#ifndef __VIRTUAL_MACHINE_INSTRUCTION_HPP
#define __VIRTUAL_MACHINE_INSTRUCTION_HPP

#include <vector>
#include <Vm/Variant.hpp>

namespace Vm
{
	class Instruction
	{
	public:
		enum Opcode
		{
			// Misc.
			NoOperation					= 0x00,

			// Stack
			Push						= 0x02,
			Pop,
			Swap,
			Duplicate,

			// Variables
			SetVariable					= 0x10,
			GetVariable,
			SetIndex,
			GetIndex,
			GetVariableLength,

			// Classes
			GetThis						= 0x16,
			GetBase,
			GetBaseOf,

			// Arithmetic
			Add							= 0x20,
			Subtract,
			Multiply,
			Divide,
			Modulo,

			// Bitwise
			LogicalNegate				= 0x30,
			BitwiseNegate,
			BitwiseNot,
			BitwiseAnd,
			BitwiseOr,
			BitwiseXor,
			BitwiseShiftLeft,
			BitwiseShiftRight,

			// Optimized arithmetic
			IncrementVariable			= 0x40,
			IncrementField,

			// Flow control
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

			// Classes
			ClassSetFieldStatic			= 0x60,
			ClassGetFieldStatic,
			ClassSetField,
			ClassGetField,
			ClassCall,
			ClassCallStatic,
			ClassCallVirtual,

			// Types
			New							= 0x70,
			TypeOf,

			// Casting
			CastVariant					= 0x80,

			// Debugging
			Breakpoint					= 0xF1
		};

	private:
		Opcode opcode;
		std::vector<Variant> operands;

	public:
		Instruction(Opcode opcode);
		Instruction(Opcode opcode, const Variant& operand);
		Instruction(Opcode opcode, const std::vector<Variant>& operands);

		Opcode Op() const;
		const Variant& Operand() const;

		operator Opcode() const;
		operator const Variant&() const;
		const Variant& operator[](size_t i) const;

		static size_t GetOperandCount(Opcode opcode);
	};
}

#endif
