using System.Collections.Generic;

namespace Swift.Source.Lexer
{
	internal static class Parser
	{
		public static bool IsReservedIdent(string value)
		{
			return new HashSet<string>
			{
				"class",
				"entity",
				"state",
				"global",
				"var",
				"method",
				"const",
				"enum",
				"str",
				"int",
				"double",
				"if",
				"for",
				"do",
				"while",
				"foreach",
				"repeat"
			}.Contains(value);
		}

		public static bool IsAddOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Delimiter)
				return false;

			return new HashSet<string> { "+", "-" }.Contains(token.Value);
		}

		public static bool IsMulOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Delimiter)
				return false;

			return new HashSet<string> { "*", "/", "%" }.Contains(token.Value);
		}

		public static bool IsAssignOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Delimiter)
				return false;

			return new HashSet<string> { "=", "+=", "-=", "*=", "/=", "%=", "|=", "^=", "&=", "<<=", ">>=" }.Contains(token.Value);
		}

		public static bool IsShiftOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Delimiter)
				return false;

			return new HashSet<string> { "<<", ">>" }.Contains(token.Value);
		}

		public static bool IsRelationalOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Delimiter)
				return false;

			return new HashSet<string> { "<", ">", "<=", ">=" }.Contains(token.Value);
		}

		public static bool IsEqualityOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Delimiter)
				return false;

			return new HashSet<string> { "==", "!=" }.Contains(token.Value);
		}

		public static bool IsIncrementOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Delimiter)
				return false;

			return new HashSet<string> { "++", "--" }.Contains(token.Value);
		}

		public static bool IsCastOperation(Token token)
		{
			if (token == null || token.Type != TokenType.Word)
				return false;

			return new HashSet<string>
			{
				"int",
				"double",
				"str"
			}.Contains(token.Value);
		}

		public static bool IsVisibility(Token token)
		{
			return token.Type == TokenType.Word && new HashSet<string>
			{
				"public",
				"private",
				"protected"
			}.Contains(token.Value);
		}
	}
}
