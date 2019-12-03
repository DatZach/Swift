using System;
using System.Collections.Generic;
using System.Linq;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Source
{
	internal partial class Compiler
	{
		private void Pass1()
		{
			foreach(Method method in currentClass.Methods.Where(m => m.Position != -1))
			{
				currentMethod = method;
				TokenStream.Position = method.Position;

				// Invoke initializer list if one was defined
				if ((currentClass.Type == ClassType.Entity || currentClass.Type == ClassType.State) &&
					method.Name == "Create" && IsMethod("__InitializerList"))
				{
					Instructions.Add(new Instruction(Opcode.GetThis));
					Instructions.Add(new Instruction(Opcode.ClassCall, "__InitializerList", 0));
				}

				foreach (string arg in currentMethod.Arguments.FastReverse())
					Instructions.Add(new Instruction(Opcode.SetVariable, arg));

				// Top level scope stack for arguments
				scopeStack.Push(currentMethod.Arguments);
				FunctionBlock();
				scopeStack.Pop();

				// Scope stack should already be empty, but just to make sure we explicitly clear it here
				if (scopeStack.Count != 0)
				{
					Console.WriteLine("INTERNAL WARN: Scope stack is not empty!");
					scopeStack.Clear();
				}
			}

			if (IsMethod("__InitializerList") && !IsMethod("Create"))
			{
				Method createMethod = new Method
				{
					Name = "Create"
				};

				createMethod.Instructions.Add(new Instruction(Opcode.GetThis));
				createMethod.Instructions.Add(new Instruction(Opcode.ClassCall, "__InitializerList", 0));

				currentClass.Methods.Add(createMethod);
			}

			// Clean up hooked methods and fields
			if (currentClass.Type == ClassType.Entity)
			{
				Class entityClass = engineClasses.First(c => c.Name == "entity");
				foreach (Method method in entityClass.Methods)
					currentClass.Methods.Remove(method);
			}
			else if (currentClass.Type == ClassType.State)
			{
				Class stateClass = engineClasses.First(c => c.Name == "state");
				foreach (Method method in stateClass.Methods)
					currentClass.Methods.Remove(method);
			}
		}

		private void FunctionBlock()
		{
			scopeStack.Push(new List<string>());

			TokenStream.Expect(TokenType.Delimiter, "{");
			while (!TokenStream.Accept(TokenType.Delimiter, "}"))
				BlockStatement();

			// Constructors should return themselves
			if (currentMethod.Name == IdentConstructor)
				Instructions.Add(new Instruction(Opcode.GetThis));

			Instructions.Add(new Instruction(Opcode.Return));

			scopeStack.Pop();
		}

		private void Block()
		{
			scopeStack.Push(new List<string>());

			if (TokenStream.Accept(TokenType.Delimiter, "{"))
			{
				while (!TokenStream.Accept(TokenType.Delimiter, "}"))
					BlockStatement();
			}
			else
				BlockStatement();

			scopeStack.Pop();
		}

		private void BlockStatement()
		{
			if (!IfStatement() && !WhileStatement() && !DoWhileStatement() && !ForeachStatement() &&
				!ForStatement() && !SwitchStatement() && !RepeatStatement() &&
				!ReturnStatement() && !BreakStatement() && !ContinueStatement() &&
				!Variable())
			{
				Assignment();
				TokenStream.Accept(TokenType.Delimiter, ";");
			}
		}

		private bool Variable()
		{
			if (TokenStream.Peek().Value != "var")
				return false;

			TokenStream.Expect("var");
			do
			{
				int identOffset = TokenStream.Position;

				string ident = TokenStream.ReadWord();
				if (IsVariableInScope(ident) || IsField(ident) || IsMethod(ident) || IsConstant(ident) || IsEnumeration(ident))
					throw new CompilerException("Redeclaration of variable '{0}' in method '{1}'.", ident, currentMethod.Name);

				if (Parser.IsReservedIdent(ident))
					throw new CompilerException("'{0}' is a reserved symbol.", ident);

				Variant value = ParseAnnotatedVariable();

				// This should be safe, if we don't have the variable in scope but it's already been created in a previous scope
				// then we don't want to readd it, just reference it (it will already be allocated on the stack).
				if (!IsVariable(ident))
					currentMethod.Variables.Add(ident, value);

				scopeStack.Peek().Add(ident);

				if (TokenStream.Accept(TokenType.Delimiter, "="))
				{
					TokenStream.Position = identOffset;
					Assignment();
				}
				else
				{
					Instructions.Add(new Instruction(Opcode.Push, value));
					Instructions.Add(new Instruction(Opcode.SetVariable, ident));
				}
			}
			while (TokenStream.Accept(TokenType.Delimiter, ","));

			TokenStream.Accept(TokenType.Delimiter, ";");

			return true;
		}
	}
}
