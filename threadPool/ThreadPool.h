#pragma once
#include <thread>
#include <queue>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
	using Task = std::function<void()>;
	std::queue<Task> _tasks;
	std::vector<std::thread> _threads;
	std::mutex _mu;
	std::condition_variable _cond;
	std::atomic_bool _shouldClose = 0;
public:
	using Task = std::function<void()>;

	ThreadPool(unsigned numThreads = std::thread::hardware_concurrency() - 1);
	
	void enque(Task);
	void join();
	
	~ThreadPool();
	ThreadPool(const ThreadPool &other) = delete;
	ThreadPool(const ThreadPool &&other);
};

