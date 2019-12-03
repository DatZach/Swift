/*
 *	stack.hpp
 *	Fast replacement for STL stack
 */

#ifndef __UTIL_STACK_HPP
#define __UTIL_STACK_HPP

namespace Util
{
	template <typename T>
	class Stack
	{
	private:
		T* stack;
		size_t size, top;
	
	public:
		Stack::Stack()
			: stack(new T[256]),
			  size(256),
			  top(0)
		{
			
		}

		~Stack()
		{
			delete[] stack;
		}

		void Push(const T& value)
		{
			if (top >= size)
			{
				size += 256;
				stack = reinterpret_cast<T*>(realloc(stack, size));
			}

			stack[top++] = value;
		}

		T Pop()
		{
			return stack[--top];
		}

		T& Peek() const
		{
			return stack[top - 1];
		}

		void Clear()
		{
			top = 0;
		}

		size_t GetCount() const
		{
			return top;
		}

		const T& operator[](size_t i) const
		{
			return stack[i];
		}
	};
}

#endif
