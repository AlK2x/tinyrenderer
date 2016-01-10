#include "ThreadPool.h"

worker_ptr ThreadPool::getFreeWorker()
{
	worker_ptr pWorker;
	size_t minTasks = UINT32_MAX;
	for (auto &it : m_workers)
	{
		if (it->isEmpty())
		{
			return it;
		}
		else if (minTasks > it->getTaskCount())
		{
			minTasks = it->getTaskCount();
			pWorker = it;
		}
	}

	return pWorker;
}
