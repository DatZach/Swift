/*
 *	logger.hpp
 *	Logger
 *	Adapted from Crossbox's Logger classes
 */

#ifndef __UTILITY_LOGGER_HPP
#define __UTILITY_LOGGER_HPP

#include <iostream>
#include <string>

const char lendl	= '\n';
const char ltab		= '\t';

#define LgrOpOverload(x)								\
	Logger& operator <<(x value)						\
	{													\
		if (type == LtVerboseTrace && !verboseTrace)	\
			return *this;								\
														\
		std::cout << value;								\
														\
		return *this;									\
	}

#define PreLgrOpOverload(x)								\
	Logger& operator <<(x value)						\
	{													\
		if (!PrintPrefix())								\
			return *this;								\
														\
		std::cout << value;								\
														\
		return reinterpret_cast<Logger&>(*this);		\
	}

class Logger
{
public:
	enum LogType
	{
		LtTrace,
		LtWarn,
		LtError,
		LtLog,
		LtVerboseTrace,
		LtCount
	};

protected:
	LogType type;

public:
	static bool verboseTrace;

	Logger(LogType type);

	// Primitive types
	Logger& operator <<(char value)
	{
		if (type == LtVerboseTrace && !verboseTrace)
			return *this;

		// Force stdout buffer to flush
		if (value == lendl)
			std::cout << std::endl;
		else
			std::cout << value;

		return *this;
	}

	LgrOpOverload(short)
	LgrOpOverload(int)
	LgrOpOverload(long long)
	LgrOpOverload(float)
	LgrOpOverload(double)
	LgrOpOverload(unsigned char)
	LgrOpOverload(unsigned short)
	LgrOpOverload(unsigned int)
	LgrOpOverload(unsigned long long)
	LgrOpOverload(bool)

	LgrOpOverload(const char*)
	LgrOpOverload(char*)

	// Classes
	LgrOpOverload(const std::string&)
	LgrOpOverload(std::string&)
	LgrOpOverload(const std::ostream&)
	LgrOpOverload(std::ostream&)
};

class LoggerPrefixed : public Logger
{
private:
	bool PrintPrefix() const;

public:
	LoggerPrefixed(Logger::LogType type);

	// Primitive types
	Logger& operator <<(char value)
	{
		if (!PrintPrefix())
			return *this;

		// Force stdout buffer to flush
		if (value == lendl)
			std::cout << std::endl;
		else
			std::cout << value;

		return reinterpret_cast<Logger&>(*this);
	}

	PreLgrOpOverload(short)
	PreLgrOpOverload(int)
	PreLgrOpOverload(long long)
	PreLgrOpOverload(float)
	PreLgrOpOverload(double)
	PreLgrOpOverload(unsigned char)
	PreLgrOpOverload(unsigned short)
	PreLgrOpOverload(unsigned int)
	PreLgrOpOverload(unsigned long long)
	PreLgrOpOverload(bool)

	PreLgrOpOverload(const char*)
	PreLgrOpOverload(char*)

	// Classes
	PreLgrOpOverload(const std::string&)
	PreLgrOpOverload(std::string&)
	PreLgrOpOverload(const std::ostream&)
	PreLgrOpOverload(std::ostream&)
};

#undef PreLgrOpOverload
#undef LgrOpOverload

extern LoggerPrefixed Trace;
extern LoggerPrefixed Warn;
extern LoggerPrefixed Error;
extern LoggerPrefixed Log;
extern LoggerPrefixed VerboseTrace;

#endif
