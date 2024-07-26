#ifndef _THREADPOOL_SAFEQUEUE_H
#define _THREADPOOL_SAFEQUEUE_H

#include <mutex>
#include <queue>

namespace threadpool
{
	template<typename T>
	class SafeQueue
	{
	public:
		SafeQueue() {}

		/// <summary>
		/// 获取队列大小
		/// </summary>
		/// <returns></returns>
		size_t size()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_queue.size();
		}

		/// <summary>
		/// 判断队列是否为空
		/// </summary>
		/// <returns></returns>
		bool empty()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_queue.empty();
		}

		/// <summary>
		/// 取出队头元素
		/// </summary>
		/// <returns></returns>
		T pop()
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			if (m_queue.empty())
			{
				return NULL;
			}

			T t = m_queue.front();
			m_queue.pop();

			return t;
		}

		/// <summary>
		/// 查看队头元素
		/// </summary>
		T front()
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			if (m_queue.empty())
			{
				return nullptr;
			}

			return m_queue.front();
		}

		/// <summary>
		/// 添加元素
		/// </summary>
		/// <param name="val"></param>
		void append(T val)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.emplace(val);
		}

	private:
		std::queue<T> m_queue;	// 队列

		std::mutex m_mutex;		// 锁
	};
}

#endif // !_THREADPOOL_SAFEQUEUE_H
