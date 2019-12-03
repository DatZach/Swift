/*
 *  Tokenizer.cpp
 *  Tokenizer
 */

#include <QDebug>
#include <QFile>
#include <Project/Lexer/Tokenizer.hpp>
#include <Project/Lexer/StringStream.hpp>

namespace Lexer
{
	TokenizerSingleton::TokenizerSingleton()
		: delimiters()
	{
		delimiters.push_back(">>=");
		delimiters.push_back("<<=");
		delimiters.push_back("+=");
		delimiters.push_back("-=");
		delimiters.push_back("*=");
		delimiters.push_back("/=");
		delimiters.push_back("%=");
		delimiters.push_back("|=");
		delimiters.push_back("^=");
		delimiters.push_back("&=");
		delimiters.push_back("==");
		delimiters.push_back("!=");
		delimiters.push_back("<=");
		delimiters.push_back(">=");
		delimiters.push_back("<<");
		delimiters.push_back(">>");
		delimiters.push_back("&&");
		delimiters.push_back("||");
		delimiters.push_back("++");
		delimiters.push_back("--");
		delimiters.push_back("..");
		delimiters.push_back("~");
		delimiters.push_back("!");
		delimiters.push_back("%");
		delimiters.push_back("^");
		delimiters.push_back("&");
		delimiters.push_back("*");
		delimiters.push_back("(");
		delimiters.push_back(")");
		delimiters.push_back("-");
		delimiters.push_back("+");
		delimiters.push_back("=");
		delimiters.push_back("[");
		delimiters.push_back("]");
		delimiters.push_back("{");
		delimiters.push_back("}");
		delimiters.push_back("|");
		delimiters.push_back(":");
		delimiters.push_back(";");
		delimiters.push_back("?");
		delimiters.push_back("/");
		delimiters.push_back("<");
		delimiters.push_back(">");
		delimiters.push_back(",");
		delimiters.push_back(".");
	}

	bool TokenizerSingleton::ParseString(const QString& value, QList<Token>& tokens)
	{
		StringStream stream(value);

		while(!stream.IsEos())
		{
			SkipWhitespace(stream);

			if (SkipComment(stream))
				continue;

			if (ParseStringInner(stream, tokens))
				continue;

			if (ParseWord(stream, tokens))
				continue;

			if (ParseNumber(stream, tokens))
				continue;

			if (ParseDelimiter(stream, tokens))
				continue;

			if (stream.IsEos())
				break;

			qDebug() << "Lexer error";
			return false;
		}

		return true;
	}

	bool TokenizerSingleton::ParseFile(const QString& filename, QList<Token>& tokens)
	{
		QFile file(filename);
		if (!file.open(QFile::ReadOnly | QFile::Text))
			return false;

		return ParseString(file.readAll(), tokens);
	}

	void TokenizerSingleton::SkipWhitespace(StringStream& stream)
	{
		while(!stream.IsEos() && QChar(stream.Peek()).isSpace())
			stream.Read();
	}

	bool TokenizerSingleton::SkipComment(StringStream& stream)
	{
		// Skip single line comments
		if (stream.Peek() == '/' && stream.Peek(1) == '/')
		{
			while(!stream.IsEos() && stream.Peek() != '\n')
				stream.Read();

			return true;
		}

		// Skip multiline comments
		if (stream.Peek() == '/' && stream.Peek(1) == '*')
		{
			while(!stream.IsEos() && !(stream.Peek() == '*' && stream.Peek(1) == '/'))
				stream.Read();

			stream.position += 2;
			return true;
		}

		return false;
	}

	bool TokenizerSingleton::ParseStringInner(StringStream& stream, QList<Token>& tokens)
	{
		if (stream.Peek() != '\"')
			return false;

		QString value = "";

		while(stream.Peek() == '\"')
		{
			char ch;

			++stream.position;
			while((ch = stream.Read()) != '\"')
			{
				if (stream.IsEos())
					qDebug() << "Unterminated string";

				value += ch;
			}

			SkipWhitespace(stream);
		}

		tokens.push_back(Token(TokenType::String, value));
		return true;
	}

	bool TokenizerSingleton::ParseWord(StringStream& stream, QList<Token>& tokens)
	{
		if (QChar(stream.Peek()).isLetter() || stream.Peek() == '_')
		{
			QString word = "";

			while(!stream.IsEos() && (QChar(stream.Peek()).isLetterOrNumber() || stream.Peek() == '_'))
				word += stream.Read();

			tokens.push_back(Token(TokenType::Word, word));
			return true;
		}

		return false;
	}

	bool TokenizerSingleton::ParseNumber(StringStream& stream, QList<Token>& tokens)
	{
		if (QChar(stream.Peek()).isDigit())
		{
			QString number = "";

			if (stream.Peek() == '0' && stream.Peek(1) == 'x')
			{
				stream.position += 2;

				while(!stream.IsEos() && IsHex(stream.Peek()))
					number += stream.Read();

				bool ok;
				qlonglong result = number.toLongLong(&ok, 16);
				if (!ok)
					qDebug() << "Invalid hex number";

				number = QString::number(result);
			}
			else if (stream.Peek() == '0' && stream.Peek(1) == 'b')
			{
				stream.position += 2;

				while(!stream.IsEos() && (stream.Peek() == '0' || stream.Peek() == '1'))
					number += stream.Read();

				bool ok;
				qlonglong result = number.toLongLong(&ok, 2);
				if (!ok)
					qDebug() << "Invalid hex number";

				number = QString::number(result);
			}
			else
			{
				while(!stream.IsEos() && (QChar(stream.Peek()).isDigit() || (stream.Peek() == '.' && stream.Peek(1) != '.')))
					number += stream.Read();
			}

			tokens.push_back(Token(TokenType::Number, number));
			return true;
		}

		return false;
	}

	bool TokenizerSingleton::ParseDelimiter(StringStream& stream, QList<Token>& tokens)
	{
		bool foundDelimiter = false;
		QString peekedDelimiter = "";

		for(QString delimiter : delimiters)
		{
			peekedDelimiter = "";

			for(int i = 0; i < delimiter.length(); ++i)
				peekedDelimiter += stream.Peek(i);

			if (peekedDelimiter == delimiter)
			{
				stream.position += delimiter.length();
				foundDelimiter = true;
				break;
			}
		}

		if (foundDelimiter)
		{
			tokens.push_back(Token(TokenType::Delimiter, peekedDelimiter));
			return true;
		}

		return false;
	}

	bool TokenizerSingleton::IsHex(char ch) const
	{
		QChar value = QChar(ch).toUpper();
		return value.isDigit() || (value >= 'A' && value <= 'F');
	}

	TokenizerSingleton& TokenizerSingleton::GetInstance()
	{
		static TokenizerSingleton instance;
		return instance;
	}
}
