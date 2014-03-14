#include "Worker.hpp"
#include "Threadpool.hpp"

#include <functional>

#include <iostream>


void Core::Worker::operator()()
{
	std::function< void() > task;

	while( true )
	{
		{
			std::unique_lock<std::mutex> lock( m_pool.m_queueMutex );
			m_pool.m_nrJobsInProgress--;

			m_pool.m_waitCondition.notify_all();

			while( !m_pool.m_stop && m_pool.m_tasks.empty() )
			{
				m_pool.m_condition.wait( lock );
			}

			if( m_pool.m_stop )
				return;

			m_pool.m_nrJobsInProgress++;
			task = m_pool.m_tasks.front();
			m_pool.m_tasks.pop_front();
		}

		task();
	}
}