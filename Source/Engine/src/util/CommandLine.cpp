/*
 *	cmdline.cpp
 *	Command Line
 */

#include <algorithm>
#include <Util/CommandLine.hpp>

#ifdef WINDOWS
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

namespace Util
{
	CommandLine::CommandLine(int argc, char* argv[])
		: flags()
	{
		for(int i = 1; i < argc; ++i)
		{
			// Parse arguments starting with '--' as a flag
			std::string value = argv[i];
			if (value.find_first_of("--") == std::string::npos)
				continue;

			// Parse argument if there is one
			std::string argument = "";
			if (i + 1 < argc)
			{
				std::string tmp = argv[i + 1];
				if (tmp.find_first_of("--") == std::string::npos)
				{
					argument = tmp;
					argument.erase(remove(argument.begin(), argument.end(), '"'), argument.end());
					++i;
				}
			}

			flags[value.substr(2)] = argument;
		}
	}

	bool CommandLine::RedirectIOToConsole()
	{
#ifdef WINDOWS
		// Allocate a console for this app
		if (!AllocConsole())
			return false;

		// Redirect unbuffered STDOUT to the console
		HANDLE lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		int hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);
		FILE* fp = _fdopen(hConHandle, "w");
		if (fp == NULL)
			return false;

		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);

		// Redirect unbuffered STDIN to the console
		lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
		hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);
		fp = _fdopen(hConHandle, "r");
		if (fp == NULL)
			return false;

		*stdin = *fp;
		setvbuf(stdin, NULL, _IONBF, 0);

		// Redirect unbuffered STDERR to the console
		lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
		hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);
		fp = _fdopen(hConHandle, "w");
		if (fp == NULL)
			return false;

		*stderr = *fp;
		setvbuf(stderr, NULL, _IONBF, 0);

		/*
		 *	Make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
		 *	point to console as well.
		 */
		return std::ios::sync_with_stdio();
#else
		return true;
#endif
	}

	std::string CommandLine::GetFlagArgument(const std::string& value) const
	{
		for(auto& flag : flags)
		{
			if (flag.first == value)
				return flag.second;
		}

		return "";
	}
	
	bool CommandLine::IsFlagSet(const std::string& value) const
	{
		for(auto& flag : flags)
		{
			if (flag.first == value)
				return true;
		}

		return false;
	}
}
