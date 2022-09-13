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
	//接受数据
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
			//枷锁，防止其他线程收到其他客户端的数据之后，对服务器的临时套接字改动，影响当前线程的通信
			std::lock_guard<std::mutex> lck(mtx);
			m_serveXTcp->m_clientTempSocketToServerUse = socketFd;
			m_serveXTcp->HandlerReceiveDataChar(buf);
		}
	}
	//关闭套接字
	closesocket(socketFd);
	delete this;
}