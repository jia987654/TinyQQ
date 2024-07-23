#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "socket.h"
#include "ipaddress.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Connection.h"

#include <unordered_map>
#include <memory>

namespace net
{
	class Socket;
	class IPAddress;
	class EventLoop;
	class Acceptor;
	class Connection;

	class Server
	{
	public:
		Server(IPAddress addr);
		~Server() {}

		/// <summary>
		/// ���������
		/// </summary>
		void start();
	public:
		/// <summary>
		/// ���ý������ӵĻص�����
		/// </summary>
		/// <param name="callback"></param>
		void setConnectCallback(ConnectCallback callback)
		{
			m_connectCallback = std::move(callback);
		}

		/// <summary>
		/// ���ö�ȡ��Ϣ�Ļص�����
		/// </summary>
		/// <param name="callback"></param>
		void setReadCallback(ReadCallback callback)
		{
			m_readCallback = std::move(callback);
		}

		/// <summary>
		/// ����д����Ϣʱ�Ļص�����
		/// </summary>
		/// <param name="callback"></param>
		void setWriteCallback(WriteCallback callback)
		{
			m_writeCallback = std::move(callback);
		}

		/// <summary>
		/// �������ӹرյĻص�����
		/// </summary>
		/// <param name="callback"></param>
		void setCloseCallback(CloseCallback callback)
		{
			m_closeCallback = std::move(callback);
		}

	private:
		Socket m_sock;			// �����socket
		IPAddress m_addr;		// ����˵�ַ

		EventLoop m_loop;		// ���¼�ѭ��
		Acceptor* m_acceptor;	// ������

		// ���е�����
		std::unordered_map<std::string, std::shared_ptr<Connection>> m_conns;
	private:
		/* �û����õĻص����� */
		ConnectCallback m_connectCallback;	// ���������ӵĻص�����
		ReadCallback m_readCallback;		// ����Ϣ�ɶ��Ļص�����
		WriteCallback m_writeCallback;		// ��������Ϣ�Ļص�����
		CloseCallback m_closeCallback;		// ���ӹرյĻص�����
	private:
		/// <summary>
		/// �յ������ӵĻص�����
		/// </summary>
		/// <param name="cfd">�����ӵĿͻ���socket������</param>
		/// <param name="addr">�����ӵĿͻ��˵�ַ</param>
		/// <returns></returns>
		void newConnect(int cfd, IPAddress* addr);

		/// <summary>
		/// �Ƴ�����
		/// </summary>
		/// <param name="conn">����</param>
		void removeConnect(Connection* conn);
	};
}

#endif // !_NET_SERVER_H
