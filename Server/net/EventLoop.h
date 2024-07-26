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
		/// 启动事件循环
		/// </summary>
		void run();

		/// <summary>
		/// 退出事件循环
		/// </summary>
		void quite();

		/// <summary>
		/// 更新Channel
		/// </summary>
		/// <param name="ch">Channel</param>
		void updataChannel(Channel* ch);

		/// <summary>
		/// 移除Channel
		/// </summary>
		/// <param name="ch">Channel</param>
		void removeChannel(Channel* ch);

		/// <summary>
		/// 设置run开始时的回调函数
		/// </summary>
		/// <param name="call"></param>
		// void setCallback(Callback call)
		// {
		// 	m_startCallback = std::move(call);
		// }

	;	private:

		std::unique_ptr<Selector> m_selector;	// 选择器

		// Callback m_startCallback;				// 每次run开始时调用的回调函数
		std::vector<Channel*> m_Channels;		// 加入的Channel

		std::atomic_bool m_quick;				// 当退出时设置为true
	};
}

#endif // !_NET_EVENTLOOP_H
