#include"SocketTask.h"
#include <windows.h>
SocketTask::SocketTask(const std::string& task_name, XTcp* server)
	:Task(task_name)
	, m_serveXTcp(server)
{

}

SocketTask::~SocketTask()
{

}

void SocketTask::run()
{
	int socketFd = GetSocketFd();
	//��������
	char buf[1024] = { 0 };
	for (;;)
	{
#if 0
		int recvLen = m_clientXTcp.Recv(buf, sizeof(buf) - 1); // len:ʵ���յ����ݴ�С
		std::cout << "strlen receive buf no \0:" << strlen(buf) << std::endl;
		if (recvLen <= 0)
		{
			break;
		}
		buf[recvLen] = '\0';
		std::cout << "sizeof receive buf:" << sizeof(buf) << std::endl;
		std::cout << "strlen receive buf add \0:" << strlen(buf) << std::endl;
		lck.lock();
		m_serveXTcp->m_clientTempSocketToServerUse = m_clientXTcp.m_sock;
		m_serveXTcp->HandlerReceiveDataChar(buf);
		lck.unlock();
#endif
		int recvLen = recv(socketFd, buf, sizeof(buf) - 1, 0);
		if (recvLen <= 0)
		{
			break;
		}
		buf[recvLen] = '\0';
		m_serveXTcp->m_clientTempSocketToServerUse = socketFd;
		m_serveXTcp->HandlerReceiveDataChar(buf);
	}
	//�ر��׽���
	//closesocket(clientSock);
	closesocket(socketFd);
	delete this;
}