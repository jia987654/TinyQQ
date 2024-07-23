#ifndef _NET_CONNECTION_H
#define _NET_CONNECTION_H

#include "Channel.h"
#include "EventLoop.h"
#include "socket.h"

#include <functional>
#include <memory>
#include <string>

namespace net
{
	class Channel;
	class Socket;
	class EventLoop;
	class Connection;

	using ConnectCallback = std::function<void(Connection* conn)>;
	using ReadCallback = std::function<void(Connection* conn, char* buffer, int len)>;
	using WriteCallback = std::function<void(Connection* conn)>;
	using CloseCallback = std::function<void(Connection* conn)>;

	class Connection
	{
	public:
		Connection(std::string name, EventLoop* loop, int fd, IPAddress addr);
		~Connection() {}

		/// <summary>
		/// ע������
		/// </summary>
		void connect();

		/// <summary>
		/// ��ȡ��������
		/// </summary>
		/// <returns></returns>
		std::string getName();

		/// <summary>
		/// ��ȡ�����������¼�ѭ��
		/// </summary>
		EventLoop* getLoop();

		/// <summary>
		/// ��ȡ���ӵĵ�ַ
		/// </summary>
		/// <returns></returns>
		IPAddress getAddr();

		/// <summary>
		/// ��ȡ���뻺��
		/// </summary>
		/// <returns></returns>
		char* getRecvBuffer()
		{
			return m_readBuffer;
		}

		/// <summary>
		/// ��ȡ�������
		/// </summary>
		/// <returns></returns>
		char* getSendBuffer()
		{
			return m_writeBuffer;
		}

		/// <summary>
		/// �������������ݷ����ͻ���
		/// </summary>
		void send();

		/// <summary>
		/// ���ý������ӵĻص�����
		/// </summary>
		/// <param name="callback"></param>
		void setConnectCallback(ConnectCallback callback);

		/// <summary>
		/// ���ö�ȡ��Ϣ�Ļص�����
		/// </summary>
		/// <param name="callback"></param>
		void setReadCallback(ReadCallback callback);

		/// <summary>
		/// ����д����Ϣʱ�Ļص�����
		/// </summary>
		/// <param name="callback"></param>
		void setWriteCallback(WriteCallback callback);

		/// <summary>
		/// �������ӹرյĻص�����
		/// </summary>
		/// <param name="callback"></param>
		void setCloseCallback(CloseCallback callback);

	private:
		std::string m_name;					// ���ӵ�����

		EventLoop* m_loop;					// �����������¼�ѭ��

		std::unique_ptr<Channel> m_ch;		// ���Ӷ�Ӧ��Channel
		std::unique_ptr<Socket> m_sock;		// ���Ӷ�Ӧ��Socket

		IPAddress m_addr;					// ���ӵĵ�ַ

		char* m_readBuffer;					// ���뻺��
		char* m_writeBuffer;				// �������
	private:
		ConnectCallback m_connectCallback;	// ���������ӵĻص�����
		ReadCallback m_readCallback;		// ����Ϣ�ɶ��Ļص�����
		WriteCallback m_writeCallback;		// ��������Ϣ�Ļص�����
		CloseCallback m_closeCallback;		// ���ӹرյĻص�����
	private:
		/// <summary>
		/// ���¼��ص�����
		/// </summary>
		void readCallback();

		/// <summary>
		/// д�¼��ص�����
		/// </summary>
		// void writeCallback();

		/// <summary>
		/// �Ͽ������¼��ص�����
		/// </summary>
		void hubCallback();
	};
}

#endif