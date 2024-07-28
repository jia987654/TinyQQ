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
		/// �������
		/// </summary>
		/// <param name="task"></param>
		void addTask(Task task);

		/// <summary>
		/// ��ȡ�߳�����
		/// </summary>
		/// <returns></returns>
		int threadNums();

		/// <summary>
		/// ��ȡ���ڹ������߳�����
		/// </summary>
		/// <returns></returns>
		int activeNums();

		/// <summary>
		/// ��ȡ���е��߳�����
		/// </summary>
		/// <returns></returns>
		int freeNums();

	private:
		SafeQueue<Task> m_tasks;				// �������

		std::vector<std::thread> m_threads;		// ���е��߳�

		std::atomic_int m_nums;					// �̵߳�����
		std::atomic_int m_active_nums;			// �������߳�

		std::mutex m_lock;
		std::condition_variable m_cond;

		std::atomic_bool m_stop;				// �Ƿ��˳�
	private:
		/// <summary>
		/// ��������������
		/// </summary>
		void worker();
	};
}

#endif // !_THREADPOOL_THREADPOOL_H
