#include<windows.h>
#include<iostream>
#include<string>
#include <thread>
#include "TcpThread.h"
#include "XTcp.h"


int main(int argc,char * argv[])
{
#if 0
	//加载socket动态库，设置lib,ws2_32.lib
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	//创建socket
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock == -1)
	{
		std::cout << "创建socket失败！！" << std::endl;
	}
	std::cout << "[" << sock << "]" << std::endl;
	
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;//tcp/ip 协议
	saddr.sin_port = htons(8080); // 8080端口 转网络字节序大端
	saddr.sin_addr.s_addr = htonl(0); //ip

	//绑定端口
	if (bind(sock, (sockaddr*)&saddr, sizeof(saddr))!=0)
	{
		std::cout << "bind port " << saddr.sin_port << "failed" << std::endl;
		return -2;
	}
	std::cout << "bind port " << saddr.sin_port << "success!" << std::endl;

	//监听
	listen(sock,2);

	//循环接受连接
	for (;;)
	{
		//accept 读取接受连接信息
		sockaddr_in clientSaddr; // 存储客户端信息
		int len = sizeof(sockaddr_in);
		int clientSock = accept(sock, (sockaddr*)&clientSaddr, &len);
		if (clientSock<=0)
		{
			break;
		}
		std::string ip = inet_ntoa(clientSaddr.sin_addr);
		unsigned port = ntohs(clientSaddr.sin_port);
		std::cout << "client ip[" << ip << "]" << " " << "port[" << port << "]" << std::endl;
		TcpThread* th = new TcpThread(clientSock);
		std::thread sth(&TcpThread::TcpHandlerClient,th); // 用th这个对象的TcpHandlerClient这个函数
		sth.detach();
	}
	closesocket(sock);
	

	//接受数据
	char buf[1024] = { 0 };
	for (;;)
	{
		int recvLen = recv(clientSock, buf, sizeof(buf) - 1, 0); // len:实际收到数据大小
		if (recvLen<=0)
		{
			break;
		}
		buf[recvLen] = '\0';
		std::cout << "buf strlen:" << strlen(buf) << std::endl;
		if (strstr(buf,"quit")!=nullptr)
		{
			char re[] = "quit success!";
			send(clientSock,re,strlen(re)+1,0);
			break;
		}
		//发数据
		int sendLen = send(clientSock,"ok",3,0); // 每次发送3个数据
		std::cout << "recv :" << buf << std::endl;
	}
	
	//关闭套接字
	closesocket(clientSock);
#endif
	XTcp server(8080);

	//循环接受连接
	//for (;;)
	//{
	//	XTcp clientConnect = server.Accept();
	//	TcpThread* th = new TcpThread(clientConnect);
	//	std::thread sth(&TcpThread::TcpHandlerClient, th); // 用th这个对象的TcpHandlerClient这个函数
	//	sth.detach();
	//}
	//server.Close();
	std::cin.get();
	return 0;
}