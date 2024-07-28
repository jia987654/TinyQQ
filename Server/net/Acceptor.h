#ifndef _NET_ACCEPTOR_H
#define _NET_ACCEPTOR_H

#include "EventLoop.h"
#include "Channel.h"

#include <functional>
#include <memory>

namespace net
{
	class EventLoop;
	class Channel;
	class Sokcet;
	class IPAddress;

	using NewConnectCallback = std::function<void(int, IPAddress*)>;

	class Acceptor
	{
	public:
		Acceptor(EventLoop* loop, Socket* sock, IPAddress addr);
		~Acceptor() {}

		/// <summary>
		/// ��������ʼ����
		/// </summary>
		void listen();

		/// <summary>
		/// ���ý��������ӵĻص�����
		/// </summary>
		/// <param name="callback"></param>
		void setNewConnectionCallback(NewConnectCallback callback)
		{
			m_newConnectionCallback = std::move(callback);
		}
	private:
		EventLoop* m_loop;								// �������������¼�ѭ��
		
		std::unique_ptr<Channel> m_ch;					// ��������Channel
		std::unique_ptr<Socket> m_sock;					// ����˵�Socket

		IPAddress m_addr;								// ����˵ĵ�ַ

		NewConnectCallback m_newConnectionCallback;	// ���������ӵĻص�����
	private:
		/// <summary>
		/// ���������Channel�Ķ��¼��ص�����
		/// </summary>
		void handleRead();
	};
}

#endif // !_NET_ACCEPTOR_H
