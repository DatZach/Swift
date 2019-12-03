/*
 *	stopwatch.hpp
 *	Stopwatch
 */

#ifndef __UTILITY_STOPWATCH_HPP
#define __UTILITY_STOPWATCH_HPP

namespace Util
{
	class Stopwatch
	{
	private:
		double startTime;
		double endTime;

	public:
		Stopwatch();

		void Start();
		void Stop();

		double GetElapsedMilliseconds() const;
	};
}

#endif
