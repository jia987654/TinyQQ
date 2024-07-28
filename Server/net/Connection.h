#ifndef _NET_CONNECTION_H
#define _NET_CONNECTION_H

#include "Channel.h"
#include "EventLoop.h"
#include "socket.h"

#include "buffer/Buffer.h"

#include <functional>
#include <memory>
#include <string>

namespace net
{
	class Channel;
	class Socket;
	class EventLoop;
	class Connection;

	using ConnectCallback = std::function<void(Connection* conn)>;
	using ReadCallback = std::function<void(Connection* conn, buffer::BufferPtr, int len)>;
	using WriteCallback = std::function<void(Connection* conn)>;
	using CloseCallback = std::function<void(Connection* conn)>;

	class Connection
	{
	public:
		Connection(std::string name, EventLoopPtr loop, int fd, IPAddress addr);
		~Connection();

		/// <summary>
		/// 注册连接
		/// </summary>
		void connect();

		/// <summary>
		/// 获取连接名称
		/// </summary>
		/// <returns></returns>
		std::string getName();

		/// <summary>
		/// 获取连接所属的事件循环
		/// </summary>
		EventLoop* getLoop();

		/// <summary>
		/// 获取连接的地址
		/// </summary>
		/// <returns></returns>
		IPAddress getAddr();

		/// <summary>
		/// 获取输入缓冲
		/// </summary>
		/// <returns></returns>
		buffer::BufferPtr BuffergetRecvBuffer()
		{
			return m_readBuffer;
		}

		/// <summary>
		/// 获取输出缓冲
		/// </summary>
		/// <returns></returns>
		buffer::BufferPtr getSendBuffer()
		{
			return m_writeBuffer;
		}

		/// <summary>
		/// 将输出缓冲的内容发给客户端
		/// </summary>
		void send();

		/// <summary>
		/// 设置建立连接的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setConnectCallback(ConnectCallback callback);

		/// <summary>
		/// 设置读取消息的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setReadCallback(ReadCallback callback);

		/// <summary>
		/// 设置写完消息时的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setWriteCallback(WriteCallback callback);

		/// <summary>
		/// 设置连接关闭的回调函数
		/// </summary>
		/// <param name="callback"></param>
		void setCloseCallback(CloseCallback callback);

	private:
		std::string m_name;								// 连接的名称

		EventLoopPtr m_loop;							// 连接所属的事件循环

		std::unique_ptr<Channel> m_ch;					// 连接对应的Channel
		std::unique_ptr<Socket> m_sock;					// 连接对应的Socket

		IPAddress m_addr;								// 连接的地址

		buffer::BufferPtr m_readBuffer;	// 输入缓冲
		buffer::BufferPtr m_writeBuffer;	// 输出缓冲
	private:
		ConnectCallback m_connectCallback;				// 建立新连接的回调函数
		ReadCallback m_readCallback;					// 有消息可读的回调函数
		WriteCallback m_writeCallback;					// 发送完消息的回调函数
		CloseCallback m_closeCallback;					// 连接关闭的回调函数
	private:
		/// <summary>
		/// 读事件回调函数
		/// </summary>
		void readCallback();

		/// <summary>
		/// 写事件回调函数
		/// </summary>
		// void writeCallback();

		/// <summary>
		/// 断开连接事件回调函数
		/// </summary>
		void hubCallback();
	};
}

#endif