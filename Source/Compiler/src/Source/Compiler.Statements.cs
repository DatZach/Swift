using System.Linq;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Source
{
	internal partial class Compiler
	{
		private bool IfStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "if"))
				return false;

			Label labelElse = new Label(this);
			Label labelEnd = new Label(this);

			TokenStream.Expect(TokenType.Delimiter, "(");
			Assignment();
			TokenStream.Expect(TokenType.Delimiter, ")");

			Instructions.Add(new Instruction(Opcode.Push, 0));
			Instructions.Add(new Instruction(Opcode.CompareNotEqual));
			Instructions.Add(new Instruction(Opcode.IfFalse, 0));
			labelElse.PatchHere();

			Block();

			if (TokenStream.Accept(TokenType.Word, "else"))
			{
				Instructions.Add(new Instruction(Opcode.Jump, 0));
				labelEnd.PatchHere();
				labelElse.Mark();

				Block();
			}
			else
				labelElse.Mark();

			labelEnd.Mark();

			labelElse.Fix();
			labelEnd.Fix();

			return true;
		}

		private bool WhileStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "while"))
				return false;

			Label labelStart = new Label(this);
			Label labelEnd = new Label(this);
			continueStack.Push(labelStart);
			breakStack.Push(labelEnd);

			labelStart.Mark();
			TokenStream.Expect(TokenType.Delimiter, "(");
			Assignment();
			TokenStream.Expect(TokenType.Delimiter, ")");

			Instructions.Add(new Instruction(Opcode.Push, 1));
			Instructions.Add(new Instruction(Opcode.CompareEqual));
			Instructions.Add(new Instruction(Opcode.IfFalse, 0));
			labelEnd.PatchHere();

			Block();

			Instructions.Add(new Instruction(Opcode.Jump, 0));
			labelStart.PatchHere();
			labelEnd.Mark();

			labelStart.Fix();
			labelEnd.Fix();
			continueStack.Pop();
			breakStack.Pop();

			return true;

		}

		private bool DoWhileStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "do"))
				return false;

			Label labelStart = new Label(this);
			Label labelEnd = new Label(this);
			continueStack.Push(labelStart);
			breakStack.Push(labelEnd);
			
			labelStart.Mark();
			Block();

			TokenStream.Expect(TokenType.Word, "while");
			TokenStream.Expect(TokenType.Delimiter, "(");
			Assignment();
			TokenStream.Expect(TokenType.Delimiter, ")");

			Instructions.Add(new Instruction(Opcode.Push, 1));
			Instructions.Add(new Instruction(Opcode.CompareNotEqual));
			Instructions.Add(new Instruction(Opcode.IfFalse, 0));
			labelStart.PatchHere();
			labelEnd.Mark();

			TokenStream.Accept(TokenType.Delimiter, ";");

			labelStart.Fix();
			labelEnd.Fix();
			continueStack.Pop();
			breakStack.Pop();

			return true;
		}

		private bool ForStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "for"))
				return false;

			Label labelStart = new Label(this);
			Label labelEnd = new Label(this);
			Label labelContinue = new Label(this);
			continueStack.Push(labelContinue);
			breakStack.Push(labelEnd);

			TokenStream.Expect(TokenType.Delimiter, "(");
			if (!Variable())
				Assignment();

			TokenStream.Accept(TokenType.Delimiter, ";");

			labelStart.Mark();
			Assignment();
			Instructions.Add(new Instruction(Opcode.Push, 1));
			Instructions.Add(new Instruction(Opcode.CompareNotEqual));
			Instructions.Add(new Instruction(Opcode.IfTrue, 0));
			labelEnd.PatchHere();
			TokenStream.Accept(TokenType.Delimiter, ";");

			TokenStream.PushPosition();
			while (!TokenStream.Accept(TokenType.Delimiter, ")"))
				TokenStream.Read();

			Block();
			int endOfBlock = TokenStream.Position;
			TokenStream.PopPosition();
			labelContinue.Mark();
			Assignment();
			TokenStream.Position = endOfBlock;

			Instructions.Add(new Instruction(Opcode.Jump, 0));
			labelStart.PatchHere();
			labelEnd.Mark();

			labelContinue.Fix();
			labelStart.Fix();
			labelEnd.Fix();
			continueStack.Pop();
			breakStack.Pop();

			return true;
		}

		private bool ForeachStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "foreach"))
				return false;

			string counterIdent = CreateTemporaryVariable();
			Label startLabel = new Label(this);
			Label endLabel = new Label(this);
			continueStack.Push(startLabel);
			breakStack.Push(endLabel);

			TokenStream.Expect(TokenType.Delimiter, "(");

			int elementIdentIndex = currentMethod.Variables.Count;
			if (!Variable())
				throw new CompilerException("Malformed foreach statement.");

			if (currentMethod.Variables.Count > elementIdentIndex + 2)
				throw new CompilerException("Too many variables declared in foreach statement.");

			string elementIdent = currentMethod.Variables.ToList()[elementIdentIndex].Key;
			string indexIdent = currentMethod.Variables.Count > elementIdentIndex + 1
								? currentMethod.Variables.ToList()[elementIdentIndex + 1].Key
								: "";

			TokenStream.Expect("in");
			string listIdent = TokenStream.ReadWord();
			TokenStream.Expect(TokenType.Delimiter, ")");

			// Lots of generated code
			Instructions.Add(new Instruction(Opcode.Push, 0));
			Instructions.Add(new Instruction(Opcode.SetVariable, counterIdent));

			startLabel.Mark();

			Instructions.Add(new Instruction(Opcode.GetVariable, counterIdent));
			if (!string.IsNullOrEmpty(indexIdent))
			{
				Instructions.Add(new Instruction(Opcode.Duplicate));
				Instructions.Add(new Instruction(Opcode.SetVariable, indexIdent));
			}

			Instructions.Add(new Instruction(Opcode.GetVariable, listIdent));
			Instructions.Add(new Instruction(Opcode.GetVariableLength));
			Instructions.Add(new Instruction(Opcode.CompareEqual));
			Instructions.Add(new Instruction(Opcode.IfTrue, 0));
			endLabel.PatchHere();

			Instructions.Add(new Instruction(Opcode.GetVariable, listIdent));
			Instructions.Add(new Instruction(Opcode.GetVariable, counterIdent));
			Instructions.Add(new Instruction(Opcode.GetIndex));
			Instructions.Add(new Instruction(Opcode.SetVariable, elementIdent));
			Instructions.Add(new Instruction(Opcode.IncrementVariable, counterIdent, 1));

			Block();

			Instructions.Add(new Instruction(Opcode.Jump, 0));
			startLabel.PatchHere();
			endLabel.Mark();

			startLabel.Fix();
			endLabel.Fix();
			breakStack.Pop();
			continueStack.Pop();

			return true;
		}

		private bool SwitchStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "switch"))
				return false;

			Label labelCase = new Label(this);
			Label labelEnd = new Label(this);
			Label labelNextCase = new Label(this);
			breakStack.Push(labelEnd);

			string valueIdent = CreateTemporaryVariable();

			// Parse switch selector
			TokenStream.Expect(TokenType.Delimiter, "(");
			Ternary();
			TokenStream.Expect(TokenType.Delimiter, ")");

			Instructions.Add(new Instruction(Opcode.SetVariable, valueIdent));

			TokenStream.Expect(TokenType.Delimiter, "{");

			while(!TokenStream.Accept(TokenType.Delimiter, "}"))
			{
				if (TokenStream.Accept(TokenType.Word, "case"))
				{
					Instructions.Add(new Instruction(Opcode.GetVariable, valueIdent));
					Ternary();
					Instructions.Add(new Instruction(Opcode.CompareEqual));
					Instructions.Add(new Instruction(Opcode.IfFalse, 0));
					labelCase.PatchHere();

					labelNextCase.Mark();
					labelNextCase.Fix();
					labelNextCase.ClearPatches();

					TokenStream.Expect(TokenType.Delimiter, ":");

					bool breakStatementPresent = false;
					if (!TokenStream.Pass("case"))
					{
						while (!(breakStatementPresent = BreakStatement()) && !TokenStream.Pass("case"))
							Block();
					}

					if (!breakStatementPresent)
					{
						Instructions.Add(new Instruction(Opcode.Jump, 0));
						labelNextCase.PatchHere();
					}

					labelCase.Mark();
					labelCase.Fix();
					labelCase = new Label(this);
				}
				else if (TokenStream.Accept(TokenType.Word, "default"))
				{
					TokenStream.Expect(TokenType.Delimiter, ":");

					while (!BreakStatement() && !TokenStream.Pass("case"))
						Block();

					if (TokenStream.Pass("case"))
						throw new CompilerException("Default case must be the last case in a switch statement.");
				}
				else
					throw new CompilerException("Expected a case or default switch.");
			}

			labelEnd.Mark();
			labelEnd.Fix();

			return true;
		}

		private bool RepeatStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "repeat"))
				return false;

			string counterIdent = CreateTemporaryVariable();

			Label startLabel = new Label(this);
			Label endLabel = new Label(this);

			TokenStream.Expect(TokenType.Delimiter, "(");
			Assignment();
			TokenStream.Expect(TokenType.Delimiter, ")");
			
			Instructions.Add(new Instruction(Opcode.SetVariable, counterIdent));
			startLabel.Mark();
			Instructions.Add(new Instruction(Opcode.GetVariable, counterIdent));
			Instructions.Add(new Instruction(Opcode.Push, 0));
			Instructions.Add(new Instruction(Opcode.CompareEqual));
			Instructions.Add(new Instruction(Opcode.IfTrue, 0));
			endLabel.PatchHere();

			Block();

			Instructions.Add(new Instruction(Opcode.GetVariable, counterIdent));
			Instructions.Add(new Instruction(Opcode.Push, 1));
			Instructions.Add(new Instruction(Opcode.Subtract));
			Instructions.Add(new Instruction(Opcode.SetVariable, counterIdent));
			Instructions.Add(new Instruction(Opcode.Jump, 0));
			startLabel.PatchHere();
			endLabel.Mark();

			startLabel.Fix();
			endLabel.Fix();

			return true;
		}

		private bool ReturnStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "return"))
				return false;

			if (IsReference(TokenStream.Peek()) || TokenStream.Pass(TokenType.Number) || TokenStream.Pass(TokenType.String) ||
				(TokenStream.Pass(TokenType.Delimiter) && !TokenStream.Accept(TokenType.Delimiter, ";")))
			{
				if (currentMethod.Name == IdentConstructor)
					throw new CompilerException("Constructors must return a 'this' pointer.");

				Ternary();
			}
			else if (currentMethod.Name == IdentConstructor)
				Instructions.Add(new Instruction(Opcode.GetThis));

			Instructions.Add(new Instruction(Opcode.Return));

			TokenStream.Accept(TokenType.Delimiter, ";");

			return true;
		}

		private bool BreakStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "break"))
				return false;

			if (breakStack.Count == 0)
				throw new CompilerException("Nothing to break from.");

			if (TokenStream.Pass(TokenType.Number))
			{
				long count = TokenStream.ReadVariant().IntValue;
				if (breakStack.Count < count)
					throw new CompilerException("Cannot break from {0} scopes deep, because we are not {0} scopes deep at this time.", count);

				Instructions.Add(new Instruction(Opcode.Jump, 0));
				Label[] labelArray = breakStack.ToArray();
				labelArray[count - 1].PatchHere();
			}
			else
			{
				Instructions.Add(new Instruction(Opcode.Jump, 0));
				breakStack.Peek().PatchHere();
			}

			TokenStream.Accept(TokenType.Delimiter, ";");

			return true;
		}

		private bool ContinueStatement()
		{
			if (!TokenStream.Accept(TokenType.Word, "continue"))
				return false;

			if (continueStack.Count == 0)
				throw new CompilerException("Nothing to continue from.");

			Instructions.Add(new Instruction(Opcode.Jump, 0));
			continueStack.Peek().PatchHere();

			TokenStream.Accept(TokenType.Delimiter, ";");

			return true;
		}
	}
}
