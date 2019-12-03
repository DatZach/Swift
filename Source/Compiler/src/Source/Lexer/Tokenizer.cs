using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;

namespace Swift.Source.Lexer
{
	internal static class Tokenizer
	{
		// NOTE Make sure that the largest delimiters come first in the list
		private static readonly List<string> Delimiters = new List<string>
		{
			"<<=", ">>=",
			"+=", "-=", "*=", "/=", "%=", "|=", "^=", "&=", "==", "!=",
			"<=", ">=", "<<", ">>", "&&", "||", "++", "--", "..",
			"~", "!", "%", "^", "&", "*", "(", ")", "-", "+", "=", "[",
			"]", "{", "}" ,"|", ":", ";", "?", "/", "<", ">", ",", "."
		};

		public static string Filename { get; private set; }
		public static uint Line { get; private set; }

		public static List<Token> ParseFile(string filename)
		{
			Filename = filename;

			try
			{
				using (StreamReader reader = new StreamReader(Filename))
					return ParseStream(new StringStream(reader.ReadToEnd()));
			}
			catch (IOException)
			{
				Console.WriteLine("Cannot open file \"{0}\" for compiling.", Filename);
				throw;
			}
		}

		private static List<Token> ParseStream(StringStream stream)
		{
			List<Token> tokens = new List<Token>();
			Line = 1;

			while (!stream.IsEndOfStream)
			{
				SkipWhitespace(stream);

				if (SkipComment(stream))
					continue;

				if (ParseString(stream, tokens))
					continue;

				if (ParseWord(stream, tokens))
					continue;

				if (ParseNumber(stream, tokens))
					continue;

				if (ParseDelimiter(stream, tokens))
					continue;

				if (stream.IsEndOfStream)
					break;

				throw new LexerException("Unexpected token '{0}' on line {1}.", stream.Read(), Line);
			}

			tokens.Add(new Token(TokenType.EndOfStream, Filename, Line));

			return tokens;
		}

		private static void SkipWhitespace(StringStream stream)
		{
			while (!stream.IsEndOfStream && char.IsWhiteSpace(stream.Peek()))
			{
				// Read new lines
				if (stream.Read() == StringStream.NewLine)
					++Line;
			}
		}

		private static bool SkipComment(StringStream stream)
		{
			// Skip single line comments
			if (stream.Peek() == '/' && stream.Peek(1) == '/')
			{
				while (!stream.IsEndOfStream && stream.Peek() != StringStream.NewLine)
					stream.Read();

				return true;
			}

			// Skip multiline comments
			if (stream.Peek() == '/' && stream.Peek(1) == '*')
			{
				stream.Position += 2;
				int nestCount = 1;

				while (nestCount > 0)
				{
					if (stream.IsEndOfStream)
						throw new LexerException("Unterminated multi-line comment ran past end of file.");

					if (stream.Peek() == '/' && stream.Peek(1) == '*')
						++nestCount;
					else if (stream.Peek() == '*' && stream.Peek(1) == '/')
						--nestCount;
					else
					{
						++stream.Position;
						continue;
					}

					stream.Position += 2;

					if (stream.Read() == StringStream.NewLine)
						++Line;
				}

				return true;
			}

			return false;
		}

		private static bool ParseString(StringStream stream, ICollection<Token> tokens)
		{
			if (!(stream.Peek() == '@' && stream.Peek(1) == '\"') && stream.Peek() != '\"')
				return false;

			uint startLine = Line;
			string value = "";

			bool escaped = true;
			if (stream.Peek() == '@')
			{
				escaped = false;
				stream.Read();
			}

			while (stream.Peek() == '\"')
			{
				char ch;

				++stream.Position;
				while ((ch = stream.Read()) != '\"')
				{
					if (stream.IsEndOfStream)
						throw new LexerException("Unterminated string on line {0}.", startLine);

					if (ch == StringStream.NewLine)
						++Line;

					value += ch;
					if (ch == '\\')
						value += stream.Read();
				}

				SkipWhitespace(stream);
			}

			value = escaped ? ParseEscapeSequence(value) : value;
			tokens.Add(new Token(TokenType.String, value, Filename, startLine));

			return true;
		}

		private static bool ParseWord(StringStream stream, ICollection<Token> tokens)
		{
			if (char.IsLetter(stream.Peek()) || stream.Peek() == '_')
			{
				string word = "";

				while (!stream.IsEndOfStream && (char.IsLetterOrDigit(stream.Peek()) || stream.Peek() == '_'))
					word += stream.Read();

				switch(word)
				{
					case "true":
						tokens.Add(new Token(TokenType.Number, "1", Filename, Line));
						break;

					case "false":
						tokens.Add(new Token(TokenType.Number, "0", Filename, Line));
						break;

					default:
						tokens.Add(new Token(TokenType.Word, word, Filename, Line));
						break;
				}

				return true;
			}

			return false;
		}

