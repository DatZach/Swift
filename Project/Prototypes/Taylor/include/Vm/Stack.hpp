/*
 *	Stack.hpp
 *	Specialized generic stack
 */

#ifndef __UTIL_GENERIC_STACK_HPP
#define __UTIL_GENERIC_STACK_HPP

#include <cstdlib>

namespace Util
{
	template <typename T>
	class Stack
	{
	private:
		static const int STACK_SIZE = 256;

	private:
		T* stack;
		size_t size, top;

	public:
		Stack()
			: stack(nullptr),
			  size(0),
			  top(0)
		{
			
		}

		Stack(Stack<T>& other, size_t count)
			: stack(nullptr),
			  size(0),
			  top(0)
		{
			Exchange(other, count);
		}

		~Stack()
		{
			delete[] stack;
		}

		void Push(T value)
		{
			if (stack == nullptr)
			{
				size = STACK_SIZE;
				stack = new T[size];
			}
			else if (top >= size)
			{
				size <<= 1;
				stack = (T*)realloc(stack, size * sizeof(T));
			}

			stack[top++] = value;
		}

		T Pop()
		{
			return stack[--top];
		}

		T Peek() const
		{
			return stack[top - 1];
		}

		void Exchange(Stack<T>& other, size_t count)
		{
			while(count--)
				Push(other.Pop());
		}

		void Clear()
		{
			top = 0;
		}

		bool Empty() const
		{
			return Count() == 0;
		}

		size_t Count() const
		{
			return top;
		}

		T operator[](size_t i) const
		{
			return stack[i];
		}
	};
}

#endif
