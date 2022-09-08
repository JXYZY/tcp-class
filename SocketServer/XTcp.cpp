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
	//加载socket动态库，设置lib,ws2_32.lib
	//static bool first = true;
	if (first)
	{
		//只进一次，加载socket dll
		first = false;
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
	}
}

//创建服务端用
XTcp::XTcp(unsigned short port)
	:m_flag(true)
{
	if (first)
	{
		//只进一次，加载socket dll
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
	ThreadPool pool(5);//包含10个线程的线程池
	std::cout << "connect thread id:" << std::this_thread::get_id() << std::endl;
	while(m_flag)
	{
		//XTcp clientConnect = Accept();
		//if (clientConnect.m_sock == 0)
		//{
		//	std::cout << "accept 失败！" << std::endl;
		//	return;
		//}
		//TcpThread* th = new TcpThread(clientConnect,this);
		//std::thread sth(&TcpThread::TcpHandlerClient, th); // 用th这个对象的TcpHandlerClient这个函数
		//sth.detach();
		/*std::thread sth();*/
		int clientSock = Accept();
		if (clientSock <0)
		{
			std::cout << "accept 失败！" << std::endl;
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

//创建客户端的时候用
XTcp::XTcp(char* ip, unsigned short port)
{
	if (first)
	{
		//只进一次，加载socket dll
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
	std::cout << "调用析构函数" << std::endl;
	
}

int XTcp::CreateSocket()
{
	//创建socket
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == -1)
	{
		std::cout << "创建socket失败！！" << std::endl;
	}
	std::cout << "socketfd:[" << m_sock << "]" << std::endl;
	return m_sock;
}

bool XTcp::Bind(unsigned short port)
{
	if (m_sock == 0)
		CreateSocket();
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;//tcp/ip 协议
	saddr.sin_port = htons(port); // 8080端口 转网络字节序大端
	saddr.sin_addr.s_addr = htonl(0); //ip

	//绑定端口
	if (bind(m_sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		std::cout << "bind port " << saddr.sin_port << "failed" << std::endl;
		return false;
	}
	std::cout << "bind port " << saddr.sin_port << "success!" << std::endl;

	//监听
	int ret = listen(m_sock, 5);
	std::cout << "ret:" << ret << std::endl;
	return true;
}
int XTcp::Accept() 
{
	//accept 读取接受连接信息
	sockaddr_in clientSaddr; // 存储客户端信息
	int len = sizeof(sockaddr_in);
	int clientSock = accept(m_sock, (sockaddr*)&clientSaddr, &len);
	return clientSock;
}
#if 0
XTcp XTcp::Accept()
{
	XTcp tcp;
	//accept 读取接受连接信息
	sockaddr_in clientSaddr; // 存储客户端信息
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
	//用户可以根据tcp.sock判断这次的
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
		//这里用accept返回的文件描述符
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
	array.resize(strlen(data)+1);//重置数据大小
	memcpy(array.data(), data, strlen(data)+1);//copy数据
	HandleReceiveData(array);
}

