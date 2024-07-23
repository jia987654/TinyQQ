#ifndef _NET_EPOLL_H
#define _NET_EPOLL_H

#include "Selector.h"
#include "Channel.h"

namespace net
{
	class Selector;
	class Channel;

	class Epoll :public Selector
	{
	public:
		Epoll();
		virtual ~Epoll();
		void updataChannel(Channel* ch);
		void removeChannel(Channel* ch);
		std::vector<Channel*> wait();
	private:
		int m_efd;						// Epoll���ļ�������
		struct epoll_event m_evs[1024];	// Epoll�¼�����

		// ������Channel
		std::unordered_map<int, Channel*> m_chs;

		// ���¼���Channel
		std::vector<Channel*> m_eventsChannel;

	private:
		/// <summary>
		/// ����Channel�¼�
		/// </summary>
		/// <param name="ch">��Ҫ���µ�Channel</param>
		/// <param name="op">����</param>
		void updata(Channel* ch, int op);
	};
};

#endif