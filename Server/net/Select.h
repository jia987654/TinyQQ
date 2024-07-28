#ifndef _NET_SELECT_H
#define _NET_SELECT_H

#include "Selector.h"
#include "Channel.h"

#include <unordered_map>

namespace net
{
	class Selector;
	class Channel;

	class Select :public Selector
	{
	public:
		virtual ~Select() {}
		void updataChannel(Channel* ch);
		void removeChannel(Channel* ch);
		std::vector<Channel*> wait();
	private:
		int m_nfds;		// �����ļ�������+1
		fd_set m_rset;	// ���¼�λͼ
		fd_set m_wset;	// д�¼�λͼ
		fd_set m_eset;	// �����¼�λͼ

		// ������Channel
		std::unordered_map<int, Channel*> m_chs;

		// ���¼���Channel
		std::vector<Channel*> m_eventsChannel;

	private:
		/// <summary>
		/// �ж��ļ��������Ƿ�����chs��
		/// </summary>
		/// <returns></returns>
		bool findFd(int fd);
	};
};

#endif // !_NET_SELECT_H