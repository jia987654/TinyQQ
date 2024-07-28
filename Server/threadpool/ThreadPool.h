#ifndef _THREADPOOL_THREADPOOL_H
#define _THREADPOOL_THREADPOOL_H

#include "SafeQueue.h"

#include <functional>
#include <thread>
#include <atomic>
#include <any>
#include <condition_variable>
#include <vector>

namespace threadpool
{
	using Task = std::function<void()>;

	class ThreadPool
	{
	public:
		ThreadPool(int nums);
		~ThreadPool();

		/// <summary>
		/// 添加任务
		/// </summary>
		/// <param name="task"></param>
		void addTask(Task task);

		/// <summary>
		/// 获取线程数量
		/// </summary>
		/// <returns></returns>
		int threadNums();

		/// <summary>
		/// 获取正在工作的线程数量
		/// </summary>
		/// <returns></returns>
		int activeNums();

		/// <summary>
		/// 获取空闲的线程数量
		/// </summary>
		/// <returns></returns>
		int freeNums();

	private:
		SafeQueue<Task> m_tasks;				// 任务队列

		std::vector<std::thread> m_threads;		// 所有的线程

		std::atomic_int m_nums;					// 线程的数量
		std::atomic_int m_active_nums;			// 工作的线程

		std::mutex m_lock;
		std::condition_variable m_cond;

		std::atomic_bool m_stop;				// 是否退出
	private:
		/// <summary>
		/// 消费者启动函数
		/// </summary>
		void worker();
	};
}

#endif // !_THREADPOOL_THREADPOOL_H
