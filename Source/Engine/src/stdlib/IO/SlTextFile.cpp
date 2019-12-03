/*
 *	textfile.cpp
 *	Standard Library Text File
 */

#include <fstream>
#include <string>
#include <streambuf>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace TextFile
	{
		/*
			handle
			filename
			mode

			Constructor(filename, mode);
			Deconstructor();
			
			getPosition();
			getSize();
			getFilename();
			getMode();
			isOpen();
			atEof();
			close();

			seek(offset, origin);

			read(count);
			readLine();
			readToEnd();

			write(value);
			writeLine(value);

			Mode { Read = 0x01, Write = 0x02, Truncate = 0x04, Append = 0x08 }
			Origin { Begin, Current, End }
		 */

		enum Mode
		{
			Read		= 0x01,
			Write		= 0x02,
			Truncate	= 0x04,
			Append		= 0x08
		};

		enum Origin
		{
			Begin,
			Current,
			End
		};

		void ConstructorFilename(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::Integer);
			
			std::string filename = *stack.Pop();
			long long mode = *stack.Pop();

			if ((mode & Read) != Read && (mode & Write) != Write)
			{
				Error << "TextFile(): Read nor Write flag were set." << lendl;
				return;
			}

			unsigned int fstreamMode = 0x00;
			fstreamMode |= (mode & Read) == Read ? std::fstream::in : 0;
			fstreamMode |= (mode & Write) == Write ? std::fstream::out : 0;
			fstreamMode |= (mode & Truncate) == Truncate ? (std::fstream::trunc | std::fstream::out) : 0;
			fstreamMode |= (mode & Append) == Append ? (std::fstream::app | std::fstream::ate) : 0;

			std::fstream* handle = new std::fstream(filename.c_str(), fstreamMode);
			if (handle == nullptr || !handle->is_open())
			{
				Error << "TextFile(): Cannot open file \""
					  << filename
					  << "\"."
					  << lendl;
				return;
			}

			parent->Field("__handle") = Vm::Variant(handle, nullptr);
			parent->Field("filename") = filename;
			parent->Field("mode") = mode;
		}

		void Deconstructor(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
				return;

			handle->close();
			delete handle;
			parent->Field("__handle").SetEngineObject(nullptr);
		}

		void GetPosition(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "GetPosition(): File is not open." << lendl;
				StackPush(0);
				return;
			}

			unsigned int mode = parent->Field("mode");

			StackPush(static_cast<int>((mode & Read) == Read ? handle->tellg() : handle->tellp()));
		}

		void SetPosition(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			int position = *stack.Pop();

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "SetPosition(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");

			if ((mode & Read) == Read)
				handle->seekg(position, std::fstream::beg);
			else if ((mode & Write) == Write)
				handle->seekp(position, std::fstream::beg);
		}

		void GetSize(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "GetSize(): File is not open." << lendl;
				StackPush(0);
				return;
			}

			unsigned int mode = parent->Field("mode");
			long long size = 0;

			if ((mode & Read) == Read)
			{
				std::fstream::pos_type pos = handle->tellg();
				handle->seekg(0, std::fstream::end);
				size = static_cast<long long>(handle->tellg());
				handle->seekg(pos, std::fstream::beg);
			}
			else
			{
				std::fstream::pos_type pos = handle->tellp();
				handle->seekp(0, std::fstream::end);
				size = static_cast<long long>(handle->tellp());
				handle->seekp(pos, std::fstream::beg);
			}

			StackPush(size);
		}

		void GetFilename(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			StackPush(parent->Field("filename"));
		}

		void GetMode(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			StackPush(parent->Field("mode"));
		}

		void IsOpen(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			StackPush(parent->Field("__handle").EngineObject<std::fstream>() != nullptr);
		}

		void AtEof(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				StackPush(true);
				return;
			}

			StackPush(handle->eof());
		}

		void Close(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
				return;

			handle->close();
			delete handle;
			parent->Field("__handle").SetEngineObject(nullptr);
		}

		void Seek(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);
			ExpectType(1, Vm::Variant::Type::Integer);

			long long offset = *stack.Pop();
			unsigned int origin = *stack.Pop();

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "Seek(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			unsigned int fstreamOrigin = 0;
			if (origin == Begin)
				fstreamOrigin = std::fstream::beg;
			else if (origin == Current)
				fstreamOrigin = std::fstream::cur;
			else if (origin == End)
				fstreamOrigin = std::fstream::end;
			else
			{
				Error << "Seek(): Origin must be Begin, Current or End." << lendl;
				return;
			}

			if ((mode & Read) == Read)
				handle->seekg(offset, fstreamOrigin);
			else
				handle->seekp(offset, fstreamOrigin);
		}

		void FileRead(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			unsigned int count = *stack.Pop();
			std::string value(count, '\0');

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "Read(): File is not open." << lendl;
				StackPush("");
				return;
			}

			if ((parent->Field("mode").Integer() & Read) != Read)
			{
				Error << "read(): File not opened for reading." << lendl;
				StackPush("");
				return;
			}

			handle->read(const_cast<char*>(value.data()), count);
			StackPush(value);
		}

		void ReadLine(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::string value;

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadLine(): File is not open." << lendl;
				StackPush("");
				return;
			}

			if ((parent->Field("mode").Integer() & Read) != Read)
			{
				Error << "ReadLine(): File not opened for reading." << lendl;
				StackPush("");
				return;
			}

			getline(*handle, value);
			StackPush(value);
		}

		void ReadToEnd(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::string value;

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadLine(): File is not open." << lendl;
				StackPush("");
				return;
			}

			if ((parent->Field("mode").Integer() & Read) != Read)
			{
				Error << "ReadToEnd(): File not opened for reading." << lendl;
				StackPush("");
				return;
			}
			
			handle->seekg(0, std::fstream::end);
			value.reserve(static_cast<size_t>(handle->tellg()));
			handle->seekg(0, std::fstream::beg);

			value.assign(std::istreambuf_iterator<char>(*handle), std::istreambuf_iterator<char>());
			StackPush(value);
		}

		void FileWrite(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::string value = stack.Pop()->Cast(Vm::Variant::Type::String);

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "Write(): File is not open." << lendl;
				return;
			}

			if ((parent->Field("mode").Integer() & Write) != Write)
			{
				Error << "Write(): File not opened for writing." << lendl;
				return;
			}

			*handle << value;
		}

		void WriteLine(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::string value = stack.Pop()->Cast(Vm::Variant::Type::String);

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteLine(): File is not open." << lendl;
				return;
			}

			if ((parent->Field("mode").Integer() & Write) != Write)
			{
				Error << "WriteLine(): File not opened for writing." << lendl;
				return;
			}

			*handle << value << std::endl;
		}

		/*!
			class TextFile

			properties
			Position		get set int
			Size			get int
			Filename		get string
			Mode			get int
			Opened			get int
			Eof				get int

			methods
			__Constructor	2
			Close			0
			Seek			2
			Read			1
			ReadLine		0
			ReadToEnd		0
			Write			1
			WriteLine		1

			enum Mode
			Read			1
			Write			2
			Truncate		4
			Append			8
			endenum

			enum Origin
			Begin			0
			Current			1
			End				2
			endenum
		 */
		void Install()
		{
			// TextFile class
			Vm::Class textFileClass;

			// Fields
			textFileClass["__handle"] = Vm::Variant(nullptr, nullptr);
			textFileClass["filename"] = "";
			textFileClass["mode"] = 0;

			// Methods
			textFileClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorFilename, 2);
			textFileClass.AddMethod(VM_CLASS_DTOR_NAME, Deconstructor, 0);

			textFileClass.AddMethod("__get_Position", GetPosition, 0);
			textFileClass.AddMethod("__set_Position", SetPosition, 1);
			textFileClass.AddMethod("__get_Size", GetSize, 0);
			textFileClass.AddMethod("__get_Filename", GetFilename, 0);
			textFileClass.AddMethod("__get_Mode", GetMode, 0);
			textFileClass.AddMethod("__get_Opened", IsOpen, 0);
			textFileClass.AddMethod("__get_Eof", AtEof, 0);

			textFileClass.AddMethod("Close", Close, 0);
			textFileClass.AddMethod("Seek", Seek, 2);

			textFileClass.AddMethod("Read", FileRead, 1);
			textFileClass.AddMethod("ReadLine", ReadLine, 0);
			textFileClass.AddMethod("ReadToEnd", ReadToEnd, 0);
			textFileClass.AddMethod("Write", FileWrite, 1);
			textFileClass.AddMethod("WriteLine", WriteLine, 1);

			VirtualMachine.AddClass("TextFile", textFileClass);
		}
	}
}
