/*
 *	TokenStream.hpp
 *	Token Stream
 */

#include <Project/Lexer/TokenStream.hpp>
#include <Project/Lexer/Tokenizer.hpp>

namespace Lexer
{
	TokenStream::TokenStream(const QString& filename, bool& ok)
		: tokens(),
		  position(0)
	{
		ok = LexerTokenizer.ParseFile(filename, tokens);
	}

	TokenStream::TokenStream(const QList<Token>& tokens)
		: tokens(tokens),
		  position(0)
	{

	}

	Token TokenStream::Peek(int offset)
	{
		return position + offset < tokens.size()
				? tokens[position + offset]
				: Token(TokenType::EndOfStream);
	}

	Token TokenStream::Read()
	{
		return IsEos()
			? Token(TokenType::EndOfStream)
			: tokens[position++];
	}

	bool TokenStream::Accept(TokenType type, const QString& value)
	{
		if (IsEos())
			return false;

		Token token = Peek();
		if (token.type() == type && token.value() == value)
		{
			++position;
			return true;
		}

		return false;
	}

	bool TokenStream::Accept(const QString& value)
	{
		if (IsEos())
			return false;

		Token token = Peek();
		if (token.type() == TokenType::Word && token.value() == value)
		{
			++position;
			return true;
		}

		return false;
	}

	bool TokenStream::Pass(TokenType type, const QString& value)
	{
		return Peek().type() == type && Peek().value() == value;
	}

	bool TokenStream::Pass(const QString& value)
	{
		return Peek().type() == TokenType::Word && Peek().value() == value;
	}

	bool TokenStream::Pass(TokenType type)
	{
		return Peek().type() == type;
	}

	QString TokenStream::ReadWord()
	{
		Token token = Read();
		if (token.type() != TokenType::Word)
			return "";

		return token.value();
	}

	QVariant TokenStream::ReadVariant()
	{
		Token token = Read();
		switch(token.type())
		{
			case TokenType::Word:
				if (token.value() == "nil")
					return QVariant();
				break;

			case TokenType::String:
				return QVariant(token.value());

			case TokenType::Number:
				if (token.value().contains('.'))
					return QVariant(token.value().toDouble());

				return QVariant(token.value().toLongLong());
		}

		return QVariant();
	}

	bool TokenStream::IsEos() const
	{
		return position >= tokens.size() || tokens[position].type() == TokenType::EndOfStream;
	}
}
