#include "TcpThread.h"
#include<windows.h>
#include <iostream>
#include<string>
#include<mutex>
std::mutex mtx;
TcpThread::TcpThread(XTcp clientXTcp, XTcp* server)
	:m_clientXTcp(clientXTcp)
	,m_serveXTcp(server)
{

}

TcpThread::~TcpThread()
{

}

void TcpThread::TcpHandlerClient()
{
	//接受数据
	char buf[1024] = { 0 };
	std::unique_lock<std::mutex> lck(mtx);
	lck.unlock();
	for (;;)
	{
		int recvLen = m_clientXTcp.Recv( buf, sizeof(buf) - 1); // len:实际收到数据大小
		if (recvLen <= 0)
		{
			break;
		}
		buf[recvLen] = '\0';
		lck.lock();
		m_serveXTcp->m_clientTempSocketToServerUse = m_clientXTcp.m_sock;
		m_serveXTcp->HandlerReceiveDataChar(buf);
		lck.unlock();
	}
	//关闭套接字
	//closesocket(clientSock);
	m_clientXTcp.Close();
	delete this;
}