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
		static const size_t Header = 8;				// ǰ8���ֽڴ洢��������С
		static const size_t DefaultSize = 1024;		// ������Ĭ��1kB
	public:
		Buffer();
		~Buffer() {}

		/// <summary>
		/// ��ȡ����Ӳ��ִ�С
		/// </summary>
		/// <returns></returns>
		size_t prependable();

		/// <summary>
		/// ��ȡ�ɶ����ִ�С
		/// </summary>
		/// <returns></returns>
		size_t readable();

		/// <summary>
		/// ��ȡ��д���ִ�С
		/// </summary>
		/// <returns></returns>
		size_t writable();

		/// <summary>
		/// ��ȡ��������ʼλ��
		/// </summary>
		/// <returns></returns>
		char* begin();

		/// <summary>
		/// ��ȡ�ɶ���Ϣ����ʼλ��
		/// </summary>
		/// <returns></returns>
		char* peek();

		/// <summary>
		/// �ӻ�������ȡ����
		/// </summary>
		/// <param name="len">��ȡ�ĳ���</param>
		/// <returns></returns>
		std::string read(int len);

		/// <summary>
		/// �ӻ�������ȡȫ������
		/// </summary>
		/// <returns></returns>
		std::string read();

		/// <summary>
		/// ���ٿɶ����ִ�С
		/// </summary>
		/// <param name="len"></param>
		void reduceReadable(int len);

		/// <summary>
		/// ��տɶ�����
		/// </summary>
		void clearReadable();

		/// <summary>
		/// �򻺳���д������
		/// </summary>
		/// <param name="data">����</param>
		/// <param name="len">���ݳ���</param>
		void write(char* data, int len);

		/// <summary>
		/// ��С��д���ִ�С
		/// </summary>
		/// <param name="len"></param>
		void reduceWritable(int len);

		/// <summary>
		/// ��ȡsocket������ڴ�
		/// </summary>
		/// <param name="fd">socket������</param>
		/// <returns>��ȡ�����ݳ���</returns>
		int readFd(int fd);

	private:
		std::vector<char> m_buffer;					// ������

		size_t m_write;								// ��дλ
		size_t m_read;								// �ɶ�λ
		
		bool m_full;								// �������Ƿ�����
	private: 
		/// <summary>
		/// ���󻺳�ռ�
		/// </summary>
		/// <param name="len">����Ĵ�С</param>
		void makeSpace(int len);

		/// <summary>
		/// �ƶ��ռ�
		/// </summary>
		void moveSpace();
	};

	typedef std::shared_ptr<Buffer> BufferPtr;
}

#endif // !_BUFFER_BUFFER_H
