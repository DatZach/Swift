/*
 *	binaryfile.cpp
 *	Standard Library Binary File
 */

#include <fstream>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace BinaryFile
	{
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
				Error << "BinaryFile(): Read nor Write flag were set." << lendl;
				return;
			}

			unsigned int fstreamMode = std::fstream::binary;
			fstreamMode |= (mode & Read) == Read ? std::fstream::in : 0;
			fstreamMode |= (mode & Write) == Write ? std::fstream::out : 0;
			fstreamMode |= (mode & Truncate) == Truncate ? (std::fstream::trunc | std::fstream::out) : 0;
			fstreamMode |= (mode & Append) == Append ? (std::fstream::app | std::fstream::ate) : 0;

			std::fstream* handle = new std::fstream(filename.c_str(), fstreamMode);
			if (handle == nullptr || !handle->is_open())
			{
				Error << "BinaryFile(): Cannot open file \""
					  << filename
					  << "\"."
					  << lendl;
				return;
			}

			parent->Field("__handle").SetEngineObject(handle);
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

		void ReadByte(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadByte(): File is not open." << lendl;
				StackPush(0);
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Read) != Read)
			{
				Error << "ReadByte(): File is not opened for reading." << lendl;
				StackPush(0);
				return;
			}

			unsigned char value = 0;
			handle->read(reinterpret_cast<char*>(&value), sizeof(unsigned char));
			StackPush(value);
		}

		void ReadWord(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadWord(): File is not open." << lendl;
				StackPush(0);
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Read) != Read)
			{
				Error << "ReadWord(): File is not opened for reading." << lendl;
				StackPush(0);
				return;
			}

			unsigned short value = 0;
			handle->read(reinterpret_cast<char*>(&value), sizeof(unsigned short));
			StackPush(value);
		}

		void ReadDword(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadDword(): File is not open." << lendl;
				StackPush(0);
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Read) != Read)
			{
				Error << "ReadDword(): File is not opened for reading." << lendl;
				StackPush(0);
				return;
			}

			unsigned int value = 0;
			handle->read(reinterpret_cast<char*>(&value), sizeof(unsigned int));
			StackPush(static_cast<long long>(value));
		}

		void ReadQword(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadQword(): File is not open." << lendl;
				StackPush(0);
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Read) != Read)
			{
				Error << "ReadQword(): File is not opened for reading." << lendl;
				StackPush(0);
				return;
			}

			long long value = 0;
			handle->read(reinterpret_cast<char*>(&value), sizeof(unsigned long long));
			StackPush(value);
		}

		void ReadDouble(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadDouble(): File is not open." << lendl;
				StackPush(0.0);
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Read) != Read)
			{
				Error << "ReadDouble(): File is not opened for reading." << lendl;
				StackPush(0.0);
				return;
			}

			double value = 0.0;
			handle->read(reinterpret_cast<char*>(&value), sizeof(double));
			StackPush(value);
		}

		void ReadSingle(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadSingle(): File is not open." << lendl;
				StackPush(0.0);
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Read) != Read)
			{
				Error << "ReadSingle(): File is not opened for reading." << lendl;
				StackPush(0.0);
				return;
			}

			float value = 0.0f;
			handle->read(reinterpret_cast<char*>(&value), sizeof(float));
			StackPush(value);
		}

		void ReadString(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "ReadString(): File is not open." << lendl;
				StackPush("");
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Read) != Read)
			{
				Error << "ReadString(): File is not opened for reading." << lendl;
				StackPush("");
				return;
			}

			unsigned int length = 0;
			handle->read(reinterpret_cast<char*>(&length), sizeof(unsigned int));
			if (length == 0)
			{
				StackPush("");
				return;
			}

			std::string value(length, '\0');
			handle->read(const_cast<char*>(value.data()), length);
			StackPush(value);
		}

		void WriteByte(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			unsigned char value = static_cast<unsigned char>(stack.Pop()->Integer());

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteByte(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Write) != Write)
			{
				Error << "WriteByte(): File is not opened for writing." << lendl;
				return;
			}

			handle->write(reinterpret_cast<char*>(&value), sizeof(unsigned char));
		}

		void WriteWord(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			unsigned short value = static_cast<unsigned short>(stack.Pop()->Integer());

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteWord(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Write) != Write)
			{
				Error << "WriteWord(): File is not opened for writing." << lendl;
				return;
			}

			handle->write(reinterpret_cast<char*>(&value), sizeof(unsigned short));
		}

		void WriteDword(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			unsigned int value = static_cast<unsigned int>(stack.Pop()->Integer());

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteDword(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Write) != Write)
			{
				Error << "WriteDword(): File is not opened for writing." << lendl;
				return;
			}

			handle->write(reinterpret_cast<char*>(&value), sizeof(unsigned int));
		}

		void WriteQword(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Integer);

			unsigned long long value = static_cast<unsigned long long>(stack.Pop()->Integer());

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteQword(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Write) != Write)
			{
				Error << "WriteQword(): File is not opened for writing." << lendl;
				return;
			}

			handle->write(reinterpret_cast<char*>(&value), sizeof(unsigned long long));
		}

		void WriteDouble(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Double);

			double value = stack.Pop()->Double();

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteDouble(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Write) != Write)
			{
				Error << "WriteDouble(): File is not opened for writing." << lendl;
				return;
			}

			handle->write(reinterpret_cast<char*>(&value), sizeof(double));
		}

		void WriteSingle(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Double);

			float value = static_cast<float>(stack.Pop()->Double());

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteSingle(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Write) != Write)
			{
				Error << "WriteSingle(): File is not opened for writing." << lendl;
				return;
			}

			handle->write(reinterpret_cast<char*>(&value), sizeof(float));
		}

		void WriteString(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);

			std::string value = *stack.Pop();

			std::fstream* handle = parent->Field("__handle").EngineObject<std::fstream>();
			if (handle == nullptr)
			{
				Error << "WriteString(): File is not open." << lendl;
				return;
			}

			unsigned int mode = parent->Field("mode");
			if ((mode & Write) != Write)
			{
				Error << "WriteString(): File is not opened for writing." << lendl;
				return;
			}

			unsigned int length = value.length();
			handle->write(reinterpret_cast<char*>(&length), sizeof(unsigned int));
			handle->write(const_cast<char*>(value.c_str()), length);
		}

		/*!
			class BinaryFile

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
			ReadByte		0
			ReadWord		0
			ReadDword		0
			ReadQword		0
			ReadDouble		0
			ReadSingle		0
			ReadString		0
			WriteByte		1
			WriteWord		1
			WriteDword		1
			WriteQword		1
			WriteDouble		1
			WriteSingle		1
			WriteString		1

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
			// BinaryFile class
			Vm::Class binaryFileClass;

			// Fields
			binaryFileClass["__handle"] = Vm::Variant(nullptr, nullptr);
			binaryFileClass["filename"] = "";
			binaryFileClass["mode"] = 0;

			// Methods
			binaryFileClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorFilename, 2);
			binaryFileClass.AddMethod(VM_CLASS_DTOR_NAME, Deconstructor, 0);

			binaryFileClass.AddMethod("__get_Position", GetPosition, 0);
			binaryFileClass.AddMethod("__set_Position", SetPosition, 1);
			binaryFileClass.AddMethod("__get_Size", GetSize, 0);
			binaryFileClass.AddMethod("__get_Filename", GetFilename, 0);
			binaryFileClass.AddMethod("__get_Mode", GetMode, 0);
			binaryFileClass.AddMethod("__get_Opened", IsOpen, 0);
			binaryFileClass.AddMethod("__get_Eof", AtEof, 0);

			binaryFileClass.AddMethod("Close", Close, 0);
			binaryFileClass.AddMethod("Seek", Seek, 2);

			binaryFileClass.AddMethod("ReadByte", ReadByte, 0);
			binaryFileClass.AddMethod("ReadWord", ReadWord, 0);
			binaryFileClass.AddMethod("ReadDword", ReadDword, 0);
			binaryFileClass.AddMethod("ReadQword", ReadQword, 0);
			binaryFileClass.AddMethod("ReadDouble", ReadDouble, 0);
			binaryFileClass.AddMethod("ReadSingle", ReadSingle, 0);
			binaryFileClass.AddMethod("ReadString", ReadString, 0);

			binaryFileClass.AddMethod("WriteByte", WriteByte, 1);
			binaryFileClass.AddMethod("WriteWord", WriteWord, 1);
			binaryFileClass.AddMethod("WriteDword", WriteDword, 1);
			binaryFileClass.AddMethod("WriteQword", WriteQword, 1);
			binaryFileClass.AddMethod("WriteDouble", WriteDouble, 1);
			binaryFileClass.AddMethod("WriteSingle", WriteSingle, 1);
			binaryFileClass.AddMethod("WriteString", WriteString, 1);

			VirtualMachine.AddClass("BinaryFile", binaryFileClass);
		}
	}
}
