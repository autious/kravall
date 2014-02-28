#ifndef SRC_GFX_UTILITY_TIMER_HPP
#define SRC_GFX_UTILITY_TIMER_HPP

#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#define WCLOCK
#endif

namespace GFX
{
	class HighresTimer
	{
#ifndef WCLOCK
		std::chrono::high_resolution_clock::time_point m_start, m_end;
		std::chrono::high_resolution_clock::time_point m_totalStart;
#else
		private:

		LARGE_INTEGER m_frequency;        // ticks per second
		LARGE_INTEGER m_totalStart;
		LARGE_INTEGER m_start, m_end;           // ticks
#endif
		long long m_totalTime;
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

		/*!
		Gets the total time
		\return Returns the total time passed since the application started
		*/
		long long GetTotal();

	};
	HighresTimer& Timer();
}

#endif