using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Swift.Source.Bytecode
{
	internal class Instruction
	{
		public static readonly Dictionary<Opcode, int> OperandCount = new Dictionary<Opcode, int>
		{
			{ Opcode.NoOperation, 0 },
			{ Opcode.Push, 1 },
			{ Opcode.Pop, 0 },
			{ Opcode.Swap, 0 },
			{ Opcode.Duplicate, 0 },
			{ Opcode.SetVariable, 1 },
			{ Opcode.GetVariable, 1 },
			{ Opcode.SetIndex, 0 },
			{ Opcode.GetIndex, 0 },
			{ Opcode.GetVariableLength, 0 },
			{ Opcode.GetArgument, 0 },
			{ Opcode.GetThis, 0 },
			{ Opcode.GetBase, 0 },
			{ Opcode.GetBaseOf, 0 },
			{ Opcode.Add, 0 },
			{ Opcode.Subtract, 0 },
			{ Opcode.Multiply, 0 },
			{ Opcode.Divide, 0 },
			{ Opcode.Modulo, 0 },
			{ Opcode.LogicalNegate, 0 },
			{ Opcode.BitwiseNegate, 0 },
			{ Opcode.BitwiseNot, 0 },
			{ Opcode.BitwiseAnd, 0 },
			{ Opcode.BitwiseOr, 0 },
			{ Opcode.BitwiseXor, 0 },
			{ Opcode.BitwiseShiftLeft, 0 },
			{ Opcode.BitwiseShiftRight, 0 },
			{ Opcode.IncrementVariable, 2 },
			{ Opcode.IncrementField, 2 },
			{ Opcode.CompareEqual, 0 },
			{ Opcode.CompareNotEqual, 0 },
			{ Opcode.CompareGreaterThan, 0 },
			{ Opcode.CompareLesserThan, 0 },
			{ Opcode.CompareGreaterThanOrEqual, 0 },
			{ Opcode.CompareLesserThanOrEqual, 0 },
			{ Opcode.IfTrue, 1 },
			{ Opcode.IfFalse, 1 },
			{ Opcode.Jump, 1 },
			{ Opcode.Return, 0 },
			{ Opcode.ClassSetFieldStatic, 1 },
			{ Opcode.ClassGetFieldStatic, 1 },
			{ Opcode.ClassSetField, 1 },
			{ Opcode.ClassGetField, 1 },
			{ Opcode.ClassCall, 2 },
			{ Opcode.ClassCallStatic, 2 },
			{ Opcode.ClassCallVirtual, 0 },
			{ Opcode.New, 1 },
			{ Opcode.TypeOf, 0 },
			{ Opcode.CastVariant, 1 },
			{ Opcode.Breakpoint, 0 },
		};

		public Opcode Opcode { get; private set; }
		public List<Variant> Operands { get; private set; }

		public Variant Operand
		{
			get
			{
				return Operands.FirstOrDefault() ?? new Variant();
			}
		}

		public Instruction(Opcode opcode, params object[] operands)
		{
			Opcode = opcode;
			Operands = new List<Variant>();

			foreach (object operand in operands)
			{
				if (operand is int)
					Operands.Add(new Variant((int)operand));
				else if (operand is long)
					Operands.Add(new Variant((long)operand));
				else if (operand is double)
					Operands.Add(new Variant((double)operand));
				else if (operand is string)
					Operands.Add(new Variant((string)operand));
				else if (operand is List<Variant>)
					Operands.Add(new Variant((List<Variant>)operand));
				else if (operand is Variant)
					Operands.Add((Variant)operand);
				else
					throw new CompilerException("Illegal operand type '{0}'.", operand.GetType());
			}

			if (OperandCount[Opcode] != Operands.Count)
				throw new CompilerException("Instruction {0} was encoded incorrectly.", Opcode);
		}

		public void Write(BinaryWriter writer)
		{
			writer.Write((byte)Opcode);
			foreach(Variant operand in Operands)
				operand.Write(writer);
		}

		public override string ToString()
		{
			string operand = Operand.ToString().PadLeft(40 - Opcode.ToString().Length, ' ');
			return string.Format("{0}{1}", Opcode, OperandCount[Opcode] > 0 ? operand : "");
		}
	}

	internal enum Opcode : byte
	{
		NoOperation				= 0x00,
		Push					= 0x02,
		Pop,
		Swap,
		Duplicate,
		SetVariable				= 0x10,
		GetVariable,
		SetIndex,
		GetIndex,
		GetVariableLength,
		GetArgument,
		GetThis,
		GetBase,
		GetBaseOf,
		Add						= 0x20,
		Subtract,
		Multiply,
		Divide,
		Modulo,
		LogicalNegate			= 0x30,
		BitwiseNegate,
		BitwiseNot,
		BitwiseAnd,
		BitwiseOr,
		BitwiseXor,
		BitwiseShiftLeft,
		BitwiseShiftRight,
		IncrementVariable		= 0x40,
		IncrementField,
		CompareEqual			= 0x50,
		CompareNotEqual,
		CompareGreaterThan,
		CompareLesserThan,
		CompareGreaterThanOrEqual,
		CompareLesserThanOrEqual,
		IfTrue,
		IfFalse,
		Jump,
		Return,
		ClassSetFieldStatic		= 0x60,
		ClassGetFieldStatic,
		ClassSetField,
		ClassGetField,
		ClassCall,
		ClassCallStatic,
		ClassCallVirtual,
		New						= 0x70,
		TypeOf,
		CastVariant				= 0x80,
		Breakpoint				= 0xF1
	}
}
