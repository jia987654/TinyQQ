#include "Channel.h"

#include <string.h>
#include <stdlib.h>

using namespace net;

#define existState(state) (m_revents & state)

net::Channel::Channel(EventLoop* loop, int fd) :
	m_loop(loop),
	m_fd(fd),
	m_events(0),
	m_revents(0)
{

}

int Channel::getFd()
{
	return m_fd;
}

int Channel::getEvents()
{
	return m_events;
}

void Channel::returnEvents(int events)
{
	m_revents |= events;
}

void Channel::clearRevents()
{
	m_revents = 0;
}

bool Channel::isConnect()
{
	return m_isConnect;
}

void Channel::connect()
{
	m_isConnect = true;
}

void Channel::remove()
{
	m_loop->removeChannel(this);
	m_isConnect = false;
}

void Channel::disableAll()
{
	m_events = 0;
	updata();
}

void Channel::setEvents(int events)
{
	m_events |= events;
	updata();
}

void Channel::setReadEvent()
{
	setEvents(EventRead);
}

void Channel::setWriteEvent()
{
	m_events |= EventWrite;
	updata();
}

void Channel::handle()
{
	// 读事件
	if (existState(EventRead))
	{
		if (m_readCallback) m_readCallback();
	}

	// 写事件
	if (existState(EventWrite))
	{
		if (m_writeCallback) m_writeCallback();
	}
	
	// 断开连接
	if (existState(EventHub))
	{
		if (m_hubCallback) m_hubCallback();
		return;
	}
	
	// 错误事件
	if (existState(EventError))
	{
		if (m_errorCallback) m_errorCallback();
	}
}

void Channel::updata()
{
	m_loop->updataChannel(this);
}