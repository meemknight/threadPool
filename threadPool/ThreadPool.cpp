#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned numThreads)
{
	if (numThreads == 0) { numThreads = 1; }
	for(unsigned i=0; i<numThreads; i++)
	{
		_threads.emplace_back
		(
		[this]()
		{
			while(1)
			{
				std::mutex _muLock;
				std::unique_lock<std::mutex> lock(_muLock); //todo, check for _mu mutex				
				
				{
					std::lock_guard<std::mutex> l(_mu);
					if (_tasks.size() != 0)
					{
						goto again;
					}

				}

				_cond.wait(lock);						
				again:
				if (_shouldClose) { break; }
				
				Task t;
				{
					std::lock_guard<std::mutex> l(_mu);
					if(_tasks.size()!=0)
					{
						t = std::move(_tasks.front());
						_tasks.pop();
					}
					
				}

				t();
			}
			
		}
		);
	}

}

void ThreadPool::enque(Task t)
{
	std::lock_guard<std::mutex> lg(_mu);
	_tasks.push(t);
	_cond.notify_one();
}

void ThreadPool::join()
{
	while(1)
	{
		if(_tasks.size()==0)
		{
			break;
		}
	}
	std::atomic_bool b = 0;
	std::reference_wrapper<std::atomic_bool> done = b;
	for(int i=0; i<_threads.size(); i++)
	{
		enque
		(
		[&done]()
			{
			while (!done.get()) {}
			}
		);
	}
	while(_tasks.size()!=0){}

	b = 1;
}

ThreadPool::~ThreadPool()
{
	join();
	_shouldClose = 1;
	_cond.notify_all();
	for(auto &i: _threads)
	{
		if(i.joinable())
		{
			i.join();
		}
	}
}

ThreadPool::ThreadPool(const ThreadPool && other)
{
}
