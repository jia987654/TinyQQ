#ifndef _NET_EVENTLOOP_H
#define _NET_EVENTLOOP_H

#include "Selector.h"

#include <vector>
#include <memory>

#include <atomic>

namespace net 
{
	class Selector;
	class Channel;

	// using EventLoopPtr = std::shared_ptr<EventLoop>;

	class EventLoop
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
		/// ����run��ʼʱ�Ļص�����
		/// </summary>
		/// <param name="call"></param>
		// void setCallback(Callback call)
		// {
		// 	m_startCallback = std::move(call);
		// }

	;	private:

		std::unique_ptr<Selector> m_selector;	// ѡ����

		// Callback m_startCallback;				// ÿ��run��ʼʱ���õĻص�����
		std::vector<Channel*> m_Channels;		// �����Channel

		std::atomic_bool m_quick;				// ���˳�ʱ����Ϊtrue
	};
}

#endif // !_NET_EVENTLOOP_H
