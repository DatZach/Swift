using System;
using System.Collections.Generic;
using System.Linq;
using Swift.Source.Bytecode;
using Swift.Source.Lexer;

namespace Swift.Source
{
	internal partial class Compiler
	{
		private const string IdentConstructor = "__Constructor";

		private Class currentClass;
		private Method currentMethod;
		private readonly Stack<List<string>> scopeStack; 
		private readonly Stack<string> typeStack;
		private readonly Stack<Label> breakStack;
		private readonly Stack<Label> continueStack; 
		private readonly Random random;

		public List<Instruction> Instructions
		{
			get
			{
				return currentMethod.Instructions;
			}
		}

		private Class GetClass(string ident)
		{
			return Classes.FirstOrDefault(c => c.Name == ident);
		}

		private Field GetField(string ident, Class parent)
		{
			return parent.Fields.FirstOrDefault(f => f.Name == ident);
		}

		private Method GetMethod(string ident, Class parent)
		{
			return parent.Methods.FirstOrDefault(m => m.Name == ident);
		}

		private Method GetMethod(string ident, int argCount, Class parent)
		{
			return parent.Methods.FirstOrDefault(m => m.Name == ident && m.Arguments.Count == argCount);
		}

		private Field GetProperty(string ident, Class parent)
		{
			return parent.Fields.FirstOrDefault(p => p.Name == "__property_" + ident);
		}

		private bool IsClass(string ident)
		{
			return Classes.Any(c => c.Name == ident);
		}

		private bool IsClass(Token token)
		{
			return token.Type == TokenType.Word && IsClass(token.Value);
		}

		private bool IsVariableInScope(string ident)
		{
			return scopeStack.Any(list => list.Contains(ident));
		}

		private bool IsVariable(string ident)
		{
			return currentMethod.Variables.ContainsKey(ident);
		}

		private bool IsVariable(Token token)
		{
			return token.Type == TokenType.Word && IsVariable(token.Value);
		}

		private bool IsReference(string ident, Class parent = null)
		{
			return IsVariableInScope(ident) || IsField(ident, parent) || IsProperty(ident, parent) || ident == "this";
		}

		private bool IsReference(Token token)
		{
			return token.Type == TokenType.Word && IsReference(token.Value);
		}

		private bool IsConstant(string ident, Class parent = null)
		{
			Class target = parent ?? currentClass;
			return target.Constants.Any(constant => constant.Name == ident);
		}

		private bool IsConstant(Token token)
		{
			return token.Type == TokenType.Word && IsConstant(token.Value);
		}

		private bool IsEnumeration(string ident, Class parent = null)
		{
			Class target = parent ?? currentClass;
			return target.Enums.Any(e => e.Name == ident);
		}

		private bool IsEnumeration(Token token, Class parent = null)
		{
			return token.Type == TokenType.Word && IsEnumeration(token.Value, parent);
		}

		private bool IsMethod(string ident, Class parent = null)
		{
			Class target = parent ?? currentClass;
			return target.Methods.Any(method => method.Name == ident);
		}

		private bool IsMethod(Token token, Class parent = null)
		{
			return token.Type == TokenType.Word && IsMethod(token.Value, parent);
		}

		private bool IsField(string ident, Class parent = null)
		{
			Class target = parent ?? currentClass;
			return target.Fields.Any(field => field.Name == ident && !field.Static);
		}

		private bool IsField(Token token, Class parent = null)
		{
			return token.Type == TokenType.Word && IsField(token.Value, parent);
		}

		private bool IsStaticField(string ident, Class parent = null)
		{
			Field field = GetField(ident, parent ?? currentClass);
			return field != null && field.Static;
		}

		private bool IsProperty(string ident, Class parent = null)
		{
			return IsField("__property_" + ident, parent);
		}

		private bool IsStaticProperty(string ident, Class parent = null)
		{
			return IsStaticField("__property_" + ident, parent);
		}

		private string CreateTemporaryVariable()
		{
			string name;

			do
				name = "__tmp_var_" + random.Next(9999);
			while(IsVariable(name));

			currentMethod.Variables.Add(name, new Variant());

			return name;
		}
	}
}
