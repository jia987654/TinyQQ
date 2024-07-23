#include "ipaddress.h"

#include <stdio.h>
#include <string.h>

using namespace net;

net::IPAddress::IPAddress(std::string ip, int port) :
	m_port(port)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(m_port);

	if (inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr) != 1)
	{
		perror("Creat IP Info Failed.\n");
	}
}

std::string net::IPAddress::getIp()
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &m_addr.sin_addr, ip, sizeof(ip));

	return ip;
}

sockaddr_in* net::IPAddress::getAddr()
{
	return &m_addr;
}

int net::IPAddress::getPort()
{
	return ntohs(m_addr.sin_port);
}