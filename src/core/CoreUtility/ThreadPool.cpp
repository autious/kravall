#include "Threadpool.hpp"
#include <atomic>

#include <logger/Logger.hpp>


Core::ThreadPool::ThreadPool()
{
	m_nrThreads = 0;
}

void Core::ThreadPool::Initialize( int nrThreads )
{
	if( m_nrThreads != 0 )
		return;

	m_nrJobsInProgress = nrThreads;
	m_stop = false;
	
	m_nrThreads = nrThreads;

	for( int i = 0; i < nrThreads; i++ )
		std::thread( Worker( *this ) ).detach();
}

Core::ThreadPool::~ThreadPool()
{
	if( m_nrThreads == 0 )
		return;

	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
	
	m_condition.notify_all();

	// kill yourselves, threads! ... and then kill the lock. Such beautiful!
	std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
}


void Core::ThreadPool::Enqueue( std::function<void()> job )
{
	if( m_nrThreads == 0 )
		return;

	{
		std::unique_lock<std::mutex> lock( m_queueMutex );
		m_tasks.push_back( job );
	}

	m_condition.notify_one();
}

bool Core::ThreadPool::IsDone()
{
	return m_nrJobsInProgress == 0;
}

void Core::ThreadPool::Wait()
{
	if( m_nrThreads == 0 )
		return;

	while( m_nrJobsInProgress != 0 && m_tasks.size() != 0 )
	{
		std::unique_lock<std::mutex> lock( m_waitMutex );
		m_waitCondition.wait_for( lock, std::chrono::nanoseconds(10) );
	}
}


void Core::ThreadPool::KillThreads()
{
	if( m_nrThreads == 0 )
		return;

	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }

	m_condition.notify_all();

	m_nrThreads = 0;
}


bool Core::ThreadPool::HasThreads()
{
	return m_nrThreads != 0;
}
