#ifndef _NET_ACCEPTOR_H
#define _NET_ACCEPTOR_H

#include "EventLoop.h"
#include "Channel.h"

#include <functional>
#include <memory>

namespace net
{
	class EventLoop;
	class Channel;
	class Sokcet;
	class IPAddress;

	using NewConnectCallback = std::function<void(int, IPAddress*)>;

	class Acceptor
	{
	public:
		Acceptor(EventLoop* loop, Socket* sock, IPAddress addr);
		~Acceptor() {}

		/// <summary>
		/// 监听器开始监听
		/// </summary>
		void listen();

		/// <summary>
		/// 设置建立线连接的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setNewConnectionCallback(NewConnectCallback callback)
		{
			m_newConnectionCallback = std::move(callback);
		}
	private:
		EventLoop* m_loop;								// 接收器所属的事件循环
		
		std::unique_ptr<Channel> m_ch;					// 接收器的Channel
		std::unique_ptr<Socket> m_sock;					// 服务端的Socket

		IPAddress m_addr;								// 服务端的地址

		NewConnectCallback m_newConnectionCallback;	// 建立线连接的回调函数
	private:
		/// <summary>
		/// 负责监听的Channel的读事件回调函数
		/// </summary>
		void handleRead();
	};
}

#endif // !_NET_ACCEPTOR_H
