#ifndef _NET_SOCKET_H
#define _NET_SOCKET_H

#include "ipaddress.h"

namespace net
{
	class IPAddress;

	class Socket
	{
	public:
		Socket(int fd) : 
			m_fd(fd) {}
		Socket() {}

		/// <summary>
		/// 设置文件描述符
		/// </summary>
		/// <param name="fd"></param>
		void setFd(int fd);

		/// <summary>
		/// 获取文件描述符
		/// </summary>
		/// <param name="fd"></param>
		int getFd();

		/// <summary>
		/// 绑定socket
		/// </summary>
		/// <param name="addr"></param>
		void bind(IPAddress &addr);

		/// <summary>
		/// 开启监听
		/// </summary>
		/// <param name="nums"></param>
		void listen(int nums);

		/// <summary>
		/// 接受连接
		/// </summary>
		/// <param name="addr">客户端地址</param>
		/// <returns>客户端文件描述符</returns>
		int accept(IPAddress* addr);

		/// <summary>
		/// 设置为非堵塞
		/// </summary>
		void nonBlocking();

	private:
		int m_fd;
	};
}

#endif