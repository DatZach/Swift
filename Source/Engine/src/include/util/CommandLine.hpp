/*
 *	cmdline.hpp
 *	Command Line
 */

#ifndef __UTILITY_COMMAND_LINE_HPP
#define __UTILITY_COMMAND_LINE_HPP

#include <string>
#include <map>

namespace Util
{
	class CommandLine
	{
	private:
		std::map<std::string, std::string> flags;

	public:
		CommandLine(int argc, char* argv[]);

		static bool RedirectIOToConsole();

		std::string GetFlagArgument(const std::string& value) const;
		bool IsFlagSet(const std::string& value) const;
	};
}

#endif
