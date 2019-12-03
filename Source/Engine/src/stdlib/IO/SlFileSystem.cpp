/*
 *	filesystem.cpp
 *	Standard Library File System
 */

#include <fstream>
#include <Windows.h>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace FileSystem
	{
		/*
			static fileExists(value);
			static directoryExists(value);

			static deleteFile(value);
			static deleteDirectory(value);

			static getFiles(filter, findmode); -- Don't include . or ..
			static getAttributes(path);
			
			static getFilename(value);
			static getExtension(value);
			static getPath(value);

			static getWorkingDirectory();
			static getTempDirectory();
		*/

		// TODO Should all be abstracted behind Util::FileSystem

		enum FindMode
		{
			Files		= 0x01,
			Directories = 0x02,
			Devices		= 0x04
		};

		enum Attributes
		{
			ReadOnly	= 0x01,
			Hidden		= 0x02,
			System		= 0x04,
			Directory	= 0x08,
			Archive		= 0x10
		};

		void FileExists(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);

			std::string filename = *stack.Pop();
			std::ifstream f(filename.c_str(), std::ifstream::in);
			StackPush(f.is_open());
		}

		void DirectoryExists(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);

			std::string filename = *stack.Pop();
			DWORD attrib = GetFileAttributesA(filename.c_str());
			if (attrib == INVALID_FILE_ATTRIBUTES)
			{
				StackPush(false);
				return;
			}

			StackPush((attrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
		}

		// NOTE DeleteFile is reserved by WinAPI, of course
		void DeleteFilePath(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);

			std::string filename = *stack.Pop();
			StackPush(remove(filename.c_str()) == 0);
		}

		void DeleteDirectory(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);

			std::string filename = *stack.Pop();
			StackPush(RemoveDirectoryA(filename.c_str()));
		}

		void GetFiles(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string filter = *stack.Pop();
			unsigned int mode = *stack.Pop();

			std::vector<Vm::Variant*> list;
			
#ifdef WINDOWS
			WIN32_FIND_DATA findData;
			HANDLE hFind = FindFirstFileA(filter.c_str(), &findData);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				StackPush(list);
				return;
			}

			do
			{
				// Is this a file? Are we not accepting files?
				// Is this a directory? Are we not accepting directories?
				if (!((mode & Files) == Files && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) &&
					!((mode & Directories) == Directories && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
					continue;

				// Don't accept . or ..
				if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
					continue;

				list.push_back(VirtualMachine.CreateVariant(findData.cFileName));
			}
			while(FindNextFileA(hFind, &findData));

			FindClose(hFind);
#endif

			StackPush(list);
		}

		void GetAttributes(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string path = *stack.Pop();
			int flags = 0;

#ifdef WINDOWS
			DWORD winFlags = GetFileAttributesA(path.c_str());
			if (winFlags != INVALID_FILE_ATTRIBUTES)
			{
				flags |= (winFlags & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY ? ReadOnly : 0;
				flags |= (winFlags & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN ? Hidden : 0;
				flags |= (winFlags & FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM ? System : 0;
				flags |= (winFlags & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY ? Directory : 0;
				flags |= (winFlags & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE ? Archive : 0;
			}
#endif

			StackPush(flags);
		}

		void GetFilename(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();
			std::string oValue = value;

			// Unify the path delimiters
			size_t i = 0;
			while(true)
			{
				i = value.find("\\", i);
				if (i == std::string::npos)
					break;

				value.replace(i++, 1, "/");
			}

			// Get the position of the filename
			size_t pathEnd = value.rfind('/');
			if (pathEnd == std::string::npos)
			{
				StackPush("");
				return;
			}

			StackPush(oValue.substr(pathEnd + 1, value.length() - pathEnd - 1));
		}

		void GetExtension(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			size_t filenameEnd = value.rfind('.');
			if (filenameEnd == std::string::npos)
			{
				StackPush("");
				return;
			}

			StackPush(value.substr(filenameEnd + 1, value.length() - filenameEnd - 1));
		}

		void GetPath(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();
			std::string oValue = value;

			// Unify the path delimiters
			size_t i = 0;
			while(true)
			{
				i = value.find("\\", i);
				if (i == std::string::npos)
					break;

				value.replace(i++, 1, "/");
			}

			// Get the position of the filename
			size_t pathEnd = value.rfind('/');
			if (pathEnd == std::string::npos)
			{
				StackPush("");
				return;
			}

			StackPush(oValue.substr(0, pathEnd));
		}

		void GetWorkingDirectory(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			char value[MAX_PATH + 1];
			GetCurrentDirectoryA(MAX_PATH + 1, value);
			
			StackPush(value);
		}

		void GetTempDirectory(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			char value[MAX_PATH + 1];
			GetTempPathA(MAX_PATH + 1, value);
			
			StackPush(value);
		}

		/*!
			class FileSystem

			properties
			WorkingDirectory		get static
			TempDirectory			get static

			methods
			FileExists				1 static
			DirectoryExists			1 static
			DeleteFile				1 static
			DeleteDirectory			1 static
			GetFiles				2 static
			GetAttributes			1 static
			GetFilename				1 static
			GetExtension			1 static
			GetPath					1 static

			enum FileMode
			Files					1
			Directories				2
			Devices					4
			All						7
			endenum

			enum Attributes
			ReadOnly				1
			Hidden					2
			System					4
			Directory				8
			Archive					16
			endenum
		 */
		void Install()
		{
			Vm::Class fileSystemClass;

			// Methods
			fileSystemClass.AddMethod("FileExists", FileExists, 1);
			fileSystemClass.AddMethod("DirectoryExists", DirectoryExists, 1);
			fileSystemClass.AddMethod("DeleteFile", DeleteFilePath, 1);
			fileSystemClass.AddMethod("DeleteDirectory", DeleteDirectory, 1);
			fileSystemClass.AddMethod("GetFiles", GetFiles, 2);
			fileSystemClass.AddMethod("GetAttributes", GetAttributes, 1);
			fileSystemClass.AddMethod("GetFilename", GetFilename, 1);
			fileSystemClass.AddMethod("GetExtension", GetExtension, 1);
			fileSystemClass.AddMethod("GetPath", GetPath, 1);
			fileSystemClass.AddMethod("__get_WorkingDirectory", GetWorkingDirectory, 0);
			fileSystemClass.AddMethod("__get_TempDirectory", GetTempDirectory, 0);

			VirtualMachine.AddClass("FileSystem", fileSystemClass);
		}
	}
}
