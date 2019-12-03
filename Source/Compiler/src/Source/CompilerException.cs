using System;

namespace Swift.Source
{
	[Serializable]
	internal class CompilerException : Exception
	{
		public CompilerException(string message, params object[] args)
			: base(String.Format(message, args))
		{

		}

		public CompilerException(string message, Exception innerException, params object[] args)
			: base(String.Format(message, args), innerException)
		{

		}
	}
}
