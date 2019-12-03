/*
 *	slstring.cpp
 *	Standard Library String
 */

#include <algorithm>
#include <string>
#include <cctype>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace String
	{
		void Length(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			StackPush(static_cast<long long>(value.length()));
		}

		void Size(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			StackPush(static_cast<long long>(value.size()));
		}

		void ToUpper(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			transform(value.begin(), value.end(), value.begin(), toupper);
			StackPush(value);
		}

		void ToLower(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			transform(value.begin(), value.end(), value.begin(), tolower);
			StackPush(value);
		}

		void SwapCase(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			transform(value.begin(), value.end(), value.begin(),
			[=](char c)
			{
				return islower(c) ? toupper(c) : tolower(c);
			});

			StackPush(value);
		}

		void SubString(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::Integer);
			ExpectType(2, Vm::Variant::Type::Integer);

			std::string value = *stack.Pop();
			size_t index = *stack.Pop();
			size_t count = *stack.Pop();
			if (count == -1)
				count = value.length();

			StackPush(value.substr(index, count));
		}

		void Insert(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::Integer);
			ExpectType(2, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			size_t index = *stack.Pop();
			std::string sub = *stack.Pop();

			StackPush(value.insert(index, sub));
		}

		void Overwrite(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::Integer);
			ExpectType(2, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			size_t index = *stack.Pop();
			std::string overwriteValue = *stack.Pop();

			// TODO Would be better with a memcpy, too lazy right now
			size_t count = value.length();
			size_t ovrCount = overwriteValue.length();
			for(size_t i = index, j = 0; i < count && j < ovrCount; ++i, ++j)
				value[i] = overwriteValue[j];

			StackPush(value);
		}

		void Remove(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::Integer);
			ExpectType(2, Vm::Variant::Type::Integer);

			std::string value = *stack.Pop();
			size_t index = *stack.Pop();
			size_t count = *stack.Pop();

			StackPush(value.erase(index, count));
		}

		void Replace(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);
			ExpectType(2, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			std::string oldValue = *stack.Pop();
			std::string newValue = *stack.Pop();

			size_t pos = 0;
			while ((pos = value.find(oldValue, pos)) != std::string::npos)
			{
				 value.replace(pos, oldValue.length(), newValue);
				 pos += newValue.length();
			}

			StackPush(value);
		}

		void First(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);
			ExpectType(2, Vm::Variant::Type::Integer);

			std::string value = *stack.Pop();
			std::string findValue = *stack.Pop();
			size_t index = *stack.Pop();

			size_t pos = value.find(findValue, index);
			StackPush(static_cast<int>(pos == std::string::npos ? -1 : pos));
		}

		void Last(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);
			ExpectType(2, Vm::Variant::Type::Integer);

			std::string value = *stack.Pop();
			std::string findValue = *stack.Pop();
			size_t i = *stack.Pop();

			size_t x = std::string::npos;
			while(i--)
			{
				if ((x = value.find(findValue, i)) != std::string::npos)
					break;
			}

			StackPush(static_cast<int>(x == std::string::npos ? -1 : x));
		}

		void FirstStart(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			std::string findValue = *stack.Pop();

			size_t pos = value.find(findValue, 0);
			StackPush(static_cast<int>(pos == std::string::npos ? -1 : pos));
		}

		void LastStart(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			std::string findValue = *stack.Pop();

			size_t i = value.length();
			size_t x = std::string::npos;
			while(i--)
			{
				if ((x = value.find(findValue, i)) != std::string::npos)
					break;
			}

			StackPush(static_cast<int>(x == std::string::npos ? -1 : x));
		}

		void StartsWith(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			std::string subValue = *stack.Pop();

			StackPush(value.find(subValue) == 0);
		}

		void EndsWith(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			std::string subValue = *stack.Pop();

			size_t i = value.length();
			size_t x = std::string::npos;
			while(i--)
			{
				if ((x = value.find(subValue, i)) != std::string::npos)
					break;
			}

			if (x == std::string::npos)
			{
				StackPush(false);
				return;
			}

			size_t y = value.length() - subValue.length();

			StackPush(x == y);
		}

		void IsAlphaNumeric(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			size_t i = value.length();
			while(i--)
			{
				if (!isalnum(value[i]))
				{
					StackPush(false);
					return;
				}
			}

			StackPush(true);
		}

		void IsAlpha(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			size_t i = value.length();
			while(i--)
			{
				if (!isalpha(value[i]))
				{
					StackPush(false);
					return;
				}
			}

			StackPush(true);
		}

		void IsNumeric(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			size_t i = value.length();
			while(i--)
			{
				if (!isdigit(value[i]))
				{
					StackPush(false);
					return;
				}
			}

			StackPush(true);
		}

		void IsLower(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			size_t i = value.length();
			while(i--)
			{
				if (!islower(value[i]))
				{
					StackPush(false);
					return;
				}
			}

			StackPush(true);
		}

		void IsUpper(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			size_t i = value.length();
			while(i--)
			{
				if (!isupper(value[i]))
				{
					StackPush(false);
					return;
				}
			}

			StackPush(true);
		}

		void SplitLines(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			std::vector<Vm::Variant*> lines;
			std::string line;

			size_t count = value.length();
			for(size_t i = 0; i < count; ++i)
			{
				if (value[i] == '\n')
				{
					lines.push_back(VirtualMachine.CreateVariant(line));
					line = "";
				}
				else if (value[i] != '\r')
					line += value[i];
			}

			lines.push_back(VirtualMachine.CreateVariant(line));

			StackPush(lines);
		}

		void Contains(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			std::string sub = *stack.Pop();

			StackPush(value.find(sub) != std::string::npos);
		}

		void PadLeft(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);
			ExpectType(2, Vm::Variant::Type::Integer);

			std::string value = *stack.Pop();
			std::string sub = *stack.Pop();
			long long count = *stack.Pop();

			size_t len = std::max(0, static_cast<int>(count - value.length()));
			while(len--)
				value.insert(0, sub);

			StackPush(value);
		}

		void PadRight(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);
			ExpectType(2, Vm::Variant::Type::Integer);

			std::string value = *stack.Pop();
			std::string sub = *stack.Pop();
			long long count = *stack.Pop();

			size_t len = std::max(0, static_cast<int>(count - value.length()));
			while(len--)
				value.insert(value.length() - 1, sub);

			StackPush(value);
		}

		void Split(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::String);

			std::string value = *stack.Pop();
			std::string delim = *stack.Pop();

			std::vector<Vm::Variant*> items;

			size_t i = 0, j = 0, count = value.length();
			while(i < count)
			{
				i = value.find(delim, i);
				if (i == std::string::npos)
					break;

				items.push_back(VirtualMachine.CreateVariant(value.substr(j, i - j)));
				i = j = i + delim.length();
			}

			items.push_back(VirtualMachine.CreateVariant(value.substr(j, i - j)));

			StackPush(items);
		}

		void Trim(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			std::string value = *stack.Pop();

			while(value.length() && isspace(value[0]))
				value.erase(0, 1);

			while(value.length() && isspace(value[value.length() - 1]))
				value.erase(value.length() - 1, 1);

			StackPush(value);
		}

		void Format(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);
			ExpectType(1, Vm::Variant::Type::List);

			std::string value = *stack.Pop();
			std::vector<Vm::Variant*> list = *stack.Pop();
			std::string formatted;

			for(size_t i = 0; value[i];)
			{
				if (value[i] == '%')
				{
					if (value[++i] == '%')
					{
						formatted += '%';
						continue;
					}

					std::string tmp;
					size_t index;

					while(isdigit(value[i]))
						tmp += value[i++];

					std::stringstream ss(tmp);
					ss >> index;

					if (index >= list.size() || index < 0)
					{
						formatted += tmp;
						continue;
					}

					formatted += list[index]->Cast(Vm::Variant::Type::String).String();
				}
				else
					formatted += value[i++];
			}

			StackPush(formatted);
		}

		/*!
			ignore
		*/
		void Install()
		{
			Vm::Class stringClass;

			stringClass.AddMethod("Length", Length, 1);
			stringClass.AddMethod("Size", Size, 1);
			stringClass.AddMethod("ToUpper", ToUpper, 1);
			stringClass.AddMethod("ToLower", ToLower, 1);
			stringClass.AddMethod("SwapCase", SwapCase, 1);
			stringClass.AddMethod("SubString", SubString, 3);
			stringClass.AddMethod("Insert", Insert, 3);
			stringClass.AddMethod("Overwrite", Overwrite, 3);
			stringClass.AddMethod("Remove", Remove, 3);
			stringClass.AddMethod("Replace", Replace, 3);
			stringClass.AddMethod("First", First, 3);
			stringClass.AddMethod("Last", Last, 3);
			stringClass.AddMethod("First", FirstStart, 2);
			stringClass.AddMethod("Last", LastStart, 2);
			stringClass.AddMethod("IndexOf", First, 3);
			stringClass.AddMethod("IndexOf", FirstStart, 2);
			stringClass.AddMethod("StartsWith", StartsWith, 2);
			stringClass.AddMethod("EndsWith", EndsWith, 2);
			stringClass.AddMethod("IsAlphaNumeric", IsAlphaNumeric, 1);
			stringClass.AddMethod("IsAlpha", IsAlpha, 1);
			stringClass.AddMethod("IsNumeric", IsNumeric, 1);
			stringClass.AddMethod("IsLower", IsLower, 1);
			stringClass.AddMethod("IsUpper", IsUpper, 1);
			stringClass.AddMethod("SplitLines", SplitLines, 1);
			stringClass.AddMethod("Contains", Contains, 2);
			stringClass.AddMethod("PadLeft", PadLeft, 3);
			stringClass.AddMethod("PadRight", PadRight, 3);
			stringClass.AddMethod("Split", Split, 2);
			stringClass.AddMethod("Trim", Trim, 1);
			stringClass.AddMethod("Format", Format, 2);

			VirtualMachine.AddClass("__str", stringClass);
		}
	}
}
