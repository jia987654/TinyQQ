#include "Epoll.h"

#include <string.h>

using namespace net;

#define isHasEvent(state) (m_evs[i].events & state)

Epoll::Epoll()
{
	
}

Epoll::~Epoll()
{
	close(m_efd);
}

void Epoll::updataChannel(Channel* ch)
{
	// ������
	if (ch->isConnect())
	{
		// û���¼�������Channel
		if (ch->getEvents() == 0)
		{
			updata(ch, EPOLL_CTL_DEL);
		}
		// ���¼����޸�
		else
		{
			updata(ch, EPOLL_CTL_MOD);
		}
	}
	// û������
	else
	{
		m_chs[ch->getFd()] = ch;
		updata(ch, EPOLL_CTL_ADD);
	}
}

void Epoll::removeChannel(Channel* ch)
{
	m_chs.erase(ch->getFd());
	updata(ch, EPOLL_CTL_DEL);
}

std::vector<Channel*> Epoll::wait()
{
	int res = epoll_wait(m_efd, m_evs, 1024, -1);

	if (res == 0)
	{
		// TODO:��ʱ
	}
	else if (res < 0)
	{
		perror("Epoll Error\n");
	}
	else
	{
		m_eventsChannel.clear();
		for (int i = 0; i < res; i++)
		{
			Channel* ch = (Channel*)m_evs[i].data.ptr;
			ch->clearRevents();

			// ���¼�
			if (isHasEvent(EPOLLIN))
			{
				ch->returnEvents(EventRead);
			}

			// д�¼�
			if (isHasEvent(EPOLLOUT))
			{
				ch->returnEvents(EventWrite);
			}

			// �Ͽ�����
			if (isHasEvent(EPOLLHUP))
			{
				ch->returnEvents(EventHub);
			}

			// �����¼�
			if (isHasEvent(EPOLLERR))
			{
				ch->returnEvents(EventError);
			}

			m_eventsChannel.emplace_back(ch);
		}
	}

	return m_eventsChannel;
}

void Epoll::updata(Channel* ch, int op)
{
	struct epoll_event event;

	if (ch->getEvents() & EventRead)
	{
		event.events |= EPOLLIN;
	}

	if (ch->getEvents() & EventWrite)
	{
		event.events |= EPOLLOUT;
	}

	event.data.ptr = ch;

	epoll_ctl(m_efd, op, ch->getFd(), &event);
}