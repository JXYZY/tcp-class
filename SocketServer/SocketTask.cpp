#include"SocketTask.h"
#include <windows.h>
#include"ThreadPool.h"
std::mutex mtx;
SocketTask::SocketTask(const std::string& task_name, XTcp* server)
	:Task(task_name)
	, m_serveXTcp(server)
{

}

SocketTask::~SocketTask()
{

}

void SocketTask::run(ThreadPool* threadpool)
{
	int socketFd = GetSocketFd();
	//��������
	char buf[1024] = { 0 };
	for (;;)
	{
		int recvLen = recv(socketFd, buf, sizeof(buf) - 1, 0);
		if (recvLen <= 0 )
		{
			break;
		}
		buf[recvLen] = '\0';
		{
			//��������ֹ�����߳��յ������ͻ��˵�����֮�󣬶Է���������ʱ�׽��ָĶ���Ӱ�쵱ǰ�̵߳�ͨ��
			std::lock_guard<std::mutex> lck(mtx);
			m_serveXTcp->m_clientTempSocketToServerUse = socketFd;
			m_serveXTcp->HandlerReceiveDataChar(buf);
		}
	}
	//�ر��׽���
	closesocket(socketFd);
	delete this;
}