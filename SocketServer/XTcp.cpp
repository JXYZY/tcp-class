//#include"stdafx.h"
#include "XTcp.h"
#include <windows.h>
#include<iostream>
XTcp::XTcp()
{
	//加载socket动态库，设置lib,ws2_32.lib
	static bool first = true;
	if (first)
	{
		//只进一次，加载socket dll
		first = false;
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
	}
}

XTcp::~XTcp()
{

}

int XTcp::CreateSocket()
{
	//创建socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		std::cout << "创建socket失败！！" << std::endl;
	}
	std::cout << "[" << sock << "]" << std::endl;
	return sock;
}

bool XTcp::Bind(unsigned short port)
{
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;//tcp/ip 协议
	saddr.sin_port = htons(port); // 8080端口 转网络字节序大端
	saddr.sin_addr.s_addr = htonl(0); //ip

									  //绑定端口
	if (bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		std::cout << "bind port " << saddr.sin_port << "failed" << std::endl;
		return false;
	}
	std::cout << "bind port " << saddr.sin_port << "success!" << std::endl;

	//监听
	listen(sock, 2);
	return true;
}

XTcp XTcp::Accept()
{
	XTcp tcp;
	//accept 读取接受连接信息
	sockaddr_in clientSaddr; // 存储客户端信息
	int len = sizeof(sockaddr_in);
	int clientSock = accept(sock, (sockaddr*)&clientSaddr, &len);
	if (clientSock <= 0)
	{
		return tcp;
	}
	tcp.sock = clientSock;
	//tcp.ip = inet_ntoa(clientSaddr.sin_addr);
	char* ip = inet_ntoa(clientSaddr.sin_addr);
	strcpy(tcp.ip,ip);
	tcp.port = ntohs(clientSaddr.sin_port);
	std::cout << "client ip[" << tcp.ip << "]" << " " << "port[" << tcp.port << "]" << std::endl;
	//用户可以根据tcp.sock判断这次的
	return tcp;
}

void XTcp::Close()
{
	if (sock<=0)
	{
		return;
	}
	closesocket(sock);
}

int XTcp::Recv(char* buf, int bufsize)
{
	return recv(sock,buf,bufsize,0);
}

int XTcp::Send(const char* buf, int sendsize)
{
	int sendedSize = 0;
	for (;;)
	{
		int sendLen = send(sock, buf, sendsize - sendedSize, 0);
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
	if (sock<0)
		CreateSocket();
	sockaddr_in cliaddr;
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = inet_addr(ip);
	cliaddr.sin_port = htons(port);

	if (connect(sock,(sockaddr*)&cliaddr,sizeof(cliaddr))!=0)
	{
		std::cout << "connect [" << ip << "]" << "[" << port << "] failed!" << std::endl;
		return false;
	}
	std::cout << "connect [" << ip << "]" << "[" << port << "] success!" << std::endl;
	return true;
}