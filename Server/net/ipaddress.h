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
		/// ��ȡip��ַ
		/// </summary>
		/// <returns></returns>
		std::string getIp();

		/// <summary>
		/// ��ȡsockaddr_in�ṹ��
		/// </summary>
		struct sockaddr_in* getAddr();

		/// <summary>
		/// ��ȡ�˿ں�
		/// </summary>
		/// <returns></returns>
		int getPort();

	private:
		struct sockaddr_in m_addr;
		int m_port;
	};
}

#endif