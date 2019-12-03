/*
 *	logger.cpp
 *	Logger
 */

#include <Util/Logger.hpp>

bool Logger::verboseTrace = false;

// Used for debugging related information
LoggerPrefixed Trace(Logger::LtTrace);

// Used for nonfatal errors
LoggerPrefixed Warn(Logger::LtWarn);

// Used for fatal errors
LoggerPrefixed Error(Logger::LtError);

// Used for general purpose logging
LoggerPrefixed Log(Logger::LtLog);

// Used for verbose (--debug) debug logging
LoggerPrefixed VerboseTrace(Logger::LtVerboseTrace);

Logger::Logger(LogType type)
	: type(type)
{

}

bool LoggerPrefixed::PrintPrefix() const
{
	const char* prefixes[LtCount] =
	{
		"[Trace]",
		"[Warn ]",
		"[Error]",
		"[Log  ]",
		"[Trace]"
	};

	if (type == LtVerboseTrace && !verboseTrace)
		return false;

	std::cout << prefixes[static_cast<unsigned int>(type)]
			  << ' ';
	
	return true;
}

LoggerPrefixed::LoggerPrefixed(LogType type)
	: Logger(type)
{
	
}
