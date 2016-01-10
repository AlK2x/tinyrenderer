#pragma once
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

typedef std::function<void()> fn_type;

class Worker
{
public:
	Worker()
		:m_enabled(true),
		m_fqueue(),
		m_thread(&Worker::thread_fn, this)
	{}

	~Worker()
	{
		m_enabled = false;
		m_cv.notify_one();
		m_thread.join();
	}
	
	void appendFn(fn_type fn)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_fqueue.push(fn);
		m_cv.notify_one();
	}

	size_t getTaskCount()
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		return m_fqueue.size();
	}

	bool isEmpty()
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		return m_fqueue.empty();
	}

private:
	bool                    m_enabled;
	std::condition_variable m_cv;
	std::queue<fn_type>     m_fqueue;
	std::mutex              m_mutex;
	std::thread             m_thread;

	void thread_fn();
};

typedef std::shared_ptr<Worker> worker_ptr;
