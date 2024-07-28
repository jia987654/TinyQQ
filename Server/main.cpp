#include <stdio.h>
#include <string.h>
#include "net/Server.h"
#include "buffer/Buffer.h"
#include "threadpool/ThreadPool.h"

net::IPAddress addr("0.0.0.0", 12000);
net::Server server(addr);

void onExit()
{
	server.close();
}

/// <summary>
/// 有新的连接
/// </summary>
void newConnect(net::Connection* conn)
{
	printf("New Client\n");
	printf("Client IP:%s\n", conn->getAddr().getIp().c_str());
	printf("Client Port:%d\n", conn->getAddr().getPort());
}

/// <summary>
/// 收到新的消息
/// </summary>
/// <param name="conn"></param>
/// <param name="buffer"></param>
/// <param name="len"></param>
void onMessage(net::Connection* conn, buffer::BufferPtr buffer, int len)
{
	std::string msg = buffer->read();

	printf("Get Message Form [%s]:%s.\n", conn->getName().c_str(), msg.c_str());

	conn->getSendBuffer()->write((char*)msg.c_str(), len);

	// strcpy(conn->getSendBuffer(), buffer);
	conn->send();
}

/// <summary>
/// 发送消息结束
/// </summary>
void onSend(net::Connection* conn)
{
	printf("Send Message Succeed.\n");
}

void onClose(net::Connection* conn)
{
	printf("Client[%s] Disconnected.\n", conn->getName().c_str());
}

int main()
{
	atexit(onExit);

	server.setConnectCallback(std::bind(&newConnect, std::placeholders::_1));
	server.setReadCallback(std::bind(&onMessage, std::placeholders::_1, std::placeholders::_2,
		std::placeholders::_3));
	server.setWriteCallback(std::bind(&onSend, std::placeholders::_1));
	server.setCloseCallback(std::bind(&onClose, std::placeholders::_1));

	server.start();

	return 0;
}