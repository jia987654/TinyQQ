#include "socket.h"

#include <stdio.h>
#include <stdlib.h>

using namespace net;

void Socket::setFd(int fd)
{
	m_fd = fd;
}

int Socket::getFd()
{
	return m_fd;
}

void Socket::bind(IPAddress& addr)
{
	if (::bind(m_fd, (struct sockaddr*)addr.getAddr(), sizeof(struct sockaddr_in)) == -1)
	{
		perror("Socket Bind Failed.\n");
		exit(1);
	}
}

void Socket::listen(int nums)
{
	if (::listen(m_fd, nums) == -1)
	{
		perror("Socket Listening Failed\n");
		exit(1);
	}
}

int Socket::accept(IPAddress* addr)
{
	socklen_t len = sizeof(struct sockaddr);
	struct sockaddr* _addr = (struct sockaddr*)addr->getAddr();
	int cfd = ::accept(m_fd, _addr, &len);

	return cfd;
}

void Socket::close()
{
	::close(m_fd);
}

void Socket::nonBlocking()
{
	int oldSocketFlag = fcntl(m_fd, F_GETFL, 0);
	int newSocketFlag = oldSocketFlag | O_NONBLOCK;
	fcntl(m_fd, F_SETFL, newSocketFlag);
}


