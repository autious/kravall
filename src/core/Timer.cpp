#include "Timer.hpp"

namespace Core
{
	HighresTimer::HighresTimer()
	{
	#ifndef WCLOCK
	#else
		QueryPerformanceFrequency(&m_frequency);
	#endif
	}

	void HighresTimer::Start()
	{
	#ifndef WCLOCK
		m_start = std::chrono::high_resolution_clock::now();
	#else
		QueryPerformanceCounter(&m_start);
	#endif
	}

	void HighresTimer::Stop()
	{
	#ifndef WCLOCK
		m_end = std::chrono::high_resolution_clock::now();
	#else
		QueryPerformanceCounter(&m_end);
	#endif
	}

	std::chrono::microseconds HighresTimer::GetDelta()
	{
	#ifndef WCLOCK
		return std::chrono::duration_cast<std::chrono::microseconds>( m_end - m_start );
	#else
		double elapsed = ((m_end.QuadPart - m_start.QuadPart)*1E6)/static_cast<double>(m_frequency.QuadPart);
		std::chrono::duration<double, std::micro> ms(elapsed);
		return std::chrono::duration_cast<std::chrono::microseconds>( ms );
	#endif
	}
	HighresTimer& Timer()
	{
		static HighresTimer timer;
		return timer;
	}
}