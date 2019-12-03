/*
 *	StringStream.hpp
 *	Lexer's String Stream
 */

#ifndef __LEXER_STRING_STREAM_HPP
#define __LEXER_STRING_STREAM_HPP

#include <QString>

namespace Lexer
{
	class StringStream
	{
	private:
		QString value;

	public:
		int position;

	public:
		StringStream(const QString& value);

		char Peek(int offset = 0);
		char Read();

		bool IsEos() const;
	};
}

#endif
