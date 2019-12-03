/*
 *	Stack.hpp
 *	Stack
 */

#ifndef __VIRTUAL_MACHINE_STACK_HPP
#define __VIRTUAL_MACHINE_STACK_HPP

#include <iostream>

template <typename T>
class Stack
{
private:
	static const int STACK_SIZE = 256;

private:
	T* stack;
	size_t size, top;

public:
	Stack::Stack()
		: stack(nullptr),
		  size(0),
		  top(0)
	{
		
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
			size += STACK_SIZE;
			stack = (T*)realloc(stack, size * sizeof(T));
		}

		stack[top++] = value;
	}

	T Pop()
	{
		return stack[--top];
	}

	void Clear()
	{
		top = 0;
	}

	size_t GetCount() const
	{
		return top;
	}

	T operator[](size_t i) const
	{
		return stack[i];
	}
};

#endif
