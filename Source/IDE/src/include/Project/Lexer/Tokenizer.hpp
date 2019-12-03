/*
 *  Tokenizer.hpp
 *  Tokenizer
 */

#ifndef __LEXER_TOKENIZER_HPP
#define __LEXER_TOKENIZER_HPP

#include <QList>
#include <QStringList>
#include <Project/Lexer/Token.hpp>
#include <Project/Lexer/StringStream.hpp>

#define LexerTokenizer      Lexer::TokenizerSingleton::GetInstance()

namespace Lexer
{
	class TokenizerSingleton
	{
	private:
		QStringList delimiters;

	private:
		void SkipWhitespace(StringStream& stream);
		bool SkipComment(StringStream& stream);
		bool ParseStringInner(StringStream& stream, QList<Token>& tokens);
		bool ParseWord(StringStream& stream, QList<Token>& tokens);
		bool ParseNumber(StringStream& stream, QList<Token>& tokens);
		bool ParseDelimiter(StringStream& stream, QList<Token>& tokens);

		bool IsHex(char ch) const;

	public:
		TokenizerSingleton();

		bool ParseString(const QString& value, QList<Token>& tokens);
		bool ParseFile(const QString& filename, QList<Token>& tokens);

		static TokenizerSingleton& GetInstance();
	};
}

#endif
