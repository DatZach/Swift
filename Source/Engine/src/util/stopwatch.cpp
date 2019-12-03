/*
 *	stopwatch.cpp
 *	Stopwatch
 */

#include <Util/StopWatch.hpp>
#include <GLFW/glfw3.h>

namespace Util
{
	Stopwatch::Stopwatch()
		: startTime(0.0),
		  endTime(0.0)
	{

	}

	void Stopwatch::Start()
	{
		startTime = glfwGetTime();
		endTime = 0.0;
	}

	void Stopwatch::Stop()
	{
		endTime = glfwGetTime();
	}

	double Stopwatch::GetElapsedMilliseconds() const
	{
		double elapsed = endTime == 0
							? glfwGetTime() - startTime
							: endTime - startTime;

		return elapsed * 1000.0;
	}
}
