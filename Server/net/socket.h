#ifndef _NET_SOCKET_H
#define _NET_SOCKET_H

#include "ipaddress.h"

namespace net
{
	class IPAddress;

	class Socket
	{
	public:
		Socket(int fd) : 
			m_fd(fd) {}
		Socket() {}

		/// <summary>
		/// �����ļ�������
		/// </summary>
		/// <param name="fd"></param>
		void setFd(int fd);

		/// <summary>
		/// ��ȡ�ļ�������
		/// </summary>
		/// <param name="fd"></param>
		int getFd();

		/// <summary>
		/// ��socket
		/// </summary>
		/// <param name="addr"></param>
		void bind(IPAddress &addr);

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="nums"></param>
		void listen(int nums);

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="addr">�ͻ��˵�ַ</param>
		/// <returns>�ͻ����ļ�������</returns>
		int accept(IPAddress* addr);

		/// <summary>
		/// ����Ϊ�Ƕ���
		/// </summary>
		void nonBlocking();

	private:
		int m_fd;
	};
}

#endif