/*
 *	StringStream.hpp
 *	Lexer's String Stream
 */

#include <Project/Lexer/StringStream.hpp>

namespace Lexer
{
	StringStream::StringStream(const QString& value)
		: value(value),
		  position(0)
	{

	}

	char StringStream::Peek(int offset)
	{
		return position + offset < value.length()
				? value[position + offset].toLatin1()
				: '\0';
	}

	char StringStream::Read()
	{
		return position < value.length()
			? value[position++].toLatin1()
			: '\0';
	}

	bool StringStream::IsEos() const
	{
		return position >= value.length();
	}
}
