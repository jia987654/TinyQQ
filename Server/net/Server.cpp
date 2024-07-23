#include "Server.h"

#include <stdio.h>
#include <string>

using namespace net;

net::Server::Server(IPAddress addr) :
	m_addr(addr)
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

	m_acceptor = new Acceptor(&m_loop, &m_sock, m_addr);
	m_acceptor->setNewConnectionCallback(std::bind(&Server::newConnect, this, std::placeholders::_1, 
		std::placeholders::_2));
	m_acceptor->listen();
}

void net::Server::newConnect(int cfd, IPAddress* addr)
{
	std::string name;

	name = addr->getIp() + ":" + std::to_string(addr->getPort());

	std::shared_ptr<Connection> conn = std::make_shared<Connection>(name, &m_loop, cfd, *addr);
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
