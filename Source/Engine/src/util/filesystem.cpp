/*
 *	filesystem.cpp
 *	Abstracted Filesystem
 */

#ifdef WINDOWS
	#include <Windows.h>
#endif

#include <Util/FileSystem.hpp>

namespace Util
{
	std::string Filesystem::FindFile(const std::string& mask)
	{
		std::string path = "";

#ifdef WINDOWS
		WIN32_FIND_DATA findData;

		HANDLE hFind = FindFirstFileA(mask.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE)
			return "";

		path = findData.cFileName;

		FindClose(hFind);
#endif

		return path;
	}
}
