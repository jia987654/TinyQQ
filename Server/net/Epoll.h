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
		int m_efd;						// Epoll的文件描述符
		struct epoll_event m_evs[1024];	// Epoll事件数组

		// 监听的Channel
		std::unordered_map<int, Channel*> m_chs;

		// 有事件的Channel
		std::vector<Channel*> m_eventsChannel;

	private:
		/// <summary>
		/// 更新Channel事件
		/// </summary>
		/// <param name="ch">需要更新的Channel</param>
		/// <param name="op">操作</param>
		void updata(Channel* ch, int op);
	};
};

#endif