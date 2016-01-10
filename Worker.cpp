#include "Worker.h"

void Worker::thread_fn()
{
	while (m_enabled)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_cv.wait(locker, [&]() { return !m_fqueue.empty() || !m_enabled; });
		while (!m_fqueue.empty())
		{
			fn_type fn = m_fqueue.front();
			locker.unlock();
			fn();
			locker.lock();
			m_fqueue.pop();
		}
	}
}