		private static bool ParseDelimiter(StringStream stream, ICollection<Token> tokens)
		{
			bool foundDelimiter = false;
			string peekedDelimiter = "";

			foreach (string delimiter in Delimiters)
			{
				peekedDelimiter = "";

				for (int i = 0; i < delimiter.Length; ++i)
					peekedDelimiter += stream.Peek(i);

				if (peekedDelimiter == delimiter)
				{
					stream.Position += delimiter.Length;
					foundDelimiter = true;
					break;
				}
			}

			if (foundDelimiter)
			{
				tokens.Add(new Token(TokenType.Delimiter, peekedDelimiter, Filename, Line));
				return true;
			}

			return false;
		}

		private static bool ParseNumber(StringStream stream, ICollection<Token> tokens)
		{
			if (char.IsDigit(stream.Peek()))
			{
				string number = "";

				if (stream.Peek() == '0' && stream.Peek(1) == 'x')
				{
					stream.Position += 2;

					while (!stream.IsEndOfStream && stream.Peek().IsHex())
						number += stream.Read();

					ulong result;
					try
					{
						result = ulong.Parse(number, NumberStyles.HexNumber, CultureInfo.InvariantCulture);
					}
					catch (OverflowException)
					{
						throw new LexerException("Number is larger than 64bits on line {0}.", Line);
					}
					catch (Exception)
					{
						throw new LexerException("Invalid number on line {0}.", Line);
					}

					number = result.ToString("G");
				}
				else if (stream.Peek() == '0' && stream.Peek(1) == 'b')
				{
					stream.Position += 2;

					while (!stream.IsEndOfStream && char.IsDigit(stream.Peek()))
						number += stream.Read();

					long result;
					try
					{
						result = Convert.ToInt64(number, 2);
					}
					catch(OverflowException)
					{
						throw new LexerException("Number is larger than 64bits on line {0}.", Line);
					}
					catch(Exception)
					{
						throw new LexerException("Invalid number on line {0}.", Line);
					}

					number = result.ToString("G");
				}
				else
				{
					while (!stream.IsEndOfStream && (char.IsDigit(stream.Peek()) || (stream.Peek() == '.' && stream.Peek(1) != '.')))
						number += stream.Read();
				}

				tokens.Add(new Token(TokenType.Number, number, Filename, Line));

				return true;
			}

			return false;
		}

		private static string ParseEscapeSequence(string value)
		{
			StringStream stringStream = new StringStream(value);
			string parsedValue = "";

			while (!stringStream.IsEndOfStream)
			{
				// Read non-escaping characters
				if (stringStream.Peek() != '\\')
				{
					parsedValue += stringStream.Read();
					continue;
				}

				// Skip backslash
				stringStream.Read();

				switch (stringStream.Read())
				{
					case '\'':
						parsedValue += '\'';
						continue;

					case '\"':
						parsedValue += '\"';
						continue;

					case '\\':
						parsedValue += '\\';
						continue;

					case '%':
						parsedValue += '%';
						continue;

					case ':':
						parsedValue += ':';
						continue;

					case '\0':
						parsedValue += '\0';
						continue;

					case 'a':
						parsedValue += '\a';
						continue;

					case 'b':
						parsedValue += '\b';
						continue;

					case 'f':
						parsedValue += '\f';
						continue;

					case 'n':
						parsedValue += '\n';
						continue;

					case 'r':
						parsedValue += '\r';
						continue;

					case 't':
						parsedValue += '\t';
						continue;

					case 'u':
					case 'U':
					case 'x':
					{
						string unicodeValue = "";

						for(int i = 0; i < 8; ++i)
						{
							char read = stringStream.Read();
							unicodeValue += read;

							if (!stringStream.Peek(1).IsHex() && (i == 1 || i == 3 || i == 7))
								break;
						}

						uint unicodeIntValue;
						if (!uint.TryParse(unicodeValue, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out unicodeIntValue))
							throw new LexerException("Malformed escape code \\x, \\u or \\U on line {0}.", Line);

						parsedValue += Convert.ToChar(unicodeIntValue);
						continue;
					}

					case 'v':
						parsedValue += '\v';
						continue;
				}

				throw new LexerException("Bad escape sequence.");
			}

			return parsedValue;
		}

		private static bool IsHex(this char value)
		{
			return char.IsDigit(value) || new HashSet<char>
			{
				'A',
				'B',
				'C',
				'D',
				'E',
				'F'
			}.Contains(char.ToUpper(value, CultureInfo.InvariantCulture));
		}
	}
}
