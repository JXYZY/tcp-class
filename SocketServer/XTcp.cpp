//#include"stdafx.h"
#include "XTcp.h"
#include <windows.h>
#include<iostream>
#include"TcpThread.h"
#include"ThreadPool.h"
#include"SocketTask.h"
bool XTcp::first = true;
XTcp::XTcp()
{
	//����socket��̬�⣬����lib,ws2_32.lib
	//static bool first = true;
	if (first)
	{
		//ֻ��һ�Σ�����socket dll
		first = false;
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
	}
}

//�����������
XTcp::XTcp(unsigned short port)
	:m_flag(true)
{
	if (first)
	{
		//ֻ��һ�Σ�����socket dll
		first = false;
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
	}
	Bind(port);
	m_connectManageThread = new std::thread(&XTcp::NewConnectionHandler,this);
	m_connectManageThread->detach();
}

void XTcp::NewConnectionHandler()
{
	ThreadPool pool(5);//����10���̵߳��̳߳�
	std::cout << "connect thread id:" << std::this_thread::get_id() << std::endl;
	while(m_flag)
	{
		//XTcp clientConnect = Accept();
		//if (clientConnect.m_sock == 0)
		//{
		//	std::cout << "accept ʧ�ܣ�" << std::endl;
		//	return;
		//}
		//TcpThread* th = new TcpThread(clientConnect,this);
		//std::thread sth(&TcpThread::TcpHandlerClient, th); // ��th��������TcpHandlerClient�������
		//sth.detach();
		/*std::thread sth();*/
		int clientSock = Accept();
		if (clientSock <0)
		{
			std::cout << "accept ʧ�ܣ�" << std::endl;
			continue;
		}
		else
		{
			SocketTask* task = new SocketTask("SocketCommunicate",this);
			task->SetSocketFd(clientSock);
			pool.AddTask(task);
		}
	}
	
}

//�����ͻ��˵�ʱ����
XTcp::XTcp(char* ip, unsigned short port)
{
	if (first)
	{
		//ֻ��һ�Σ�����socket dll
		first = false;
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
	}
	Connect(ip, port);
}

XTcp::~XTcp()
{
	if (m_connectManageThread)
		delete m_connectManageThread;
	std::cout << "������������" << std::endl;
	
}

int XTcp::CreateSocket()
{
	//����socket
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == -1)
	{
		std::cout << "����socketʧ�ܣ���" << std::endl;
	}
	std::cout << "socketfd:[" << m_sock << "]" << std::endl;
	return m_sock;
}

bool XTcp::Bind(unsigned short port)
{
	if (m_sock == 0)
		CreateSocket();
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;//tcp/ip Э��
	saddr.sin_port = htons(port); // 8080�˿� ת�����ֽ�����
	saddr.sin_addr.s_addr = htonl(0); //ip

	//�󶨶˿�
	if (bind(m_sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		std::cout << "bind port " << saddr.sin_port << "failed" << std::endl;
		return false;
	}
	std::cout << "bind port " << saddr.sin_port << "success!" << std::endl;

	//����
	int ret = listen(m_sock, 5);
	std::cout << "ret:" << ret << std::endl;
	return true;
}
int XTcp::Accept() 
{
	//accept ��ȡ����������Ϣ
	sockaddr_in clientSaddr; // �洢�ͻ�����Ϣ
	int len = sizeof(sockaddr_in);
	int clientSock = accept(m_sock, (sockaddr*)&clientSaddr, &len);
	return clientSock;
}
#if 0
XTcp XTcp::Accept()
{
	XTcp tcp;
	//accept ��ȡ����������Ϣ
	sockaddr_in clientSaddr; // �洢�ͻ�����Ϣ
	int len = sizeof(sockaddr_in);
	std::cout << "sockaddr_in len:" << len << std::endl;
	int clientSock = accept(m_sock, (sockaddr*)&clientSaddr, &len);
	//printf("%s\n", strerror(errno));
	perror("error:");
	std::cout << "accept return:" << clientSock << std::endl;
	if (clientSock <= 0)
	{
		return tcp;
	}
	tcp.m_sock = clientSock;
	//tcp.ip = inet_ntoa(clientSaddr.sin_addr);
	char* ip = inet_ntoa(clientSaddr.sin_addr);
	strcpy(tcp.m_ip,ip);
	tcp.m_port = ntohs(clientSaddr.sin_port);
	std::cout << "client ip[" << tcp.m_ip << "]" << " " << "port[" << tcp.m_port << "]" << std::endl;
	//�û����Ը���tcp.sock�ж���ε�
	return tcp;
}
#endif

void XTcp::Close()
{
	if (m_sock <=0)
	{
		return;
	}
	closesocket(m_sock);
}

int XTcp::Recv(char* buf, int bufsize)
{
	return recv(m_sock,buf,bufsize,0);
}

int XTcp::Send(const char* buf, int sendsize)
{
	std::cout << "Send thread id:" << std::this_thread::get_id() << std::endl;
	int sendedSize = 0;
	for (;;)
	{
		//������accept���ص��ļ�������
		int sendLen = send(m_clientTempSocketToServerUse, buf, sendsize - sendedSize, 0);
		//int sendLen = send(m_sock, buf, sendsize - sendedSize, 0);
		if (sendLen<=0)
			break;
		sendedSize += sendLen;
		if (sendedSize == sendsize)
			break;
	}
	return sendedSize;
}

bool XTcp::Connect(const char* ip, unsigned short port)
{
	if (m_sock<0)
		CreateSocket();
	sockaddr_in cliaddr;
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = inet_addr(ip);
	cliaddr.sin_port = htons(port);

	if (connect(m_sock,(sockaddr*)&cliaddr,sizeof(cliaddr))!=0)
	{
		std::cout << "connect [" << ip << "]" << "[" << port << "] failed!" << std::endl;
		return false;
	}
	std::cout << "connect [" << ip << "]" << "[" << port << "] success!" << std::endl;
	return true;
}

void XTcp::HandlerReceiveDataChar(char* data)
{
	std::cout << "DataChar thread id:" << std::this_thread::get_id() << std::endl;
	QByteArray array;
	array.resize(strlen(data)+1);//�������ݴ�С
	memcpy(array.data(), data, strlen(data)+1);//copy����
	HandleReceiveData(array);
}

