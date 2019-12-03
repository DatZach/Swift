using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using Swift.Source.Bytecode;

namespace Swift.Source.Lexer
{
	internal class TokenStream
	{
		private readonly Stack<int> positionStack;
		private readonly List<Token> tokens;
		public int Position;

		public bool EndOfStream
		{
			get
			{
				return Position >= tokens.Count || tokens[Position].Type == TokenType.EndOfStream;
			}
		}

		public string Filename
		{
			get
			{
				return tokens.First() == null
						? ""
						: tokens.First().Filename;
			}
		}

		public uint CurrentLine
		{
			get
			{
				return Peek() == null
						? tokens.Last().Line
						: Peek().Line;
			}
		}

		public TokenStream(string filename)
		{
			positionStack = new Stack<int>();
			tokens = Tokenizer.ParseFile(filename);
			Position = 0;
		}

		public TokenStream(List<Token> tokens)
		{
			positionStack = new Stack<int>();
			this.tokens = tokens;
			Position = 0;
		}

		public Token Peek(int offset = 0)
		{
			return Position + offset < tokens.Count
				       ? tokens[Position + offset]
				       : new Token(TokenType.EndOfStream, tokens.First().Filename, 0);
		}

		public Token Read()
		{
			return EndOfStream
				? new Token(TokenType.EndOfStream, tokens.First().Filename, 0)
				: tokens[Position++];
		}

		public void Expect(TokenType type)
		{
			if (!EndOfStream && Read().Type == type)
				return;

			Expected(type.ToString());
		}

		public void Expect(TokenType type, string value)
		{
			if (!EndOfStream)
			{
				Token token = Peek();
				if (token.Type == type && token.Value == value)
				{
					++Position;
					return;
				}
			}

			Expected(value);
		}

		public void Expect(string value)
		{
			if (!EndOfStream)
			{
				Token token = Peek();
				if (token.Type == TokenType.Word && token.Value == value)
				{
					++Position;
					return;
				}
			}

			Expected(value);
		}

		public bool Accept(TokenType type)
		{
			if (EndOfStream)
				return false;

			Token token = Peek();
			if (token.Type == type)
			{
				++Position;
				return true;
			}

			return false;
		}

		public bool Accept(TokenType type, string value)
		{
			if (EndOfStream)
				return false;

			Token token = Peek();
			if (token.Type == type && token.Value == value)
			{
				++Position;
				return true;
			}

			return false;
		}

		public bool Pass(TokenType type)
		{
			return Peek().Type == type;
		}

		public bool Pass(TokenType type, string value)
		{
			return Peek().Type == type && Peek().Value == value;
		}

		public bool Pass(string value)
		{
			return Peek().Type == TokenType.Word && Peek().Value == value;
		}

		public string ReadWord()
		{
			Token token = Read() ?? tokens.Last();
			if (token.Type != TokenType.Word)
			{
				Expected("identifier");
				return "";
			}

			return token.Value;
		}

		public Variant ReadVariant()
		{
			Token token = Read();
			switch(token.Type)
			{
				case TokenType.Word:
					if (token.Value == "null")
						return new Variant((object)null);
					
					if (token.Value == "nil")
						return new Variant();

					throw new CompilerException("Unexpected identifier \"{0}\".", token.Value);

				case TokenType.String:
					return new Variant(token.Value);

				case TokenType.Number:
				{
					if (token.Value.Contains("."))
					{
						double value;

						if (!double.TryParse(token.Value, NumberStyles.Number, null, out value))
							throw new LexerException("Cannot parse variant literal \"{0}\".", value);

						return new Variant(value);
					}
					else
					{
						long value;

						if (!long.TryParse(token.Value, NumberStyles.Number, null, out value))
							throw new LexerException("Cannot parse variant literal \"{0}\".", value);

						return new Variant(value);
					}
				}

				case TokenType.Delimiter:
				{
					// NOTE For expressions, literal lists should be evaluated above this
					//		This is primarily for fields.
					if (token.Value != "[")
						break;

					List<Variant> listValues = new List<Variant>();

					while(!Accept(TokenType.Delimiter, "]"))
					{
						if (Peek(1).Value == "..")
						{
							Variant rangeStart = ReadVariant();
							Expect(TokenType.Delimiter, "..");
							Variant rangeEnd = ReadVariant();

							if (rangeStart.Type != VariantType.Int64 || rangeEnd.Type != VariantType.Int64)
								throw new LexerException("List range identifiers must be integers.");

							for(long i = rangeStart.IntValue; i <= rangeEnd.IntValue; ++i)
								listValues.Add(new Variant(i));
						}
						else
							listValues.Add(ReadVariant());

						Accept(TokenType.Delimiter, ",");
					}

					return new Variant(listValues);
				}
			}

			--Position;
			Expected("variant");

			return new Variant();
		}

		public void PushPosition()
		{
			positionStack.Push(Position);
		}

		public void PopPosition()
		{
			Position = positionStack.Pop();
		}

		private void Error(string message, params object[] args)
		{
			throw new CompilerException(message, args);
		}

		public void Expected(string value)
		{
			Error("Expected {0} got \"{1}\" instead.", value, Peek().Value);
		}
	}
}
