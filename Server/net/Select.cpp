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

	// ���»�ȡ�����ļ�������
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

	// ��ʼ��λͼ
	FD_ZERO(&m_rset);
	FD_ZERO(&m_wset);
	FD_ZERO(&m_eset);

	// ����¼�
	for (auto ch : m_chs)
	{
		// ��Ӵ����¼�
		FD_SET(ch.first, &m_eset);
		// ��Ӷ��¼�
		if (ch.second->getEvents() & EventRead)
		{
			FD_SET(ch.first, &m_rset);
		}
		// ���д�¼�
		if (ch.second->getEvents() & EventWrite)
		{
			FD_SET(ch.first, &m_wset);
		}
	}

	int result = select(m_nfds, &m_rset, &m_wset, &m_eset, NULL);

	int flag = 0;

	if (result > 0)
	{
		// ������socket������
		int fd = 0;

		for (auto k : m_chs)
		{
			flag = 0;
			
			Channel* ch = k.second;

			ch->clearRevents();

			// ���¼�
			if (FD_ISSET(ch->getFd(), &m_rset) && ch->getEvents())
			{
				ch->returnEvents(EventRead);
				flag = 1;
			}

			// д�¼�
			if (FD_ISSET(ch->getFd(), &m_wset) && ch->getEvents() & EventWrite)
			{
				ch->returnEvents(EventWrite);
				flag = 1;
			}

			// �����¼�
			if (FD_ISSET(ch->getFd(), &m_eset) && ch->getEvents() & EventError)
			{
				ch->returnEvents(EventError);
				flag = 1;
			}

			// ���Channel���¼������뵽�¼�����
			if(flag) m_eventsChannel.emplace_back(ch);
		}
	}

	return m_eventsChannel;
}

bool Select::findFd(int fd)
{
	// ��ѯfd�Ƿ��Ѽ���Select
	if (m_chs.find(fd) != m_chs.end())
	{
		return false;
	}

	return true;
}
