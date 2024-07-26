#include "Acceptor.h"

using namespace net;

Acceptor::Acceptor(EventLoopPtr loop, Socket* sock, IPAddress addr) :
	m_loop(loop),
	m_sock(sock),
	m_addr(addr),
	m_ch(std::make_unique<Channel>(loop, sock->getFd()))
{
	m_ch->setReadCallback(std::bind(&Acceptor::handleRead, this));
	m_ch->setReadEvent();
}

void net::Acceptor::listen()
{
	m_sock->listen(10);

	m_loop->run();
}

void Acceptor::handleRead()
{
	IPAddress c_addr;	// �ͻ��˵�ַ
	int c_fd;			// �ͻ���socket������

	c_fd = m_sock->accept(&c_addr);

	// �������ӳɹ�
	if (c_fd > 0)
	{
		if (m_newConnectionCallback) m_newConnectionCallback(c_fd, &c_addr);
	}
	else
	{
		// TODO:֪ͨ�ͻ�������ʧ��
	}
}

