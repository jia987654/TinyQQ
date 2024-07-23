#include "Select.h"

#include <stdio.h>

using namespace net;

void Select::updataChannel(Channel* ch)
{
	if (ch->isConnect())
	{
		return;
	}

	ch->connect();
	m_chs[ch->getFd()] = ch;
	if (m_nfds <= ch->getFd())
	{
		m_nfds = ch->getFd() + 1;
	}
}

void net::Select::removeChannel(Channel* ch)
{
	m_chs.erase(ch->getFd());

	if (ch->getFd() < m_nfds - 1)
	{
		return;
	}

	// 重新获取最大的文件描述符
	if (m_chs.empty())
	{
		m_nfds = 0;
		return;
	}

	int maxk = 0;

	for (auto k : m_chs)
	{
		if (k.first > maxk)
		{
			maxk = k.first;
		}
	}

	m_nfds = maxk;
}

std::vector<Channel*> net::Select::wait()
{
	m_eventsChannel.clear();

	if (m_nfds == 0)
	{
		return m_eventsChannel;
	}

	// 初始化位图
	FD_ZERO(&m_rset);
	FD_ZERO(&m_wset);
	FD_ZERO(&m_eset);

	// 添加事件
	for (auto ch : m_chs)
	{
		// 添加错误事件
		FD_SET(ch.first, &m_eset);
		// 添加读事件
		if (ch.second->getEvents() & EventRead)
		{
			FD_SET(ch.first, &m_rset);
		}
		// 添加写事件
		if (ch.second->getEvents() & EventWrite)
		{
			FD_SET(ch.first, &m_wset);
		}
	}

	int result = select(m_nfds, &m_rset, &m_wset, &m_eset, NULL);

	int flag = 0;

	if (result > 0)
	{
		// 监听的socket描述符
		int fd = 0;

		for (auto k : m_chs)
		{
			flag = 0;
			
			Channel* ch = k.second;

			ch->clearRevents();

			// 读事件
			if (FD_ISSET(ch->getFd(), &m_rset) && ch->getEvents())
			{
				ch->returnEvents(EventRead);
				flag = 1;
			}

			// 写事件
			if (FD_ISSET(ch->getFd(), &m_wset) && ch->getEvents() & EventWrite)
			{
				ch->returnEvents(EventWrite);
				flag = 1;
			}

			// 错误事件
			if (FD_ISSET(ch->getFd(), &m_eset) && ch->getEvents() & EventError)
			{
				ch->returnEvents(EventError);
				flag = 1;
			}

			// 如果Channel有事件，加入到事件数组
			if(flag) m_eventsChannel.emplace_back(ch);
		}
	}

	return m_eventsChannel;
}

bool Select::findFd(int fd)
{
	// 查询fd是否已加入Select
	if (m_chs.find(fd) != m_chs.end())
	{
		return false;
	}

	return true;
}
