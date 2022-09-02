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
	//��������
	char buf[1024] = { 0 };
	for (;;)
	{
		int recvLen = m_clientXTcp.Recv( buf, sizeof(buf) - 1); // len:ʵ���յ����ݴ�С
		if (recvLen <= 0)
		{
			break;
		}
		buf[recvLen] = '\0';
		//std::cout << "buf strlen:" << strlen(buf) << std::endl;
		//����˴����յ�������
		m_serveXTcp->HandleReceiveData(buf);
		//����˳�����ʱ��ĳ���������
		/*if (strstr(buf, "quit") != nullptr)
		{
			char re[] = "quit success!";
			m_clientXTcp.Send( re, strlen(re) + 1);
			break;
		}*/
		//������
		//int sendLen = clientXTcp.Send("ok", 3); // ÿ�η���3������
		//std::cout << "recv :" << buf << std::endl;
	}
	//�ر��׽���
	//closesocket(clientSock);
	m_clientXTcp.Close();
	delete this;
}