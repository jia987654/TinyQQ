#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "socket.h"
#include "ipaddress.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Connection.h"

#include "threadpool/ThreadPool.h"

#include <unordered_map>
#include <memory>

namespace net
{
	class Socket;
	class IPAddress;
	class EventLoop;
	class Acceptor;
	class Connection;

	using EventLoops = std::vector<EventLoopPtr>;

	class Server
	{
	public:
		const static int childLoopsNum = 1;

		Server(IPAddress addr);
		~Server() {}

		/// <summary>
		/// 启动服务端
		/// </summary>
		void start();

		/// <summary>
		/// 关闭服务端
		/// </summary>
		void close();
	public:
		/// <summary>
		/// 设置建立连接的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setConnectCallback(ConnectCallback callback)
		{
			m_connectCallback = std::move(callback);
		}

		/// <summary>
		/// 设置读取消息的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setReadCallback(ReadCallback callback)
		{
			m_readCallback = std::move(callback);
		}

		/// <summary>
		/// 设置写完消息时的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setWriteCallback(WriteCallback callback)
		{
			m_writeCallback = std::move(callback);
		}

		/// <summary>
		/// 设置连接关闭的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setCloseCallback(CloseCallback callback)
		{
			m_closeCallback = std::move(callback);
		}

	private:
		Socket m_sock;				// 服务端socket
		IPAddress m_addr;			// 服务端地址

		EventLoopPtr m_loop;		// 主事件循环
		EventLoops m_childLoops;	// 子事件循环
		Acceptor* m_acceptor;		// 接收器

		// 子EventLoop线程
		threadpool::ThreadPool m_pool;

		int m_polling;				// 轮询到的子事件循环下标

		// 所有的连接
		std::unordered_map<std::string, std::shared_ptr<Connection>> m_conns;
	private:
		/* 用户设置的回调函数 */
		ConnectCallback m_connectCallback;	// 建立新连接的回调函数
		ReadCallback m_readCallback;		// 有消息可读的回调函数
		WriteCallback m_writeCallback;		// 发送完消息的回调函数
		CloseCallback m_closeCallback;		// 连接关闭的回调函数
	private:
		/// <summary>
		/// 收到新连接的回调函数
		/// </summary>
		/// <param name="cfd">新连接的客户端socket描述符</param>
		/// <param name="addr">新连接的客户端地址</param>
		/// <returns></returns>
		void newConnect(int cfd, IPAddress* addr);

		/// <summary>
		/// 移除连接
		/// </summary>
		/// <param name="conn">连接</param>
		void removeConnect(Connection* conn);

		/// <summary>
		/// 轮询事件循环
		/// </summary>
		/// <returns></returns>
		EventLoopPtr polling();
	};
}

#endif // !_NET_SERVER_H
