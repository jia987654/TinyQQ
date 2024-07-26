#ifndef _NET_CHANNEL_H
#define _NET_CHANNEL_H

#include "EventLoop.h"
#include "ipaddress.h"
#include "socket.h"

#include <functional>
#include <memory>
#include <sys/epoll.h>

#define IO_BUFFER_SIZE 512

namespace net
{
	class EventLoop;

	enum
	{
		EventRead	= 0x001,
		EventWrite	= 0x002,
		EventHub	= 0x008,
		EventError	= 0x10,
	};

	using Callback = std::function<void()>;
	using EventLoopPtr = std::shared_ptr<EventLoop>;

	class Channel
	{
	public:
		Channel(EventLoopPtr loop, int fd);
		~Channel() {}

		/// <summary>
		/// 获取文件描述符
		/// </summary>
		/// <returns></returns>
		int getFd();

		/// <summary>
		/// 读取感兴趣的事件
		/// </summary>
		/// <returns>事件</returns>
		int getEvents();

		/// <summary>
		/// 返回事件
		/// </summary>
		/// /// <param name="events">事件</param>
		void returnEvents(int events);

		/// <summary>
		/// 清理返回的事件
		/// </summary>
		void clearRevents();

		/// <summary>
		/// 判断是否连接到Selector中
		/// </summary>
		/// <returns></returns>
		bool isConnect();

		/// <summary>
		/// 加入到Selector中
		/// </summary>
		/// <param name="state"></param>
		void connect();

		/// <summary>
		/// 从Selector中移除
		/// </summary>
		void remove();

		/// <summary>
		/// 禁用所有事件
		/// </summary>
		void disableAll();

		/// <summary>
		/// 设置事件
		/// </summary>
		/// <param name="events">设置的事件</param>
		void setEvents(int events);

		/// <summary>
		/// 设置读事件
		/// </summary>
		void setReadEvent();

		/// <summary>
		/// 设置写事件
		/// </summary>
		void setWriteEvent();

		void setAcceptCallback(Callback callback)
		{
			m_acctptCallback = std::move(callback);
		}

		/// <summary>
		/// 设置读事件回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setReadCallback(Callback callback)
		{
			m_readCallback = std::move(callback);
		}

		/// <summary>
		/// 设置写事件回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setWriteCallback(Callback callback)
		{
			m_writeCallback = std::move(callback);
		}

		/// <summary>
		/// 设置断开连接事件回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setHubCallback(Callback callback)
		{
			m_hubCallback = std::move(callback);
		}

		/// <summary>
		/// 设置错误事件回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setErrorCallback(Callback callback)
		{
			m_errorCallback = std::move(callback);
		}

		/// <summary>
		/// 获取接受连接事件回调函数
		/// </summary>
		/// <returns></returns>
		Callback getAcceptCallback()
		{
			return m_acctptCallback;
		}

		/// <summary>
		/// 获取读取事件回调函数
		/// </summary>
		/// <returns></returns>
		Callback getReadCallback()
		{
			return m_readCallback;
		}

		/// <summary>
		/// 获取写入事件回调函数
		/// </summary>
		/// <returns></returns>
		Callback getWriteCallback()
		{
			return m_writeCallback;
		}

		/// <summary>
		/// 获取断开连接事件回调函数
		/// </summary>
		/// <returns></returns>
		Callback getHubCallback()
		{
			return m_hubCallback;
		}

		/// <summary>
		/// 获取错误事件回调函数
		/// </summary>
		/// <returns></returns>
		Callback getErrorCallback()
		{
			return m_errorCallback;
		}

		/// <summary>
		/// 处理事件
		/// </summary>
		void handle();

	private:
		int m_fd;			// 所监听的socket文件描述符

		int m_events;		// 注册的事件
		int m_revents;		// 返回的事件

		bool m_isConnect;	// 是否连接到Selector

		EventLoopPtr m_loop;// 所在循环

	private:
		Callback m_readCallback;	// 读取数据回调
		Callback m_writeCallback;	// 写入数据回调
		Callback m_hubCallback;		// 断开连接回调
		Callback m_errorCallback;	// 错误事件回调
		Callback m_acctptCallback;	// 接受连接回调

		char* m_readBuffer;
		char* m_writeBuffer;

	private:
		/// <summary>
		/// 更新Channel到EventLoop
		/// </summary>
		void updata();
	};
}

#endif // !_NET_CHANNEL_H
