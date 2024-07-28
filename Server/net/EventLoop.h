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
		/// 在事件循环所处的线程中执行方法
		/// </summary>
		/// <param name="func">需要执行的方法</param>
		void doInLoop(PendingFunc func);

		/// <summary>
		/// 设置run开始时的回调函数
		/// </summary>
		/// <param name="call"></param>
		// void setCallback(Callback call)
		// {
		// 	m_startCallback = std::move(call);
		// }

	;private:
		std::unique_ptr<Selector> m_selector;			// 选择器
		// Callback m_startCallback;					// 每次run开始时调用的回调函数
		// std::unordered_map<int, Channel*> m_Channels;	// 加入的Channel

		int m_wakeFd;									// 负责唤醒EventLoop的文件描述符
		std::shared_ptr<Channel> m_wakeChannel;			// 服务唤醒EventLoop的Channel

		std::atomic_bool m_quick;						// 当退出时设置为true
		std::thread::id m_threadId;						// 当前线程的Id确保不会跨线程调用

		// 其他线程加入到当前线程的方法
		threadpool::SafeQueue<PendingFunc> m_pendingFuncions;
	private:
		/// <summary>
		/// 判断当前线程和事件循环所处的线程是否为同一个线程
		/// </summary>
		/// <returns></returns>
		bool isInLoop();

		/// <summary>
		/// 添加需要执行的方法
		/// </summary>
		void appendPendingFunc(PendingFunc func);

		/// <summary>
		/// 唤醒该事件循环
		/// </summary>
		void wakeup();

		/// <summary>
		/// weakup Channel读事件回调函数
		/// </summary>
		void wakeupReadHandle();
	};
}

#endif // !_NET_EVENTLOOP_H
