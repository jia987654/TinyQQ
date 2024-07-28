#ifndef _BUFFER_BUFFER_H
#define _BUFFER_BUFFER_H

#include <vector>
#include <string>

#include <memory>

namespace buffer
{
	class Buffer
	{
	public:
		static const size_t Header = 8;				// 前8个字节存储缓冲区大小
		static const size_t DefaultSize = 1024;		// 缓冲区默认1kB
	public:
		Buffer();
		~Buffer() {}

		/// <summary>
		/// 获取可添加部分大小
		/// </summary>
		/// <returns></returns>
		size_t prependable();

		/// <summary>
		/// 获取可读部分大小
		/// </summary>
		/// <returns></returns>
		size_t readable();

		/// <summary>
		/// 获取可写部分大小
		/// </summary>
		/// <returns></returns>
		size_t writable();

		/// <summary>
		/// 获取缓冲区起始位置
		/// </summary>
		/// <returns></returns>
		char* begin();

		/// <summary>
		/// 获取可读信息的起始位置
		/// </summary>
		/// <returns></returns>
		char* peek();

		/// <summary>
		/// 从缓冲区读取数据
		/// </summary>
		/// <param name="len">读取的长度</param>
		/// <returns></returns>
		std::string read(int len);

		/// <summary>
		/// 从缓冲区读取全部数据
		/// </summary>
		/// <returns></returns>
		std::string read();

		/// <summary>
		/// 减少可读部分大小
		/// </summary>
		/// <param name="len"></param>
		void reduceReadable(int len);

		/// <summary>
		/// 清空可读部分
		/// </summary>
		void clearReadable();

		/// <summary>
		/// 向缓冲区写入数据
		/// </summary>
		/// <param name="data">数据</param>
		/// <param name="len">数据长度</param>
		void write(char* data, int len);

		/// <summary>
		/// 减小可写部分大小
		/// </summary>
		/// <param name="len"></param>
		void reduceWritable(int len);

		/// <summary>
		/// 读取socket传输的内存
		/// </summary>
		/// <param name="fd">socket描述符</param>
		/// <returns>读取的数据长度</returns>
		int readFd(int fd);

	private:
		std::vector<char> m_buffer;					// 缓冲区

		size_t m_write;								// 可写位
		size_t m_read;								// 可读位
		
		bool m_full;								// 缓冲区是否已满
	private: 
		/// <summary>
		/// 扩大缓冲空间
		/// </summary>
		/// <param name="len">扩大的大小</param>
		void makeSpace(int len);

		/// <summary>
		/// 移动空间
		/// </summary>
		void moveSpace();
	};

	typedef std::shared_ptr<Buffer> BufferPtr;
}

#endif // !_BUFFER_BUFFER_H
