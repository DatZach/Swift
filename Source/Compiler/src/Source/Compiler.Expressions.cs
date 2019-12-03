using System.Collections.Generic;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Source
{
	internal partial class Compiler
	{
		private void Assignment()
		{
			int initialPosition = TokenStream.Position;

			// Skip initial reference to get to expression fixpoint
			string refIdent = TokenStream.Read().Value;

			// Ignore type annotations...
			if (TokenStream.Accept(TokenType.Delimiter, ":"))
				TokenStream.ReadWord();

			// Parse descending references
			while (TokenStream.Accept(TokenType.Delimiter, "."))
				TokenStream.ReadWord();

			// And indexer
			while (TokenStream.Accept(TokenType.Delimiter, "["))
			{
				while (!TokenStream.Accept(TokenType.Delimiter, "]"))
					TokenStream.Read();
			}

			if (Parser.IsAssignOperation(TokenStream.Peek()))
			{
				Dictionary<string, Opcode> assignmentOperations = new Dictionary<string, Opcode>
				{
					{ "+=", Opcode.Add },
					{ "-=", Opcode.Subtract },
					{ "*=", Opcode.Multiply },
					{ "/=", Opcode.Divide },
					{ "%=", Opcode.Modulo },
					{ "|=", Opcode.BitwiseOr },
					{ "^=", Opcode.BitwiseXor },
					{ "&=", Opcode.BitwiseAnd },
					{ "<<=", Opcode.BitwiseShiftLeft },
					{ ">>=", Opcode.BitwiseShiftRight }
				};

				// Read assignment operator
				string assignOp = TokenStream.Read().Value;

				// Check if this is a relative assignment
				if (assignmentOperations.ContainsKey(assignOp))
				{
					// This is a relative assignment, so we need to get the value from earlier
					TokenStream.PushPosition();
					TokenStream.Position = initialPosition;
					GetReference();
					TokenStream.PopPosition();
				}

				// Parse r-value
				Ternary();

				// If this was a relative assignment, add relative assignment
				if (assignmentOperations.ContainsKey(assignOp))
					Instructions.Add(new Instruction(assignmentOperations[assignOp]));

				TokenStream.PushPosition();
				TokenStream.Position = initialPosition;
				SetReference();
				TokenStream.PopPosition();

				if (typeStack.Count > 0)
				{
					if (IsVariableInScope(refIdent) && currentMethod.Variables[refIdent].Type == VariantType.Nil &&
												string.IsNullOrEmpty(currentMethod.Variables[refIdent].StringValue))
						currentMethod.Variables[refIdent].ObjectName = typeStack.Pop();
					else if (IsField(refIdent, currentClass) && GetField(refIdent, currentClass).Value.Type == VariantType.Nil &&
																string.IsNullOrEmpty(GetField(refIdent, currentClass).Value.StringValue))
						GetField(refIdent, currentClass).Value.ObjectName = typeStack.Pop();
				}
			}
			else
			{
				// This isn't an assignment, read ternary
				TokenStream.Position = initialPosition;
				Ternary();
			}
		}

		private void Ternary()
		{
			LogicalOr();

			while (TokenStream.Accept(TokenType.Delimiter, "?"))
			{
				Label labelElse = new Label(this);
				Label labelEnd = new Label(this);

				Instructions.Add(new Instruction(Opcode.Push, 0));
				Instructions.Add(new Instruction(Opcode.CompareNotEqual));
				Instructions.Add(new Instruction(Opcode.IfFalse, 0));
				labelElse.PatchHere();

				LogicalOr();
				Instructions.Add(new Instruction(Opcode.Jump, 0));
				labelEnd.PatchHere();

				TokenStream.Expect(TokenType.Delimiter, ":");

				labelElse.Mark();
				LogicalOr();
				labelEnd.Mark();

				labelElse.Fix();
				labelEnd.Fix();
			}
		}

		private void LogicalOr()
		{
			LogicalAnd();

			while (TokenStream.Accept(TokenType.Delimiter, "||"))
			{
				LogicalAnd();
				Instructions.Add(new Instruction(Opcode.Add));
				Instructions.Add(new Instruction(Opcode.Push, 0));
				Instructions.Add(new Instruction(Opcode.CompareGreaterThan));
			}
		}

		private void LogicalAnd()
		{
			BitwiseOr();

			while (TokenStream.Accept(TokenType.Delimiter, "&&"))
			{
				BitwiseOr();
				Instructions.Add(new Instruction(Opcode.Add));
				Instructions.Add(new Instruction(Opcode.Push, 2));
				Instructions.Add(new Instruction(Opcode.CompareEqual));
			}
		}

		private void BitwiseOr()
		{
			BitwiseXor();

			while (TokenStream.Accept(TokenType.Delimiter, "|"))
			{
				BitwiseXor();
				Instructions.Add(new Instruction(Opcode.BitwiseOr));
			}
		}

		private void BitwiseXor()
		{
			BitwiseAnd();

			while (TokenStream.Accept(TokenType.Delimiter, "^"))
			{
				BitwiseAnd();
				Instructions.Add(new Instruction(Opcode.BitwiseXor));
			}
		}

		private void BitwiseAnd()
		{
			Equality();

			while (TokenStream.Accept(TokenType.Delimiter, "&"))
			{
				Equality();
				Instructions.Add(new Instruction(Opcode.BitwiseAnd));
			}
		}

		private void Equality()
		{
			Relational();

			while (Parser.IsEqualityOperation(TokenStream.Peek()))
			{
				if (TokenStream.Accept(TokenType.Delimiter, "=="))
				{
					Relational();
					Instructions.Add(new Instruction(Opcode.CompareEqual));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, "!="))
				{
					Relational();
					Instructions.Add(new Instruction(Opcode.CompareNotEqual));
				}
			}
		}

		private void Relational()
		{
			Shift();

			while (Parser.IsRelationalOperation(TokenStream.Peek()))
			{
				if (TokenStream.Accept(TokenType.Delimiter, ">"))
				{
					Shift();
					Instructions.Add(new Instruction(Opcode.CompareGreaterThan));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, "<"))
				{
					Shift();
					Instructions.Add(new Instruction(Opcode.CompareLesserThan));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, ">="))
				{
					Shift();
					Instructions.Add(new Instruction(Opcode.CompareGreaterThanOrEqual));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, "<="))
				{
					Shift();
					Instructions.Add(new Instruction(Opcode.CompareLesserThanOrEqual));
				}
			}

			if (TokenStream.Accept(TokenType.Word, "is"))
			{
				Instructions.Add(new Instruction(Opcode.TypeOf));

				if (IsClass(TokenStream.Peek()))
					Instructions.Add(new Instruction(Opcode.Push, new Variant(TokenStream.ReadWord())));
				else if (TokenStream.Accept(TokenType.Word, "nil"))
					Instructions.Add(new Instruction(Opcode.Push, "nil"));
				else if (TokenStream.Accept(TokenType.Word, "int"))
					Instructions.Add(new Instruction(Opcode.Push, "int"));
				else if (TokenStream.Accept(TokenType.Word, "double"))
					Instructions.Add(new Instruction(Opcode.Push, "double"));
				else if (TokenStream.Accept(TokenType.Word, "str"))
					Instructions.Add(new Instruction(Opcode.Push, "str"));
				else if (TokenStream.Accept(TokenType.Word, "list"))
					Instructions.Add(new Instruction(Opcode.Push, "list"));
				else
				{
					Ternary();
					Instructions.Add(new Instruction(Opcode.TypeOf));
				}

				Instructions.Add(new Instruction(Opcode.CompareEqual));
			}
		}

		private void Shift()
		{
			Additive();

			while (Parser.IsShiftOperation(TokenStream.Peek()))
			{
				if (TokenStream.Accept(TokenType.Delimiter, "<<"))
				{
					Additive();
					Instructions.Add(new Instruction(Opcode.BitwiseShiftLeft));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, ">>"))
				{
					Additive();
					Instructions.Add(new Instruction(Opcode.BitwiseShiftRight));
				}
			}
		}

		private void Additive()
		{
			Multiplicative();

			while (Parser.IsAddOperation(TokenStream.Peek()))
			{
				if (TokenStream.Accept(TokenType.Delimiter, "+"))
				{
					Multiplicative();
					Instructions.Add(new Instruction(Opcode.Add));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, "-"))
				{
					Multiplicative();
					Instructions.Add(new Instruction(Opcode.Subtract));
				}
			}
		}

		private void Multiplicative()
		{
			Unary();

			while (Parser.IsMulOperation(TokenStream.Peek()))
			{
				if (TokenStream.Accept(TokenType.Delimiter, "*"))
				{
					Unary();
					Instructions.Add(new Instruction(Opcode.Multiply));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, "/"))
				{
					Unary();
					Instructions.Add(new Instruction(Opcode.Divide));
				}
				else if (TokenStream.Accept(TokenType.Delimiter, "%"))
				{
					if (TokenStream.Pass(TokenType.Delimiter, "["))
					{
						// Write hacks err day
						Ternary();
						Instructions.Add(new Instruction(Opcode.ClassCallStatic, "__str Format", 2));
					}
					else
					{
						Unary();
						Instructions.Add(new Instruction(Opcode.Modulo));
					}
				}
			}
		}

		private void Unary()
		{
			if (Parser.IsAddOperation(TokenStream.Peek()))
				UnarySign();
			else if (TokenStream.Accept(TokenType.Delimiter, "!"))
			{
				Primary();
				Instructions.Add(new Instruction(Opcode.BitwiseNot));
			}
			else if (TokenStream.Accept(TokenType.Delimiter, "~"))
			{
				Primary();
				Instructions.Add(new Instruction(Opcode.BitwiseNegate));
			}
			else if (Parser.IsCastOperation(TokenStream.Peek()))
				TypeCast();
			else if (Parser.IsIncrementOperation(TokenStream.Peek()))
				PrefixIncrement();
			else
				Primary();
		}

		private void UnarySign()
		{
			if (TokenStream.Accept(TokenType.Delimiter, "+"))
			{
				Label aboveLabel = new Label(this);

				Primary();

				Instructions.Add(new Instruction(Opcode.Duplicate));
				Instructions.Add(new Instruction(Opcode.Push, 0));
				Instructions.Add(new Instruction(Opcode.CompareGreaterThan));
				Instructions.Add(new Instruction(Opcode.IfTrue, 0));
				aboveLabel.PatchHere();
				Instructions.Add(new Instruction(Opcode.LogicalNegate));

				aboveLabel.Mark();
				aboveLabel.Fix();
			}
			else if (TokenStream.Accept(TokenType.Delimiter, "-"))
			{
				Primary();
				Instructions.Add(new Instruction(Opcode.LogicalNegate));
			}
		}

		private void TypeCast()
		{
			Dictionary<string, VariantType> typeCasts = new Dictionary<string, VariantType>
			{
				{ "int", VariantType.Int64 },
				{ "double", VariantType.Double },
				{ "str", VariantType.String }
			};

			string castIdent = TokenStream.ReadWord();
			TokenStream.Expect(TokenType.Delimiter, "(");
			if (castIdent == "str")
			{
				string ident = TokenStream.Peek().Value;
				if (!currentMethod.Variables.ContainsKey(ident))
					Ternary();
				else
				{
					Class parentClass = GetClass(currentMethod.Variables[ident].StringValue);
					if (parentClass == null || !IsMethod("str", parentClass))
						Ternary();
					else
					{
						Ternary();
						Instructions.Add(new Instruction(Opcode.ClassCall, "str"));
					}

					typeStack.Push("__str");
				}
			}
			else
				Ternary();

			TokenStream.Expect(TokenType.Delimiter, ")");

			Instructions.Add(new Instruction(Opcode.CastVariant, (long)typeCasts[castIdent]));
		}

		private void PrefixIncrement()
		{
			if (TokenStream.Accept(TokenType.Delimiter, "++"))
			{
				TokenStream.PushPosition();
				GetReference();
				Instructions.Add(new Instruction(Opcode.Push, 1));
				Instructions.Add(new Instruction(Opcode.Add));
				TokenStream.PopPosition();
				TokenStream.PushPosition();
				SetReference();
				TokenStream.PopPosition();
				GetReference();
			}
			else if (TokenStream.Accept(TokenType.Delimiter, "--"))
			{
				TokenStream.PushPosition();
				GetReference();
				Instructions.Add(new Instruction(Opcode.Push, 1));
				Instructions.Add(new Instruction(Opcode.Subtract));
				TokenStream.PopPosition();
				TokenStream.PushPosition();
				SetReference();
				TokenStream.PopPosition();
				GetReference();
			}
		}
	}
}
