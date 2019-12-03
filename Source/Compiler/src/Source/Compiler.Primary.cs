using System.Collections.Generic;
using System.Linq;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Source
{
	internal partial class Compiler
	{
		private void Primary()
		{
			if (TokenStream.Accept(TokenType.Delimiter, "("))
			{
				Ternary();
				TokenStream.Expect(TokenType.Delimiter, ")");
			}
			else if (IsClass(TokenStream.Peek()) && (TokenStream.Peek(1).Value == "(" || TokenStream.Peek(1).Value == "."))
			{
				// Above line is to avoid confusing variables with class identifiers
				Class parentClass = GetClass(TokenStream.ReadWord());

				if (TokenStream.Accept(TokenType.Delimiter, "."))
				{
					// Evaluate potential enum, const reference or static call
					if (!EnumReference(parentClass) && !ConstReference(parentClass) && !StaticCall(parentClass) &&
						!StaticField(parentClass) && !StaticProperty(parentClass))
						throw new CompilerException("'{0}' has no member '{1}'.", parentClass.Name, TokenStream.ReadWord());
				}
				else
					NewObject(parentClass);
			}
			else if (IsReference(TokenStream.Peek()))
				PrimaryReference();
			else if (IsMethod(TokenStream.Peek()))
				MethodCall(currentClass, new Instruction(Opcode.GetThis));
			else if (IsEnumeration(TokenStream.Peek()))
				EnumReference(currentClass);
			else if (IsConstant(TokenStream.Peek()))
				ConstReference(currentClass);
			else if (TokenStream.Accept(TokenType.Word, "typeof"))
			{
				TokenStream.Expect(TokenType.Delimiter, "(");
				Ternary();
				TokenStream.Expect(TokenType.Delimiter, ")");

				Instructions.Add(new Instruction(Opcode.TypeOf));
			}
			else if (TokenStream.Accept(TokenType.Word, "len"))
			{
				TokenStream.Expect(TokenType.Delimiter, "(");
				Ternary();
				TokenStream.Expect(TokenType.Delimiter, ")");

				Instructions.Add(new Instruction(Opcode.GetVariableLength));
			}
			else
				Literal();
		}

		private void Literal()
		{
			if (TokenStream.Accept(TokenType.Delimiter, "["))
			{
				string listIdent = CreateTemporaryVariable();
				Instructions.Add(new Instruction(Opcode.Push, new List<Variant>()));
				Instructions.Add(new Instruction(Opcode.SetVariable, listIdent));

				if (!TokenStream.Accept(TokenType.Delimiter, "]"))
				{
					do
					{
						if (TokenStream.Peek(1).Value == "..")
						{
							Variant rangeStart = TokenStream.ReadVariant();
							TokenStream.Expect(TokenType.Delimiter, "..");
							Variant rangeEnd = TokenStream.ReadVariant();

							if (rangeStart.Type != VariantType.Int64 || rangeEnd.Type != VariantType.Int64)
								throw new CompilerException("Range operator expects start and end values to be integers.");

							for (long i = rangeStart.IntValue; i <= rangeEnd.IntValue; ++i)
							{
								Instructions.Add(new Instruction(Opcode.GetVariable, listIdent));
								Instructions.Add(new Instruction(Opcode.Push, i));
								Instructions.Add(new Instruction(Opcode.Add));
								Instructions.Add(new Instruction(Opcode.SetVariable, listIdent));
							}
						}
						else
						{
							Instructions.Add(new Instruction(Opcode.GetVariable, listIdent));
							Ternary();
							Instructions.Add(new Instruction(Opcode.Add));
							Instructions.Add(new Instruction(Opcode.SetVariable, listIdent));
						}
					}
					while (TokenStream.Accept(TokenType.Delimiter, ","));

					TokenStream.Expect(TokenType.Delimiter, "]");
				}

				Instructions.Add(new Instruction(Opcode.GetVariable, listIdent));

				typeStack.Push("__list");

				// I like bad hacks that's my fucking problem
				if (TokenStream.Accept(TokenType.Delimiter, "."))
					ListMethod();
				else if (TokenStream.Pass(TokenType.Delimiter, "["))
				{
					while (TokenStream.Accept(TokenType.Delimiter, "["))
					{
						Ternary();
						TokenStream.Expect(TokenType.Delimiter, "]");

						Instructions.Add(new Instruction(Opcode.GetIndex));
					}
				}
			}
			else
			{
				Variant value = TokenStream.ReadVariant();
				Instructions.Add(new Instruction(Opcode.Push, value));

				if (value.Type == VariantType.String)
				{
					typeStack.Push("__str");

					// I like bad hacks that's my fucking problem
					if (TokenStream.Accept(TokenType.Delimiter, "."))
						StringMethod();
				}
			}
		}

		private void PrimaryReference()
		{
			int refIndex = TokenStream.Position;
			GetReference();

			if (TokenStream.Accept(TokenType.Delimiter, "++"))
			{
				TokenStream.Position = refIndex;
				GetReference();
				Instructions.Add(new Instruction(Opcode.Push, 1));
				Instructions.Add(new Instruction(Opcode.Add));
				TokenStream.Position = refIndex;
				SetReference();

				TokenStream.Read();
			}
			else if (TokenStream.Accept(TokenType.Delimiter, "--"))
			{
				TokenStream.Position = refIndex;
				GetReference();
				Instructions.Add(new Instruction(Opcode.Push, 1));
				Instructions.Add(new Instruction(Opcode.Subtract));
				TokenStream.Position = refIndex;
				SetReference();

				TokenStream.Read();
			}
		}

		private void GetReference()
		{
			Variant finalReference = null;
			string rootIdent = TokenStream.ReadWord();

			// Get root ident
			if (rootIdent == "this")
				Instructions.Add(new Instruction(Opcode.GetThis));
			else
			{
				if (IsVariableInScope(rootIdent))
				{
					Instructions.Add(new Instruction(Opcode.GetVariable, rootIdent));
					finalReference = currentMethod.Variables[rootIdent];
				}
				else if (IsProperty(rootIdent))
				{
					Method getAccessor = GetMethod("__get_" + rootIdent, currentClass);
					if (getAccessor == null)
						throw new CompilerException("Property '{0}' has no get accessor.", rootIdent);

					Instructions.Add(new Instruction(Opcode.GetThis));
					Instructions.Add(new Instruction(Opcode.ClassCall, "__get_" + rootIdent, 0));
				}
				else if (IsField(rootIdent))
				{
					// Since we're at root the only class we could be referencing is 'this'
					Instructions.Add(new Instruction(Opcode.GetThis));
					Instructions.Add(new Instruction(Opcode.ClassGetField, rootIdent));
					finalReference = GetField(rootIdent, currentClass).Value;
				}
				else if (IsClass(rootIdent))
				{
					TokenStream.Expect(TokenType.Delimiter, ".");
					string fieldIdent = TokenStream.ReadWord();

					Instructions.Add(new Instruction(Opcode.ClassGetFieldStatic, rootIdent + " " + fieldIdent));
				}
				else
					throw new CompilerException("Internal Error: Cannot evaluate get-ref which should be valid.");
			}

			// Parse descending references
			while(TokenStream.Accept(TokenType.Delimiter, "."))
			{
				// At this point everything is going to either be a field or a call
				//  both of which are descendants of the base ident. So we evaluate
				//  the class of the base ident here.
				string baseClassIdent = "";
				if (IsVariableInScope(rootIdent))
					baseClassIdent = currentMethod.Variables[rootIdent].ObjectName;
				else if (IsField(rootIdent))
					baseClassIdent = GetField(rootIdent, currentClass).Value.ObjectName;
				else if (IsProperty(rootIdent))
					baseClassIdent = GetProperty(rootIdent, currentClass).Value.ObjectName;

				Class baseClass = GetClass(baseClassIdent);
				if (baseClass == null)
					throw new CompilerException("Cannot evaluate object reference in variable '{0}'.", rootIdent);

				string descendentIdent = TokenStream.Peek().Value;

				if (IsProperty(descendentIdent, baseClass))
				{
					Method getAccessor = GetMethod("__get_" + descendentIdent, baseClass);
					if (getAccessor == null)
						throw new CompilerException("Property '{0}' has no get accessor.", descendentIdent);

					if (getAccessor.Visibility != AccessVisibility.Public && baseClass != currentClass)
						throw new CompilerException("Property '{0}'s get accessor is not visible here.", descendentIdent);

					Instructions.Add(new Instruction(Opcode.ClassCall, "__get_" + TokenStream.ReadWord(), 0));
				}
				else if (IsField(descendentIdent, baseClass))
				{
					string fieldIdent = TokenStream.ReadWord();
					Instructions.Add(new Instruction(Opcode.ClassGetField, fieldIdent));
					finalReference = GetField(fieldIdent, baseClass).Value;
				}
				else if (IsMethod(descendentIdent, baseClass))
				{
					string tmpVarIdent = CreateTemporaryVariable();
					if (baseClassIdent == "__str")
						StringMethod();
					else if (baseClassIdent == "__list")
						ListMethod();
					else
					{
						Instructions.Add(new Instruction(Opcode.SetVariable, tmpVarIdent));
						MethodCall(baseClass, new Instruction(Opcode.GetVariable, tmpVarIdent));
					}
					break;
				}
				else
					throw new CompilerException("Identifier '{0}' is not a field, property or method of '{1}'.",
												descendentIdent, baseClassIdent);
			}

			if (TokenStream.Pass(TokenType.Delimiter, "(") && (object)finalReference != null)
			{
				Method method = GetMethod("call", GetClass(finalReference.ObjectName));
				if (method == null)
					throw new CompilerException("Cannot class call because class '{0}' has no call method.",
												finalReference.ObjectName);

				string tmpIdent = CreateTemporaryVariable();
				Instructions.Add(new Instruction(Opcode.SetVariable, tmpIdent));

				int argCount = ArgumentList();
				method = GetMethod("call", argCount, GetClass(finalReference.ObjectName));
				if (method == null)
					throw new CompilerException("No overload of method 'call' accepts {0} arguments.", argCount);

				Instructions.Add(new Instruction(Opcode.GetVariable, tmpIdent));
				Instructions.Add(new Instruction(Opcode.ClassCall, "call", argCount));
			}

			// Indexer if stated
			while (TokenStream.Accept(TokenType.Delimiter, "["))
			{
				Ternary();
				TokenStream.Expect(TokenType.Delimiter, "]");

				Instructions.Add(new Instruction(Opcode.GetIndex));
			}

			// If we have a resolvable object type for this reference push it onto the stack
			if ((object)finalReference != null && !string.IsNullOrEmpty(finalReference.ObjectName))
				typeStack.Push(finalReference.ObjectName);
		}

		private void SetReference()
		{
			string rootIdent = TokenStream.ReadWord();

			// Get root ident
			if (!TokenStream.Pass(TokenType.Delimiter, "."))
			{
				// There is no descending reference so we just set it at this point
				if (rootIdent == "this")
					Instructions.Add(new Instruction(Opcode.GetThis));
				else
				{
					if (IsVariableInScope(rootIdent))
						Instructions.Add(new Instruction(Opcode.SetVariable, rootIdent));
					else if (IsProperty(rootIdent))
					{
						Method setAccessor = GetMethod("__set_" + rootIdent, currentClass);
						if (setAccessor == null)
							throw new CompilerException("Property '{0}' has no set accessor.", rootIdent);

						Instructions.Add(new Instruction(Opcode.GetThis));
						Instructions.Add(new Instruction(Opcode.ClassCall, "__set_" + rootIdent, 1));
					}
					else if (IsField(rootIdent))
					{
						// Since we're at root the only class we could be referencing is 'this'
						Instructions.Add(new Instruction(Opcode.GetThis));
						Instructions.Add(new Instruction(Opcode.ClassSetField, rootIdent));
					}
					else
					{
						if (IsVariable(rootIdent))
							throw new CompilerException("Identifier '{0}' is out of scope. Please redeclare using 'var'.", rootIdent);

						throw new CompilerException("Identifier '{0}' cannot be assigned to.", rootIdent);
					}
				}
			}
			else
			{
				// There is a descending references. GET the value from this ident
				if (IsVariableInScope(rootIdent))
					Instructions.Add(new Instruction(Opcode.GetVariable, rootIdent));
				else if (IsProperty(rootIdent))
				{
					Method getAccessor = GetMethod("__get_" + rootIdent, currentClass);
					if (getAccessor == null)
						throw new CompilerException("Property '{0}' has no get accessor.", rootIdent);

					Instructions.Add(new Instruction(Opcode.GetThis));
					Instructions.Add(new Instruction(Opcode.ClassCall, "__get_" + rootIdent, 0));
				}
				else if (IsField(rootIdent))
				{
					// Since we're at root the only class we could be referencing is 'this'
					Instructions.Add(new Instruction(Opcode.GetThis));
					Instructions.Add(new Instruction(Opcode.ClassGetField, rootIdent));
				}
				else if (IsClass(rootIdent))
				{
					TokenStream.Expect(TokenType.Delimiter, ".");
					string fieldIdent = TokenStream.ReadWord();

					Instructions.Add(TokenStream.Pass(TokenType.Delimiter, ".")
						                 ? new Instruction(Opcode.ClassGetFieldStatic, rootIdent + " " + fieldIdent)
						                 : new Instruction(Opcode.ClassSetFieldStatic, rootIdent + " " + fieldIdent));
				}
				else
					throw new CompilerException("Internal Error: Cannot evaluate set-ref which should be valid.");

				// Parse descending references
				while(TokenStream.Accept(TokenType.Delimiter, "."))
				{
					// At this point everything is going to either be a field or a call
					//  both of which are descendants of the base ident. So we evaluate
					//  the class of the base ident here.
					string baseClassIdent = "";
					if (IsVariableInScope(rootIdent))
						baseClassIdent = currentMethod.Variables[rootIdent].ObjectName;
					else if (IsField(rootIdent))
						baseClassIdent = GetField(rootIdent, currentClass).Value.ObjectName;
					else if (IsProperty(rootIdent))
						baseClassIdent = GetProperty(rootIdent, currentClass).Value.ObjectName;

					Class baseClass = GetClass(baseClassIdent);
					if (baseClass == null)
						throw new CompilerException("Cannot evaluate object reference in variable '{0}'.", rootIdent);

					string descendentIdent = TokenStream.ReadWord();

					if (IsProperty(descendentIdent, baseClass))
					{
						if (TokenStream.Pass(TokenType.Delimiter, "."))
						{
							Method getAccessor = GetMethod("__get_" + descendentIdent, baseClass);
							if (getAccessor == null)
								throw new CompilerException("Property '{0}' has no get accessor.", descendentIdent);

							if (getAccessor.Visibility != AccessVisibility.Public && baseClass != currentClass)
								throw new CompilerException("Property '{0}'s get accessor is not visible here.", descendentIdent);

							Instructions.Add(new Instruction(Opcode.ClassCall, "__get_" + descendentIdent, 0));
						}
						else
						{
							Method setAccessor = GetMethod("__set_" + descendentIdent, baseClass);
							if (setAccessor == null)
								throw new CompilerException("Property '{0}' has no set accessor.", descendentIdent);

							if (setAccessor.Visibility != AccessVisibility.Public && baseClass != currentClass)
								throw new CompilerException("Property '{0}'s set accessor is not visible here.", descendentIdent);

							Instructions.Add(new Instruction(Opcode.ClassCall, "__set_" + descendentIdent, 1));
						}
					}
					else if (IsField(descendentIdent, baseClass))
					{
						// If we're going to continue to descend get the next field
						Instructions.Add(TokenStream.Pass(TokenType.Delimiter, ".")
											 ? new Instruction(Opcode.ClassGetField, descendentIdent)
											 : new Instruction(Opcode.ClassSetField, descendentIdent));
					}
					else
						throw new CompilerException("Identifier '{0}' is not a field or property of '{1}'.",
							descendentIdent, baseClassIdent);
				}
			}

			while (TokenStream.Accept(TokenType.Delimiter, "["))
			{
				// NOTE For simplicity in the face of an overly complex problem, we catch this after prior
				//		expression parsing. It should be safe to rewrite instructions at this point in time.
				//		Rewrite last instruction to GET instead of SET
				Instruction lastInstruction = Instructions.Last();
				Instructions.Remove(Instructions.Last());
				if (lastInstruction.Opcode == Opcode.SetVariable)
					Instructions.Add(new Instruction(Opcode.GetVariable, lastInstruction.Operand));
				else if (lastInstruction.Opcode == Opcode.ClassSetField)
					Instructions.Add(new Instruction(Opcode.ClassSetField, lastInstruction.Operand));
				else if (lastInstruction.Opcode == Opcode.SetIndex)
					Instructions.Add(new Instruction(Opcode.GetIndex));
				else
					throw new CompilerException("INTERNAL ERROR: Unsure how to rewrite instruction {0}.", lastInstruction.Opcode);

				Ternary();
				TokenStream.Expect(TokenType.Delimiter, "]");

				Instructions.Add(new Instruction(Opcode.SetIndex));
			}
		}

		private bool ConstReference(Class parent)
		{
			// Make sure this is a constant
			string ident = TokenStream.Peek().Value;
			if (!IsConstant(ident, parent) || !TokenStream.Pass(ident))
				return false;

			TokenStream.ReadWord();

			// Check visibility
			Field constant = parent.Constants.First(c => c.Name == ident);
			if (constant.Visibility != AccessVisibility.Public && parent != currentClass)
				throw new CompilerException("'{0}.{1}' is not visible here.", parent.Name, ident);

			// Evaluate constants inline
			Instructions.Add(new Instruction(Opcode.Push, constant.Value));

			return true;
		}

		private bool EnumReference(Class parent)
		{
			// Make sure this is an enum
			string enumIdent = TokenStream.Peek().Value;
			if (!IsEnumeration(enumIdent, parent) || !TokenStream.Pass(enumIdent))
				return false;

			TokenStream.ReadWord();

			// Check visibility
			Enumeration enumeration = parent.Enums.First(e => e.Name == enumIdent);
			if (enumeration.Visibility != AccessVisibility.Public && parent != currentClass)
				throw new CompilerException("'{0}.{1}' is not visible here.", parent.Name, enumIdent);

			// Read member
			TokenStream.Expect(TokenType.Delimiter, ".");
			string memberIdent = TokenStream.ReadWord();

			// Check the member exists
			if (!enumeration.Values.ContainsKey(memberIdent))
				throw new CompilerException("'{0}' is not a member of enumeration \'{1}.{2}'.",
											memberIdent, parent.Name, enumIdent);

			// Evaluate enums inline
			Instructions.Add(new Instruction(Opcode.Push, enumeration.Values[memberIdent]));

			return true;
		}

		private void MethodCall(Class parent, Instruction refGetter)
		{
			string methodIdent = TokenStream.ReadWord();

			// Parse arguments
			int argumentsPassed = ArgumentList();

			// Check visibility
			Method method = GetMethod(methodIdent, argumentsPassed, parent);
			if (GetMethod(methodIdent, parent) == null)
				throw new CompilerException("Method '{0}' is not a member of class '{1}'.", methodIdent, parent.Name);
			
			if (method == null)
				throw new CompilerException("No overload of method '{0}' in class '{1}' accepts {2} arguments.",
											methodIdent, parent.Name, argumentsPassed);

			if (method.Visibility != AccessVisibility.Public && parent != currentClass)
				throw new CompilerException("'{0}.{1}' is not visible here.", parent.Name, methodIdent);

			// Evaluate call
			Instructions.Add(refGetter);

			Instructions.Add(new Instruction(Opcode.ClassCall, methodIdent, argumentsPassed));
		}

		private bool StaticCall(Class parent)
		{
			// Make sure this is a static method
			string methodIdent = TokenStream.Peek().Value;
			if (!IsMethod(methodIdent, parent) || !TokenStream.Pass(methodIdent))
				return false;

			TokenStream.ReadWord();

			// Parse arguments
			int argumentsPassed = ArgumentList();

			// Check visibility
			Method method = GetMethod(methodIdent, argumentsPassed, parent);
			if (GetMethod(methodIdent, parent) == null)
				throw new CompilerException("Method '{0}' is not a member of class '{1}'.", methodIdent, parent.Name);

			if (method == null)
				throw new CompilerException("No overload of method '{0}' in class '{1}' accepts {2} arguments.",
											methodIdent, parent.Name, argumentsPassed);

			// Make sure it's static
			if (!method.Static)
				throw new CompilerException("'{0}.{1}' is not a static method.", parent.Name, methodIdent);

			// Evaluate call
			Instructions.Add(new Instruction(Opcode.ClassCallStatic, parent.Name + " " + methodIdent, argumentsPassed));

			return true;
		}

		private bool StaticField(Class parent)
		{
			// Make sure this is a static field
			string fieldIdent = TokenStream.Peek().Value;
			if (!IsStaticField(fieldIdent, parent) || !TokenStream.Pass(fieldIdent))
				return false;

			TokenStream.ReadWord();

			// Check if it's a static field
			Field field = GetField(fieldIdent, parent);
			if (!field.Static)
				throw new CompilerException("Field '{0}' is not a static member of class '{1}'.", fieldIdent, parent.Name);

			// Check visibility
			if (field.Visibility != AccessVisibility.Public && parent != currentClass)
				throw new CompilerException("'{0}.{1}' is not visible here.", parent.Name, fieldIdent);

			// Evaluate get
			Instructions.Add(new Instruction(Opcode.ClassGetFieldStatic, parent.Name + " " + fieldIdent));

			return true;
		}

		private bool StaticProperty(Class parent)
		{
			// Make sure this is a static property
			string propertyIdent = TokenStream.Peek().Value;
			if (!IsStaticProperty(propertyIdent, parent) || !TokenStream.Pass(propertyIdent))
				return false;

			TokenStream.ReadWord();

			Method method = GetMethod("__get_" + propertyIdent, parent);
			if (method == null)
				throw new CompilerException("Property '{0}' has no get accessor.", propertyIdent);

			if (!method.Static)
				throw new CompilerException("Property '{0}' is not a static member of class '{1}'.", propertyIdent, parent.Name);

			if (method.Visibility != AccessVisibility.Public && parent != currentClass)
				throw new CompilerException("Property '{0}' is not visible here.", propertyIdent);

			// Evaluate get
			Instructions.Add(new Instruction(Opcode.ClassCallStatic, parent.Name + " " + method.Name, 0));

			return true;
		}

		private int ArgumentList()
		{
			int passedArguments = 0;

			TokenStream.Expect(TokenType.Delimiter, "(");
			if (!TokenStream.Accept(TokenType.Delimiter, ")"))
			{
				do
				{
					Ternary();
					++passedArguments;
				}
				while (TokenStream.Accept(TokenType.Delimiter, ","));

				TokenStream.Expect(TokenType.Delimiter, ")");
			}

			return passedArguments;
		}

		private void NewObject(Class parent)
		{
			// Call constructor if there is one
			if (IsMethod(IdentConstructor, parent))
			{
				// Parse arguments
				int argumentsPassed = ArgumentList();

				Method method = GetMethod(IdentConstructor, argumentsPassed, parent);

				if (GetMethod(IdentConstructor, parent) == null)
					throw new CompilerException("Class '{0}' has no constructor.",  parent.Name);

				if (method == null)
					throw new CompilerException("No overload of class '{0}''s constructor accepts {1} arguments.",
												parent.Name, argumentsPassed);

				// Create new object and call constructor
				Instructions.Add(new Instruction(Opcode.New, parent.Name));
				Instructions.Add(new Instruction(Opcode.ClassCall, IdentConstructor, argumentsPassed));
			}
			else
			{
				// Create new object
				Instructions.Add(new Instruction(Opcode.New, parent.Name));

				// No constructor, read useless argument list
				TokenStream.Expect(TokenType.Delimiter, "(");
				TokenStream.Expect(TokenType.Delimiter, ")");
			}

			// Push this object type onto the type stack so the reference can be evaluated later
			typeStack.Push(parent.Name);

			// Parse out inline method calls
			if (TokenStream.Accept(TokenType.Delimiter, "."))
			{
				string tmpVarIdent = CreateTemporaryVariable();
				Instructions.Add(new Instruction(Opcode.SetVariable, tmpVarIdent));
				MethodCall(parent, new Instruction(Opcode.GetVariable, tmpVarIdent));
			}
		}

		private void StringMethod()
		{
			Class parent = GetClass("__str");

			// Make sure this is a static method
			string methodIdent = TokenStream.Peek().Value;
			if (!IsMethod(methodIdent, parent) || !TokenStream.Pass(methodIdent))
				return;

			TokenStream.ReadWord();

			// Parse arguments -- Add one to account for the "this" being passed implicity by compiler
			int argumentsPassed = ArgumentList() + 1;

			// Check visibility
			Method method = GetMethod(methodIdent, argumentsPassed, parent);
			if (GetMethod(methodIdent, parent) == null)
				throw new CompilerException("Method '{0}' is not a member of class '{1}'.", methodIdent, parent.Name);

			if (method == null)
				throw new CompilerException("No overload of method '{0}' in class '{1}' accepts {2} arguments.",
											methodIdent, parent.Name, argumentsPassed);

			// Make sure it's static
			if (!method.Static)
				throw new CompilerException("'{0}.{1}' is not a static method.", parent.Name, methodIdent);

			// Evaluate call
			Instructions.Add(new Instruction(Opcode.ClassCallStatic, parent.Name + " " + methodIdent, argumentsPassed));
		}

		private void ListMethod()
		{
			Class parent = GetClass("__list");

			// Make sure this is a static method
			string methodIdent = TokenStream.Peek().Value;
			if (!IsMethod(methodIdent, parent) || !TokenStream.Pass(methodIdent))
				return;

			TokenStream.ReadWord();

			// Parse arguments -- Add one to account for the "this" being passed implicity by compiler
			int argumentsPassed = ArgumentList() + 1;

			// Check visibility
			Method method = GetMethod(methodIdent, argumentsPassed, parent);
			if (GetMethod(methodIdent, parent) == null)
				throw new CompilerException("Method '{0}' is not a member of class '{1}'.", methodIdent, parent.Name);

			if (method == null)
				throw new CompilerException("No overload of method '{0}' in class '{1}' accepts {2} arguments.",
											methodIdent, parent.Name, argumentsPassed);

			// Make sure it's static
			if (!method.Static)
				throw new CompilerException("'{0}.{1}' is not a static method.", parent.Name, methodIdent);

			// Evaluate call
			Instructions.Add(new Instruction(Opcode.ClassCallStatic, parent.Name + " " + methodIdent, argumentsPassed));
		}
	}
}
