using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Source
{
	internal partial class Compiler
	{
		private void Pass0()
		{
			Dictionary<string, ClassType> classTypes = new Dictionary<string, ClassType>
			{
				{ "class", ClassType.Class },
				{ "entity", ClassType.Entity },
				{ "state", ClassType.State },
				{ "global", ClassType.Global }
			};

			if (TokenStream.EndOfStream)
			{
				Console.WriteLine("Warning: Script '{0}' is empty.", Path.GetFileName(TokenStream.Filename));
				return;
			}

			string classIdent = TokenStream.ReadWord();
			if (!classTypes.ContainsKey(classIdent))
				TokenStream.Expect("class, entity, state or global");

			string className = TokenStream.ReadWord();
			if (IsClass(className))
				throw new CompilerException("Redeclaration of class '{0}'.", className);

			Classes.Add(new Class
			{
				Name = className,
				Type = classTypes[classIdent],
				Stream = TokenStream
			});

			currentClass = Classes.Last();

			if (Parser.IsReservedIdent(currentClass.Name))
				throw new CompilerException("'{0}' is a reserved symbol.", currentClass.Name);

			if (currentClass.Type == ClassType.Entity)
			{
				Class entityClass = engineClasses.First(c => c.Name == "entity");
				currentClass.Fields.AddRange(entityClass.Fields);
				currentClass.Methods.AddRange(entityClass.Methods);
			}
			else if (currentClass.Type == ClassType.State)
			{
				Class stateClass = engineClasses.First(c => c.Name == "state");
				currentClass.Fields.AddRange(stateClass.Fields);
				currentClass.Methods.AddRange(stateClass.Methods);
			}

			if (TokenStream.Accept(TokenType.Delimiter, "{") &&
				(currentClass.Type == ClassType.Entity || currentClass.Type == ClassType.State))
			{
				currentClass.Methods.Add(new Method
				{
					Visibility = AccessVisibility.Private,
					Name = "__InitializerList"
				});

				currentMethod = currentClass.Methods.Last();

				if (!TokenStream.Pass(TokenType.Delimiter, "}"))
				{
					do
						Assignment();
					while (TokenStream.Accept(TokenType.Delimiter, ","));
				}

				Instructions.Add(new Instruction(Opcode.Return));
				TokenStream.Expect(TokenType.Delimiter, "}");
			}

			TokenStream.Accept(TokenType.Delimiter, ";");

			while (!TokenStream.EndOfStream)
			{
				if (!Field() && !Property() && !Method() && !Enum() && !Const())
					TokenStream.Expected("field, method, enum or const declaration");
			}
		}

		private bool Field()
		{
			int peekIndex = 0;
			if (Parser.IsVisibility(TokenStream.Peek(peekIndex)))
				++peekIndex;

			if (TokenStream.Peek(peekIndex).Value == "static")
				++peekIndex;

			if (TokenStream.Peek(peekIndex).Value != "var")
				return false;

			AccessVisibility visibility = GetVisibility();
			bool isStatic = TokenStream.Accept(TokenType.Word, "static");
			TokenStream.Expect("var");

			do
			{
				string ident = TokenStream.ReadWord();
				if (IsField(ident) || IsMethod(ident) || IsProperty(ident) || IsConstant(ident) || IsEnumeration(ident))
					throw new CompilerException("Redeclaration of field '{0}'.", ident);

				if (Parser.IsReservedIdent(ident))
					throw new CompilerException("'{0}' is a reserved symbol.", ident);

				Variant value = ParseAnnotatedVariable();

				if (TokenStream.Accept(TokenType.Delimiter, "="))
					value = TokenStream.ReadVariant();

				if (value.Type == VariantType.List)
					value.ObjectName = "__list";

				currentClass.Fields.Add(new Field
				{
					Name = ident,
					Visibility = visibility,
					Value = value,
					Static = isStatic
				});
			}
			while (TokenStream.Accept(TokenType.Delimiter, ","));

			TokenStream.Accept(TokenType.Delimiter, ";");

			return true;
		}

		private bool Method()
		{
			int peekIndex = 0;
			if (Parser.IsVisibility(TokenStream.Peek(peekIndex)))
				++peekIndex;

			if (TokenStream.Peek(peekIndex).Value == "static")
				++peekIndex;

			if (TokenStream.Peek(peekIndex).Value != "method")
				return false;

			AccessVisibility visibility = GetVisibility();
			bool isStatic = TokenStream.Accept(TokenType.Word, "static");
			TokenStream.Expect("method");

			string ident = TokenStream.Peek().Value == "("
							? IdentConstructor
							: TokenStream.ReadWord();

			if (IsField(ident) || IsProperty(ident) || IsConstant(ident) || IsEnumeration(ident))
				throw new CompilerException("Redeclaration of method '{0}'.", ident);

			if (Parser.IsReservedIdent(ident))
				throw new CompilerException("'{0}' is a reserved symbol.", ident);

			Dictionary<string, Variant> arguments = new Dictionary<string, Variant>();
			TokenStream.Expect(TokenType.Delimiter, "(");
			if (!TokenStream.Accept(TokenType.Delimiter, ")"))
			{
				do
				{
					TokenStream.Expect("var");
					string varIdent = TokenStream.ReadWord();
					Variant varValue = ParseAnnotatedVariable();

					if (TokenStream.Accept(TokenType.Delimiter, "="))
						varValue = TokenStream.ReadVariant();

					arguments.Add(varIdent, varValue);
				}
				while (TokenStream.Accept(TokenType.Delimiter, ","));

				TokenStream.Expect(TokenType.Delimiter, ")");
			}

			if (GetMethod(ident, arguments.Count, currentClass) != null)
				throw new CompilerException("Ambiguous overload of method '{0}' declared. Original method declared on line {1}.",
					ident, GetMethod(ident, arguments.Count, currentClass).Line);

			currentClass.Methods.Add(new Method
			{
				Visibility = visibility,
				Name = ident,
				Variables = arguments,
				Position = TokenStream.Position,
				Static = isStatic,
				Filename = TokenStream.Filename,
				Line = TokenStream.CurrentLine
			});

			foreach(var arg in arguments)
				currentClass.Methods.Last().Arguments.Add(arg.Key);

			TokenStream.Expect(TokenType.Delimiter, "{");
			int braceCount = 1;
			do
			{
				if (TokenStream.Accept(TokenType.Delimiter, "{"))
					++braceCount;
				else if (TokenStream.Accept(TokenType.Delimiter, "}"))
					--braceCount;
				else
					TokenStream.Read();
			}
			while (braceCount > 0 && !TokenStream.EndOfStream);

			if (braceCount > 0)
				throw new CompilerException("Unmatched brace at end of file.");

			return true;
		}

		private bool Enum()
		{
			int peekIndex = 0;
			if (Parser.IsVisibility(TokenStream.Peek(peekIndex)))
				++peekIndex;

			if (TokenStream.Peek(peekIndex).Value != "enum")
				return false;

			Dictionary<string, long> values = new Dictionary<string, long>();
			long value = 0;

			AccessVisibility visibility = GetVisibility();
			TokenStream.Expect("enum");
			string ident = TokenStream.ReadWord();
			if (IsField(ident) || IsMethod(ident) || IsProperty(ident) || IsConstant(ident) || IsEnumeration(ident))
				throw new CompilerException("Redeclaration of enum '{0}'.", ident);

			if (Parser.IsReservedIdent(ident))
				throw new CompilerException("'{0}' is a reserved symbol.", ident);

			TokenStream.Expect(TokenType.Delimiter, "{");

			if (!TokenStream.Accept(TokenType.Delimiter, "}"))
			{
				do
				{
					string itemIdent = TokenStream.ReadWord();
					if (TokenStream.Accept(TokenType.Delimiter, "="))
					{
						Variant variant = TokenStream.ReadVariant();
						if (variant.Type != VariantType.Int64)
							TokenStream.Expected("enum value to be of integer type");

						value = variant.IntValue;
					}

					values.Add(itemIdent, value++);
				}
				while (TokenStream.Accept(TokenType.Delimiter, ","));

				TokenStream.Expect(TokenType.Delimiter, "}");
			}

			currentClass.Enums.Add(new Enumeration
			{
				Name = ident,
				Visibility = visibility,
				Values = values
			});

			TokenStream.Accept(TokenType.Delimiter, ";");

			return true;
		}

		private bool Const()
		{
			int peekIndex = 0;
			if (Parser.IsVisibility(TokenStream.Peek(peekIndex)))
				++peekIndex;

			if (TokenStream.Peek(peekIndex).Value != "const")
				return false;

			AccessVisibility visibility = GetVisibility();
			TokenStream.Expect("const");

			do
			{
				string ident = TokenStream.ReadWord();
				if (IsField(ident) || IsMethod(ident) || IsProperty(ident) || IsConstant(ident) || IsEnumeration(ident))
					throw new CompilerException("Redeclaration of constant '{0}'.", ident);

				if (Parser.IsReservedIdent(ident))
					throw new CompilerException("'{0}' is a reserved symbol.", ident);

				TokenStream.Expect(TokenType.Delimiter, "=");
				Variant value = TokenStream.ReadVariant();
				TokenStream.Accept(TokenType.Delimiter, ";");

				currentClass.Constants.Add(new Field
				{
					Visibility = visibility,
					Name = ident,
					Value = value
				});
			}
			while (TokenStream.Accept(TokenType.Delimiter, ","));

			return true;
		}

		private bool Property()
		{
			int peekIndex = 0;
			if (Parser.IsVisibility(TokenStream.Peek(peekIndex)))
				++peekIndex;

			if (TokenStream.Peek(peekIndex).Value != "property")
				return false;

			AccessVisibility visibility = GetVisibility();
			TokenStream.Expect("property");

			string ident = TokenStream.ReadWord();

			if (IsField(ident) || IsMethod(ident) || IsProperty(ident) || IsConstant(ident) || IsEnumeration(ident))
				throw new CompilerException("Redeclaration of property '{0}'.", ident);

			if (Parser.IsReservedIdent(ident))
				throw new CompilerException("'{0}' is a reserved symbol.", ident);

			currentClass.Fields.Add(new Field
			{
				Name = "__property_" + ident,
				Value = new Variant(),
				Visibility = visibility
			});

			TokenStream.Expect(TokenType.Delimiter, "{");

			while(!TokenStream.Accept(TokenType.Delimiter, "}"))
			{
				AccessVisibility accessorVisibility = Parser.IsVisibility(TokenStream.Peek()) ? GetVisibility() : visibility;

				if (TokenStream.Accept(TokenType.Word, "get"))
				{
					if (TokenStream.Accept(TokenType.Delimiter, ";"))
					{
						Method getAccessor = new Method
						{
							Name = "__get_" + ident,
							Visibility = accessorVisibility
						};

						getAccessor.Instructions.Add(new Instruction(Opcode.GetThis));
						getAccessor.Instructions.Add(new Instruction(Opcode.ClassGetField, "__property_" + ident));
						getAccessor.Instructions.Add(new Instruction(Opcode.Return));

						currentClass.Methods.Add(getAccessor);
					}
					else if (TokenStream.Accept(TokenType.Delimiter, "{"))
					{
						Method getAccessor = new Method
						{
							Name = "__get_" + ident,
							Visibility = accessorVisibility,
							Position = TokenStream.Position - 1
						};

						currentClass.Methods.Add(getAccessor);

						while(!TokenStream.Accept(TokenType.Delimiter, "}"))
							TokenStream.Read();
					}
					else
						TokenStream.Expected("; or body");
				}
				else if (TokenStream.Accept(TokenType.Word, "set"))
				{
					if (TokenStream.Accept(TokenType.Delimiter, ";"))
					{
						Method setAccessor = new Method
						{
							Name = "__set_" + ident,
							Visibility = accessorVisibility,
							Arguments = new List<string> { "value" }
						};

						setAccessor.Instructions.Add(new Instruction(Opcode.GetThis));
						setAccessor.Instructions.Add(new Instruction(Opcode.ClassSetField, "__property_" + ident));
						setAccessor.Instructions.Add(new Instruction(Opcode.Return));

						currentClass.Methods.Add(setAccessor);
					}
					else if (TokenStream.Accept(TokenType.Delimiter, "{"))
					{
						Method setAccessor = new Method
						{
							Name = "__set_" + ident,
							Visibility = accessorVisibility,
							Arguments = new List<string> { "value" },
							Variables = new Dictionary<string, Variant> { { "value", new Variant() } },
							Position = TokenStream.Position - 1
						};

						currentClass.Methods.Add(setAccessor);

						while (!TokenStream.Accept(TokenType.Delimiter, "}"))
							TokenStream.Read();
					}
					else
						TokenStream.Expect("; or body");
				}
				else
					TokenStream.Expected("get or set accessor");
			}

			return true;
		}

		private Variant ParseAnnotatedVariable()
		{
			if (!TokenStream.Accept(TokenType.Delimiter, ":"))
				return new Variant();

			string type = TokenStream.ReadWord();
			switch (type)
			{
				case "nil":
					return new Variant();

				case "int":
					return new Variant(0);

				case "double":
					return new Variant(0.0);

				case "str":
					return new Variant("") { ObjectName = "__str" };

				case "list":
					return new Variant(new List<Variant>()) { ObjectName = "__list" };

				case "object":
					return new Variant((object)null);

				default:
					if (!IsClass(type))
						throw new CompilerException("'{0}' is not a valid variant type.", type);

					return new Variant((object)null) { ObjectName = type };
			}
		}

		private AccessVisibility GetVisibility()
		{
			Dictionary<string, AccessVisibility> visibilities = new Dictionary<string, AccessVisibility>
			{
				{ "public", AccessVisibility.Public },
				{ "private", AccessVisibility.Private },
				{ "protected", AccessVisibility.Protected }
			};

			return !visibilities.ContainsKey(TokenStream.Peek().Value)
				? AccessVisibility.Private
				: visibilities[TokenStream.ReadWord()];
		}
	}
}
