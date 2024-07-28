#include "Connection.h"

#include <stdlib.h>
#include <string.h>

using namespace net;

Connection::Connection(std::string name, EventLoopPtr loop, int fd, IPAddress addr) :
	m_name(name),
	m_loop(loop),
	m_addr(addr),
	m_sock(std::make_unique<Socket>(fd)),
	m_ch(std::make_unique<Channel>(loop.get(), fd)),
	m_readBuffer(std::make_shared<buffer::Buffer>()),
	m_writeBuffer(std::make_shared<buffer::Buffer>())
{
	// 绑定Channel回调函数
	m_ch->setReadCallback(std::bind(&Connection::readCallback, this));
	m_ch->setHubCallback(std::bind(&Connection::hubCallback, this));
}

Connection::~Connection()
{
	m_sock->close();
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
	return m_loop.get();
}

IPAddress Connection::getAddr()
{
	return m_addr;
}

void Connection::send()
{
	std::string str = m_writeBuffer->read();

	int len = ::send(m_sock->getFd(), str.c_str(), str.size(), 0);
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
	int len = m_readBuffer->readFd(m_sock->getFd());

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
	m_loop->removeChannel(m_ch.get());

	if (m_closeCallback) m_closeCallback(this);
}