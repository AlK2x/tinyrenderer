#pragma once
#include <vector>
#include "Worker.h"

class ThreadPool
{
public:
	ThreadPool(size_t threads = 1)
	{
		if (threads == 0)
			throw std::runtime_error("Threads must be greather then zero.");
		for (int i = 0; i < threads; ++i)
		{
			worker_ptr pWorker(new Worker);
			m_workers.push_back(pWorker);
		}
	}

	template<class _FN, class... _ARGS>
	void runAsync(_FN _fn, _ARGS... _args)
	{
		getFreeWorker()->appendFn(std::bind(_fn, _args...));
	}

	~ThreadPool() {}

private:
	worker_ptr getFreeWorker();
	std::vector<worker_ptr> m_workers;
};

