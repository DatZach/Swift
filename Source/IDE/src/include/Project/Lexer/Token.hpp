/*
 *  Token.hpp
 *  Token
 */

#ifndef __TOKEN_HPP
#define __TOKEN_HPP

#include <QString>

namespace Lexer
{
	enum class TokenType
	{
		Unknown,
		Word,
		Number,
		String,
		Delimiter,
		EndOfStream
	};

	class Token
	{
	private:
		TokenType pType;
		QString pValue;

	public:
		Token(TokenType type, QString value)
			: pType(type),
			  pValue(value)
		{

		}

		Token(TokenType type)
			: pType(type)
		{

		}

		TokenType type() const { return pType; }
		QString value() const { return pValue; }
	};
}

#endif
