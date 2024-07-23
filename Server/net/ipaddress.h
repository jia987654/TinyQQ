#ifndef _NET_IPADDRESS_H
#define _NET_IPADDRESS_H

#include <sys/socket.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <string>

namespace net
{
	class IPAddress
	{
	public:
		IPAddress(){}

		IPAddress(struct sockaddr_in addr) :
			m_addr(addr), 
			m_port(addr.sin_port) {}

		IPAddress(std::string ip, int port);
		
		~IPAddress() {}

		/// <summary>
		/// 获取ip地址
		/// </summary>
		/// <returns></returns>
		std::string getIp();

		/// <summary>
		/// 获取sockaddr_in结构体
		/// </summary>
		struct sockaddr_in* getAddr();

		/// <summary>
		/// 获取端口号
		/// </summary>
		/// <returns></returns>
		int getPort();

	private:
		struct sockaddr_in m_addr;
		int m_port;
	};
}

#endif