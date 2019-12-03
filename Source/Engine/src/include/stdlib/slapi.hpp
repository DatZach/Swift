/*
 *	slapi.hpp
 *	Standard Library API
 *	This file contains many macros to make working
 *	with the VM from an embedded environment significantly
 *	easier.
 *
 *	Macros assume function signature of:
 *		void Function(Util::Stack<Vm::Variant>& stack, Vm::Class* parent)
 */

#ifndef __STDLIB_API_HPP
#define __STDLIB_API_HPP

#include <sstream>
#include <Vm/VirtualMachine.hpp>
#include <Util/Logger.hpp>

// Stack
#define StackPush(x)			stack.Push(VirtualMachine.CreateVariant((x)))

// Types
#define StackIsType(i, t)		(stack[stack.Count() - (i) - 1]->TypeOf() == t)
#define IsType(x, t)			((x)->TypeOf() == t)
#define ExpectType(i, t)	\
	do { \
		if (stack[stack.Count() - (i) - 1]->TypeOf() != t) { \
			std::stringstream ss;	\
			ss << "Argument "	\
			   << static_cast<int>(i)	\
			   << " should be a "	\
			   << Vm::Variant::TypeName(t);	\
			throw VmException(ss.str());	\
			return;	\
		} \
	} while(0)
#define ExpectTypeNumber(i)	\
	do { \
		if (stack[stack.Count() - (i) - 1]->TypeOf() != Vm::Variant::Type::Integer && \
			stack[stack.Count() - (i) - 1]->TypeOf() != Vm::Variant::Type::Double) { \
			std::stringstream ss;	\
			ss << "Argument "	\
			   << static_cast<int>(i)	\
			   << " should be an int or double";	\
			throw VmException(ss.str());	\
			return;	\
		} \
	} while(0)

#endif
