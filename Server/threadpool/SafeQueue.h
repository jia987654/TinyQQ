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
		/// ��ȡ���д�С
		/// </summary>
		/// <returns></returns>
		size_t size()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_queue.size();
		}

		/// <summary>
		/// �ж϶����Ƿ�Ϊ��
		/// </summary>
		/// <returns></returns>
		bool empty()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_queue.empty();
		}

		/// <summary>
		/// ȡ����ͷԪ��
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
		/// �鿴��ͷԪ��
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
		/// ���Ԫ��
		/// </summary>
		/// <param name="val"></param>
		void append(T val)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.emplace(val);
		}

	private:
		std::queue<T> m_queue;	// ����

		std::mutex m_mutex;		// ��
	};
}

#endif // !_THREADPOOL_SAFEQUEUE_H
