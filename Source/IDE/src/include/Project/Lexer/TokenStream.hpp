/*
 *	TokenStream.hpp
 *	Token Stream
 */

#ifndef __LEXER_TOKEN_STREAM_HPP
#define __LEXER_TOKEN_STREAM_HPP

#include <QVariant>
#include <Project/Lexer/Token.hpp>

namespace Lexer
{
	class TokenStream
	{
	private:
		QList<Token> tokens;

	public:
		int position;

	public:
		TokenStream(const QString& filename, bool& ok);
		TokenStream(const QList<Token>& tokens);

		Token Peek(int offset = 0);
		Token Read();
		bool Accept(TokenType type, const QString& value);
		bool Accept(const QString& value);
		bool Pass(TokenType type, const QString& value);
		bool Pass(const QString& value);
		bool Pass(TokenType type);
		QString ReadWord();
		QVariant ReadVariant();

		bool IsEos() const;
	};
}

#endif
