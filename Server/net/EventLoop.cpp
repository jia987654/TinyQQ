#include "EventLoop.h"
#include "Select.h"
#include "Epoll.h"

#include <sys/eventfd.h>

using namespace net;

EventLoop::EventLoop() : 
	// m_wakeFd(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
	// m_wakeChannel(std::make_shared<Channel>(this, m_wakeFd)),
	m_selector(std::make_unique<Select>()) 
	// m_selector(std::make_unique<Poll>())
	// m_selector(std::make_unique<Epoll>())
{
	// 设置线程id
	m_threadId = std::this_thread::get_id();

	// 设置wakeup Channel
	// m_wakeChannel->setReadCallback(std::bind(&EventLoop::wakeupReadHandle, this));
	// m_wakeChannel->setReadEvent();
}

void net::EventLoop::run()
{
	m_quick = false;
	printf("Event Loop Run!\n");
	while (!m_quick)
	{
		// if (m_startCallback) m_startCallback();

		std::vector<Channel*> chs = m_selector->wait();

		if (chs.empty())
		{
			continue;
		}

		for (auto ch : chs)
		{
			ch->handle();
		}
	}
}

void EventLoop::quite()
{
	m_quick = true;

	if (!isInLoop())
	{
		wakeup();
	}
}

void EventLoop::updataChannel(Channel* ch)
{
	m_selector->updataChannel(ch);
}

void EventLoop::removeChannel(Channel* ch)
{
	m_selector->removeChannel(ch);
}

void EventLoop::doInLoop(PendingFunc func)
{
	appendPendingFunc(std::move(func));

	if (!isInLoop())
	{
		// wakeup();
	}
}

bool EventLoop::isInLoop()
{
	return std::this_thread::get_id() == m_threadId;
}

void EventLoop::appendPendingFunc(PendingFunc func)
{
	m_pendingFuncions.append(std::move(func));
}

void EventLoop::wakeup()
{
	/*
	uint64_t one = 1;

	int len = write(m_wakeFd, &one, sizeof(uint64_t));

	if (len != sizeof(uint64_t))
	{
		printf("Wakeup Write Failed.\n");
	}
	*/
}

void EventLoop::wakeupReadHandle()
{
	uint64_t one = 1;

	int len = read(m_wakeFd, &one, sizeof(uint64_t));

	if (len != sizeof(uint64_t))
	{
		printf("EventLoop Wakeup Read Failed.\n");
	}
}

