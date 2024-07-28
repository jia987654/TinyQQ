#include "ThreadPool.h"

using namespace threadpool;

ThreadPool::ThreadPool(int nums) :
	m_nums(nums),
	m_stop(false)
{
	// 初始化线程
	for (int i = 0; i < m_nums; i++)
	{
		m_threads.emplace_back(std::thread(&ThreadPool::worker, this));
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(m_lock);
		m_stop = true;
	}

	m_cond.notify_all();
	for (std::thread& ths : m_threads)
	{
		if (ths.joinable()) {
			ths.join();
		}
	}
}

void ThreadPool::addTask(Task task)
{
	m_tasks.append(task);

	m_cond.notify_one();
}

int ThreadPool::threadNums()
{
	return m_nums;
}

int threadpool::ThreadPool::activeNums()
{
	return m_active_nums;
}

int threadpool::ThreadPool::freeNums()
{
	return m_nums - m_active_nums;
}

void ThreadPool::worker()
{
	while (true)
	{
		Task task;

		{
			std::unique_lock<std::mutex> lock(m_lock);

			m_cond.wait(lock, [this]{
				return m_stop || !m_tasks.empty();
				});

			if (m_stop && m_tasks.empty())
				return;

			task = std::move(m_tasks.pop());
		}

		m_active_nums++;
		task();
		m_active_nums--;
	}
}
