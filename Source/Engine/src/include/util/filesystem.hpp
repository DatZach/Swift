/*
 *	filesystem.hpp
 *	Abstracted Filesystem
 */

#ifndef __UTIL_FILESYSTEM_HPP
#define __UTIL_FILESYSTEM_HPP

#include <string>

namespace Util
{
	class Filesystem
	{
	public:
		static std::string FindFile(const std::string& mask);
	};
}

#endif
