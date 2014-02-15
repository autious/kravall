#ifndef CORE_COREUTILITY_WORKER
#define CORE_COREUTILITY_WORKER

namespace Core
{

	class ThreadPool;

	/*!
		Worker abstraction for the threadpool. Never meant to be used separately. 
	*/
	class Worker
	{
	public:
		Worker( ThreadPool& pool ) : m_pool( pool ) { }
		void operator()();

	private:
		ThreadPool& m_pool;

	};

}


#endif