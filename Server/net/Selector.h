#ifndef _NET_SELECTOR_H
#define _NET_SELECTOR_H

#include "Channel.h"

#include <vector>

namespace net
{
	class Channel;

	class Selector
	{
	public:
		// virtual Selector() = 0;
		virtual ~Selector() {};
		/// <summary>
		/// ����Channel��Ϣ
		/// </summary>
		/// <param name="ch"></param>
		virtual void updataChannel(Channel* ch) = 0;

		/// <summary>
		/// �Ƴ�Channel��Ϣ
		/// </summary>
		/// <param name="ch"></param>
		virtual void removeChannel(Channel* ch) = 0;

		/// <summary>
		/// �ȴ��¼�����
		/// </summary>
		/// <returns>�¼�Channel</returns>
		virtual std::vector<Channel*> wait() = 0;
	private:
		std::vector<Channel*> m_chs;	// Channel�¼��б�
	};
}

#endif // _NET_SELECTOR_H
