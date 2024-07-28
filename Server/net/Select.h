#ifndef _NET_SELECT_H
#define _NET_SELECT_H

#include "Selector.h"
#include "Channel.h"

#include <unordered_map>

namespace net
{
	class Selector;
	class Channel;

	class Select :public Selector
	{
	public:
		virtual ~Select() {}
		void updataChannel(Channel* ch);
		void removeChannel(Channel* ch);
		std::vector<Channel*> wait();
	private:
		int m_nfds;		// 最大的文件描述符+1
		fd_set m_rset;	// 读事件位图
		fd_set m_wset;	// 写事件位图
		fd_set m_eset;	// 错误事件位图

		// 监听的Channel
		std::unordered_map<int, Channel*> m_chs;

		// 有事件的Channel
		std::vector<Channel*> m_eventsChannel;

	private:
		/// <summary>
		/// 判断文件描述符是否加入的chs中
		/// </summary>
		/// <returns></returns>
		bool findFd(int fd);
	};
};

#endif // !_NET_SELECT_H