#ifndef CORE_COREUTILITY_THREADPOOL
#define CORE_COREUTILITY_THREADPOOL

#include <vector>
#include <deque>

#include <thread>
#include <mutex>
#include <condition_variable>

#include "Worker.hpp"

namespace Core
{


	class ThreadPool
	{

	public:
		/*!
			Default contructor, does nothing.
		*/
		ThreadPool();

		/*!
			If there are no threads already, initialize a list of nrThreads number of std::thread instances.
		*/
		void Initialize( int nrThreads );
		
		/*!
			Add job to the job queue. Jobs will be performed in queue order.
		*/
		void Enqueue( std::function<void()> job );
		
		/*!
			Will return true if there are no threads working at the moment.
		*/
		bool IsDone();
		
		/*!
			Will stall until all threads are finished.
		*/
		void Wait();
		
		/*!
			Will join all allocated threads. The Threadpool may be reinitialized after this.
		*/
		void KillThreads();
		
		/*!
			Will return true if there are any threads allocated at the moment.
		*/
		bool HasThreads();

		/*!
			Will return the current number of threads available.
		*/
		int GetNrThreads() { return m_nrThreads; }

		~ThreadPool();

	private:
		friend class Worker;

		int m_nrThreads;

		std::deque< std::function<void()> > m_tasks;

		std::mutex m_queueMutex;
		std::condition_variable m_condition;
	
		bool m_stop;
		int m_nrJobsInProgress;
	};
}

#endif