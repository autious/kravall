#ifndef SRC_CORE_TIMER_HPP
#define SRC_CORE_TIMER_HPP

#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#define WCLOCK
#endif

namespace Core
{
	class HighresTimer
	{
#ifndef WCLOCK
		std::chrono::high_resolution_clock::time_point m_start, m_end;
#else
		private:

		LARGE_INTEGER m_frequency;        // ticks per second
		LARGE_INTEGER m_start, m_end;           // ticks
#endif
		friend HighresTimer& Timer();
		HighresTimer();
	public:
		/*!
		Starts measuring time
		*/
		void Start();

		/*!
		Stops measuring time
		*/
		void Stop();

		/*!
		Gets the delta time
		\return Returns time passed between calling Start and Stop
		*/
		std::chrono::microseconds GetDelta();

	};
	HighresTimer& Timer();
}

#endif