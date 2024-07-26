#include "Buffer.h"
#include <unistd.h>
#include <string.h>

using namespace buffer;

Buffer::Buffer() :
	m_buffer(Header + DefaultSize),
	m_read(Header),
	m_write(Header)
{
	
}

size_t Buffer::prependable()
{
	return m_read;
}

size_t Buffer::readable()
{
	return m_write - m_read;
}

size_t Buffer::writable()
{
	return m_buffer.size() - m_write;
}

char* Buffer::begin()
{
	return &*m_buffer.begin();
}

char* Buffer::peek()
{
	return begin() + m_read;
}

std::string Buffer::read(int len)
{
	// 读取数据大小 小于 可读大小
	if (len <= readable())
	{
		std::string res = std::string(peek(), len);
		reduceReadable(len);
		return res;
	}

	return nullptr;
}

std::string buffer::Buffer::read()
{
	return read(readable());
}

void Buffer::reduceReadable(int len)
{
	if (len < readable())
	{
		m_read += len;
	}
	else if (len == readable())
	{
		clearReadable();
	}
}

void Buffer::clearReadable()
{
	m_read = Header;
	m_write = Header;
}

void Buffer::write(char* data, int len)
{
	// 空间不够 扩充
	if (len > writable())
	{
		makeSpace(len - writable() + 1);
	}

	std::copy(data, data + len, begin() + m_write);
	reduceWritable(len);
}

void Buffer::reduceWritable(int len)
{
	
	if (len < writable())
	{
		m_write += len;
	}
}

int Buffer::readFd(int fd)
{
	// 临时开辟1kB栈内存
	char buffer[1024];

	size_t len = 0;		// 单次读取的字节
	size_t count = 0;	// 总共读取的字节

	do
	{
		len = ::read(fd, buffer, 1024);
		write(buffer, len);
		memset(buffer, 0x00, sizeof(buffer));
		count += len;
	} while (len > 0 && len == 1024);

	return count;
}

void Buffer::makeSpace(int len)
{
	// 移动空间
	moveSpace();

	// 空间不够继续扩充
	if (prependable() + writable() > len)
	{
		m_buffer.resize(m_write + len);
	}
}

void Buffer::moveSpace()
{
	// 空间整齐不需要移动
	if (m_read == Header)
	{
		return;
	}

	int _read = readable(); // 可读区域长度，移动前后不变

	std::copy(begin() + m_read, begin() + m_write, begin() + Header);
	m_read = Header;
	m_write = m_read + _read;
}
