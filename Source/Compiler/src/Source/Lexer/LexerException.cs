using System;

namespace Swift.Source.Lexer
{
	[Serializable]
	internal class LexerException : Exception
	{
		public LexerException(string message, params object[] args)
			: base(String.Format(message, args))
		{

		}

		public LexerException(string message, Exception innerException, params object[] args)
			: base(String.Format(message, args), innerException)
		{

		}
	}
}
