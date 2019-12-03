/*
 *	time.cpp
 *	Swift Standard Library
 *	Time
 */

#include <ctime>
#include <StdLib/SlApi.hpp>

namespace StdLib
{
	namespace Time
	{
		void GetTimestamp(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			StackPush(time(nullptr));
		}

		/*!
			class Time

			properties
			Timestamp		get static
		 */
		void Install()
		{
			// Time class
			Vm::Class classTime;

			// Methods
			classTime.AddMethod("__get_Timestamp", GetTimestamp, 0);

			VirtualMachine.AddClass("Time", classTime);
		}
	}
}
