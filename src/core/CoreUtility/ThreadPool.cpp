#include "Threadpool.hpp"
#include <atomic>

#include <logger/Logger.hpp>


Core::ThreadPool::ThreadPool()
{
}

void Core::ThreadPool::Initialize( int nrThreads )
{
	if( m_threads.size() != 0 )
		return;

	m_nrJobsInProgress = nrThreads;
	m_stop = false;
	
	for( int i = 0; i < nrThreads; i++ )
		m_threads.push_back( std::thread( Worker( *this ) ) );
}

Core::ThreadPool::~ThreadPool()
{
	if( m_threads.size() == 0 )
		return;

	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
	
	m_condition.notify_all();

	for( int i = 0; i < m_threads.size(); i++ )
		m_threads[i].join();
}


void Core::ThreadPool::Enqueue( std::function<void()> job )
{
	if( m_threads.size() == 0 )
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
	if( m_threads.size() == 0 )
		return;

	while( m_nrJobsInProgress != 0 )
		if( m_nrJobsInProgress < 0  )
			LOG_FATAL << "Threadpool reports negative number of jobs!" << std::endl;
}


void Core::ThreadPool::KillThreads()
{
	if( m_threads.size() == 0 )
		return;

	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
	
	m_condition.notify_all();

	for( int i = 0; i < m_threads.size(); i++ )
		m_threads[i].join();
}


bool Core::ThreadPool::HasThreads()
{
	return m_threads.size() != 0;
}