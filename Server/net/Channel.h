#ifndef _NET_CHANNEL_H
#define _NET_CHANNEL_H

#include "EventLoop.h"
#include "ipaddress.h"
#include "socket.h"

#include <functional>
#include <memory>
#include <sys/epoll.h>

#define IO_BUFFER_SIZE 512

namespace net
{
	class EventLoop;

	enum
	{
		EventRead	= 0x001,
		EventWrite	= 0x002,
		EventHub	= 0x008,
		EventError	= 0x10,
	};

	using Callback = std::function<void()>;
	using EventLoopPtr = std::shared_ptr<EventLoop>;

	class Channel
	{
	public:
		Channel(EventLoopPtr loop, int fd);
		~Channel() {}

		/// <summary>
		/// ��ȡ�ļ�������
		/// </summary>
		/// <returns></returns>
		int getFd();

		/// <summary>
		/// ��ȡ����Ȥ���¼�
		/// </summary>
		/// <returns>�¼�</returns>
		int getEvents();

		/// <summary>
		/// �����¼�
		/// </summary>
		/// /// <param name="events">�¼�</param>
		void returnEvents(int events);

		/// <summary>
		/// �����ص��¼�
		/// </summary>
		void clearRevents();

		/// <summary>
		/// �ж��Ƿ����ӵ�Selector��
		/// </summary>
		/// <returns></returns>
		bool isConnect();

		/// <summary>
		/// ���뵽Selector��
		/// </summary>
		/// <param name="state"></param>
		void connect();

		/// <summary>
		/// ��Selector���Ƴ�
		/// </summary>
		void remove();

		/// <summary>
		/// ���������¼�
		/// </summary>
		void disableAll();

		/// <summary>
		/// �����¼�
		/// </summary>
		/// <param name="events">���õ��¼�</param>
		void setEvents(int events);

		/// <summary>
		/// ���ö��¼�
		/// </summary>
		void setReadEvent();

		/// <summary>
		/// ����д�¼�
		/// </summary>
		void setWriteEvent();

		void setAcceptCallback(Callback callback)
		{
			m_acctptCallback = std::move(callback);
		}

		/// <summary>
		/// ���ö��¼��ص�����
		/// </summary>
		/// <param name="callback"></param>
		void setReadCallback(Callback callback)
		{
			m_readCallback = std::move(callback);
		}

		/// <summary>
		/// ����д�¼��ص�����
		/// </summary>
		/// <param name="callback"></param>
		void setWriteCallback(Callback callback)
		{
			m_writeCallback = std::move(callback);
		}

		/// <summary>
		/// ���öϿ������¼��ص�����
		/// </summary>
		/// <param name="callback"></param>
		void setHubCallback(Callback callback)
		{
			m_hubCallback = std::move(callback);
		}

		/// <summary>
		/// ���ô����¼��ص�����
		/// </summary>
		/// <param name="callback"></param>
		void setErrorCallback(Callback callback)
		{
			m_errorCallback = std::move(callback);
		}

		/// <summary>
		/// ��ȡ���������¼��ص�����
		/// </summary>
		/// <returns></returns>
		Callback getAcceptCallback()
		{
			return m_acctptCallback;
		}

		/// <summary>
		/// ��ȡ��ȡ�¼��ص�����
		/// </summary>
		/// <returns></returns>
		Callback getReadCallback()
		{
			return m_readCallback;
		}

		/// <summary>
		/// ��ȡд���¼��ص�����
		/// </summary>
		/// <returns></returns>
		Callback getWriteCallback()
		{
			return m_writeCallback;
		}

		/// <summary>
		/// ��ȡ�Ͽ������¼��ص�����
		/// </summary>
		/// <returns></returns>
		Callback getHubCallback()
		{
			return m_hubCallback;
		}

		/// <summary>
		/// ��ȡ�����¼��ص�����
		/// </summary>
		/// <returns></returns>
		Callback getErrorCallback()
		{
			return m_errorCallback;
		}

		/// <summary>
		/// �����¼�
		/// </summary>
		void handle();

	private:
		int m_fd;			// ��������socket�ļ�������

		int m_events;		// ע����¼�
		int m_revents;		// ���ص��¼�

		bool m_isConnect;	// �Ƿ����ӵ�Selector

		EventLoopPtr m_loop;// ����ѭ��

	private:
		Callback m_readCallback;	// ��ȡ���ݻص�
		Callback m_writeCallback;	// д�����ݻص�
		Callback m_hubCallback;		// �Ͽ����ӻص�
		Callback m_errorCallback;	// �����¼��ص�
		Callback m_acctptCallback;	// �������ӻص�

		char* m_readBuffer;
		char* m_writeBuffer;

	private:
		/// <summary>
		/// ����Channel��EventLoop
		/// </summary>
		void updata();
	};
}

#endif // !_NET_CHANNEL_H
