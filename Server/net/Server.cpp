#include "Server.h"

#include <stdio.h>
#include <string>

using namespace net;

net::Server::Server(IPAddress addr) :
	m_addr(addr),
	m_loop(std::make_shared<EventLoop>()),
	m_pool(childLoopsNum)
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_sock.setFd(fd);
	m_sock.nonBlocking();
}

void net::Server::start()
{
	m_sock.bind(m_addr);
	printf("Server Bind\n");
	printf("Server IP:%s\n", m_addr.getIp().c_str());
	printf("Server Port:%d\n", m_addr.getPort());

	// 初始化监听器
	m_acceptor = new Acceptor(m_loop, &m_sock, m_addr);
	m_acceptor->setNewConnectionCallback(std::bind(&Server::newConnect, this, std::placeholders::_1, 
		std::placeholders::_2));

	// 初始化各子事件循环
	for (int i = 0; i < childLoopsNum; i++)
	{
		m_childLoops.emplace_back(std::make_shared<EventLoop>());
		m_pool.addTask(std::bind(&EventLoop::run, m_childLoops.at(i).get()));
	}

	m_acceptor->listen();
}

void net::Server::newConnect(int cfd, IPAddress* addr)
{
	std::string name;

	name = addr->getIp() + ":" + std::to_string(addr->getPort());

	std::shared_ptr<Connection> conn = std::make_shared<Connection>(name, polling(), cfd, *addr);
	m_conns[name] = conn;

	// 设置连接的回调函数
	conn->setConnectCallback(m_connectCallback);
	conn->setReadCallback(m_readCallback);
	conn->setWriteCallback(m_writeCallback);
	conn->setCloseCallback(m_closeCallback);

	conn->connect();
}

void Server::removeConnect(Connection* conn)
{
	m_conns.erase(conn->getName());
}

EventLoopPtr net::Server::polling()
{
	m_polling++;
	if (m_polling == childLoopsNum)
	{
		m_polling = 0;
	}
	printf("EventLoop[%d] accept\n", m_polling);
	return m_childLoops[m_polling];
}
