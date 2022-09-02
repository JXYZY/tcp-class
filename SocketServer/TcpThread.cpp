#include "TcpThread.h"
#include<windows.h>
#include <iostream>
#include<string>

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
	for (;;)
	{
		int recvLen = m_clientXTcp.Recv( buf, sizeof(buf) - 1); // len:实际收到数据大小
		if (recvLen <= 0)
		{
			break;
		}
		buf[recvLen] = '\0';
		//std::cout << "buf strlen:" << strlen(buf) << std::endl;
		//服务端处理收到的数据
		m_serveXTcp->HandleReceiveData(buf);
		//这段退出，到时候改成心跳机制
		/*if (strstr(buf, "quit") != nullptr)
		{
			char re[] = "quit success!";
			m_clientXTcp.Send( re, strlen(re) + 1);
			break;
		}*/
		//发数据
		//int sendLen = clientXTcp.Send("ok", 3); // 每次发送3个数据
		//std::cout << "recv :" << buf << std::endl;
	}
	//关闭套接字
	//closesocket(clientSock);
	m_clientXTcp.Close();
	delete this;
}