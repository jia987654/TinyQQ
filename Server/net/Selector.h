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
		/// 更新Channel信息
		/// </summary>
		/// <param name="ch"></param>
		virtual void updataChannel(Channel* ch) = 0;

		/// <summary>
		/// 移除Channel信息
		/// </summary>
		/// <param name="ch"></param>
		virtual void removeChannel(Channel* ch) = 0;

		/// <summary>
		/// 等待事件发送
		/// </summary>
		/// <returns>事件Channel</returns>
		virtual std::vector<Channel*> wait() = 0;
	private:
		std::vector<Channel*> m_chs;	// Channel事件列表
	};
}

#endif // _NET_SELECTOR_H
