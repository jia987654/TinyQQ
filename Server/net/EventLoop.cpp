#include "EventLoop.h"

#include "Select.h"
#include "Epoll.h"

using namespace net;

EventLoop::EventLoop() : 
	m_selector(std::make_unique<Select>()) 
	// m_selector(std::make_unique<Poll>())
	// m_selector(std::make_unique<Epoll>())
{

}

void net::EventLoop::run()
{
	m_quick = false;

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
}

void EventLoop::updataChannel(Channel* ch)
{
	m_selector->updataChannel(ch);
}

void EventLoop::removeChannel(Channel* ch)
{
	m_selector->removeChannel(ch);
}

