namespace Swift
{
	/*
	 *	Crappy implementation of StringStream.
	 *	Still less crappy than any default .NET implementations of StringStream.
	 *	Rewrite using TextReader
	 */

	internal class StringStream
	{
		public const char NewLine = '\n';

		private readonly string value;
		public int Position;

		public bool IsEndOfStream
		{
			get
			{
				return Position >= value.Length;
			}
		}

		public StringStream(string value)
		{
			this.value = value;
			Position = 0;
		}

		public char Peek(int offset = 0)
		{
			return Position + offset < value.Length ? value[Position + offset] : '\0';
		}

		public char Read()
		{
			return Position < value.Length ? value[Position++] : '\0';
		}
	}
}
