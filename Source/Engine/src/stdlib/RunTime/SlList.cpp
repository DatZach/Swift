/*
 *	sllist.cpp
 *	Standard Library List
 */

#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace List
	{
		void Head(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();

			StackPush(list.size() > 0 ? *list[0] : Vm::Variant());
		}

		void Tail(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();

			// TODO Is this list copying even needed anymore?
			std::vector<Vm::Variant*> newList = list;
			if (newList.size() > 0)
				newList.erase(newList.cbegin());

			StackPush(newList);
		}

		void Last(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();

			StackPush(list.size() > 0 ? *list[list.size() - 1] : Vm::Variant());
		}

		void Init(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			std::vector<Vm::Variant*> newList = list;
			newList.pop_back();

			StackPush(newList);
		}

		void Push(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			Vm::Variant* value = stack.Pop();

			// TODO Do we need to make a copy?
			list.push_back(VirtualMachine.CreateVariant(*value));
		}

		void PushRange(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			ExpectType(1, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			std::vector<Vm::Variant*>& value = *stack.Pop();

			size_t count = value.size();
			for(size_t i = 0; i < count; ++i)
				list.push_back(value[i]);
		}

		void Pop(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			list.pop_back();
		}

		void PopRange(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			ExpectType(1, Vm::Variant::Type::Integer);
			Vm::Variant* value = stack.Pop();
			long long count = *stack.Pop();

			while(count--)
				value->List().pop_back();
		}

		void Insert(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			ExpectType(1, Vm::Variant::Type::Integer);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			unsigned int index = *stack.Pop();
			Vm::Variant* value = stack.Pop();

			if (index >= list.size())
				index = list.size() - 1;
			else if (index < 0)
				index = 0;

			// TODO Do we need to make a copy here?
			list.insert(list.begin() + index, VirtualMachine.CreateVariant(*value));
		}

		void InsertRange(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			ExpectType(1, Vm::Variant::Type::Integer);
			ExpectType(2, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			size_t index = *stack.Pop();
			std::vector<Vm::Variant*>& value = *stack.Pop();

			if (index >= list.size())
				index = list.size() - 1;
			else if (index < 0)
				index = 0;

			// TODO Do we need to create a copy here?
			size_t count = value.size();
			for(size_t i = 0; i < count; ++i)
				list.insert(list.begin() + index + i, VirtualMachine.CreateVariant(*value[i]));
		}

		void Remove(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			Vm::Variant* value = stack.Pop();

			size_t count = list.size();
			for(size_t i = 0; i < count; ++i)
			{
				if (*list[i] != *value)
					continue;

				list.erase(list.begin() + i);
				break;
			}
		}

		void RemoveAt(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			ExpectType(1, Vm::Variant::Type::Integer);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			unsigned int index = *stack.Pop();

			if (index >= list.size() || index < 0)
				return;

			list.erase(list.begin() + index);
		}

		void Reverse(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			std::vector<Vm::Variant*> value = list;

			reverse(value.begin(), value.end());
			StackPush(value);
		}

		void IndexOf(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			Vm::Variant* value = stack.Pop();

			size_t count = list.size();
			for(size_t i = 0; i < count; ++i)
			{
				if (*list[i] != *value)
					continue;

				StackPush(static_cast<long long>(i));
				return;
			}

			StackPush(-1);
		}

		void Empty(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();

			StackPush(list.size() == 0);
		}

		void Contains(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::List);
			std::vector<Vm::Variant*>& list = *stack.Pop();
			Vm::Variant* value = stack.Pop();

			size_t count = list.size();
			for(size_t i = 0; i < count; ++i)
			{
				if (*list[i] != *value)
					continue;

				StackPush(true);
				return;
			}

			StackPush(false);
		}

		/*!
			ignore
		*/
		void Install()
		{
			Vm::Class listClass;

			listClass.AddMethod("Head", Head, 1);
			listClass.AddMethod("Tail", Tail, 1);
			listClass.AddMethod("Last", Last, 1);
			listClass.AddMethod("Init", Init, 1);
			listClass.AddMethod("Push", Push, 2);
			listClass.AddMethod("PushRange", PushRange, 2);
			listClass.AddMethod("Pop", Pop, 1);
			listClass.AddMethod("PopRange", PopRange, 2);
			listClass.AddMethod("Insert", Insert, 3);
			listClass.AddMethod("InsertRange", InsertRange, 3);
			listClass.AddMethod("Remove", Remove, 2);
			listClass.AddMethod("RemoveAt", RemoveAt, 2);
			listClass.AddMethod("Reverse", Reverse, 1);
			listClass.AddMethod("IndexOf", IndexOf, 2);
			listClass.AddMethod("Empty", Empty, 1);
			listClass.AddMethod("Contains", Contains, 2);

			VirtualMachine.AddClass("__list", listClass);
		}
	}
}
