#include "Connection.h"

#include <stdlib.h>
#include <string.h>

using namespace net;

Connection::Connection(std::string name, EventLoop* loop, int fd, IPAddress addr) :
	m_name(name),
	m_loop(loop),
	m_addr(addr),
	m_sock(std::make_unique<Socket>(fd)),
	m_ch(std::make_unique<Channel>(loop, fd))
{
	// 初始化缓冲区
	m_readBuffer = (char*)malloc(sizeof(char) * IO_BUFFER_SIZE);
	m_writeBuffer = (char*)malloc(sizeof(char) * IO_BUFFER_SIZE);

	// 绑定Channel回调函数
	m_ch->setReadCallback(std::bind(&Connection::readCallback, this));
	m_ch->setHubCallback(std::bind(&Connection::hubCallback, this));
}

void net::Connection::connect()
{
	m_ch->setReadEvent();

	if (m_connectCallback) m_connectCallback(this);
}

std::string Connection::getName()
{
	return m_name;
}

EventLoop* Connection::getLoop()
{
	return m_loop;
}

IPAddress Connection::getAddr()
{
	return m_addr;
}

void Connection::send()
{
	int len = ::send(m_sock->getFd(), m_writeBuffer, sizeof(m_writeBuffer), 0);
	if (len < 0)
	{
		printf("Sned Failed.\n");
	}
	else
	{
		if (m_writeCallback) m_writeCallback(this);
	}
}

void Connection::setConnectCallback(ConnectCallback callback)
{
	m_connectCallback = std::move(callback);
}

void Connection::setReadCallback(ReadCallback callback)
{
	m_readCallback = std::move(callback);
}

void Connection::setWriteCallback(WriteCallback callback)
{
	m_writeCallback = std::move(callback);
}

void Connection::setCloseCallback(CloseCallback callback)
{
	m_closeCallback = std::move(callback);
}

void Connection::readCallback()
{
	memset(m_readBuffer, 0x00, sizeof(char) * IO_BUFFER_SIZE);
	memset(m_writeBuffer, 0x00, sizeof(char) * IO_BUFFER_SIZE);

	int len = recv(m_sock->getFd(), m_readBuffer, sizeof(char) * IO_BUFFER_SIZE, 0);

	// 断开连接
	if (len == 0)
	{
		if (m_ch->getHubCallback()) m_ch->getHubCallback()();
		return;
	}

	// 接收消息失败
	if (len < 0)
	{
		if (m_ch->getErrorCallback()) m_ch->getErrorCallback()();
		return;
	}

	if (m_readCallback) m_readCallback(this, m_readBuffer, len);
}

void net::Connection::hubCallback()
{
	/*
	printf("Client Disconnected\n");
	printf("Client IP:%s\n", m_addr.getIp().c_str());
	*/

	m_ch->disableAll();

	if (m_closeCallback) m_closeCallback(this);
}