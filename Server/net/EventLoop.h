#ifndef _NET_EVENTLOOP_H
#define _NET_EVENTLOOP_H

#include "Selector.h"
#include "threadpool/ThreadPool.h"

#include <unordered_map>
#include <memory>
#include <atomic>

namespace net 
{
	class Selector;
	class Channel;

	// using EventLoopPtr = std::shared_ptr<EventLoop>;

	using PendingFunc = std::function<void()>;

	class EventLoop : std::enable_shared_from_this<EventLoop>
	{
	public:
		EventLoop(const EventLoop&) = delete;
		EventLoop();
		/// <summary>
		/// �����¼�ѭ��
		/// </summary>
		void run();

		/// <summary>
		/// �˳��¼�ѭ��
		/// </summary>
		void quite();

		/// <summary>
		/// ����Channel
		/// </summary>
		/// <param name="ch">Channel</param>
		void updataChannel(Channel* ch);

		/// <summary>
		/// �Ƴ�Channel
		/// </summary>
		/// <param name="ch">Channel</param>
		void removeChannel(Channel* ch);

		/// <summary>
		/// ���¼�ѭ���������߳���ִ�з���
		/// </summary>
		/// <param name="func">��Ҫִ�еķ���</param>
		void doInLoop(PendingFunc func);

		/// <summary>
		/// ����run��ʼʱ�Ļص�����
		/// </summary>
		/// <param name="call"></param>
		// void setCallback(Callback call)
		// {
		// 	m_startCallback = std::move(call);
		// }

	;private:
		std::unique_ptr<Selector> m_selector;			// ѡ����
		// Callback m_startCallback;					// ÿ��run��ʼʱ���õĻص�����
		// std::unordered_map<int, Channel*> m_Channels;	// �����Channel

		int m_wakeFd;									// ������EventLoop���ļ�������
		std::shared_ptr<Channel> m_wakeChannel;			// ������EventLoop��Channel

		std::atomic_bool m_quick;						// ���˳�ʱ����Ϊtrue
		std::thread::id m_threadId;						// ��ǰ�̵߳�Idȷ��������̵߳���

		// �����̼߳��뵽��ǰ�̵߳ķ���
		threadpool::SafeQueue<PendingFunc> m_pendingFuncions;
	private:
		/// <summary>
		/// �жϵ�ǰ�̺߳��¼�ѭ���������߳��Ƿ�Ϊͬһ���߳�
		/// </summary>
		/// <returns></returns>
		bool isInLoop();

		/// <summary>
		/// �����Ҫִ�еķ���
		/// </summary>
		void appendPendingFunc(PendingFunc func);

		/// <summary>
		/// ���Ѹ��¼�ѭ��
		/// </summary>
		void wakeup();

		/// <summary>
		/// weakup Channel���¼��ص�����
		/// </summary>
		void wakeupReadHandle();
	};
}

#endif // !_NET_EVENTLOOP_H
