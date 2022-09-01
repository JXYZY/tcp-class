#include<windows.h>
#include<iostream>
#include<string>
#include <thread>
#include "TcpThread.h"
#include "XTcp.h"


int main(int argc,char * argv[])
{
#if 0
	//����socket��̬�⣬����lib,ws2_32.lib
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	//����socket
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock == -1)
	{
		std::cout << "����socketʧ�ܣ���" << std::endl;
	}
	std::cout << "[" << sock << "]" << std::endl;
	
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;//tcp/ip Э��
	saddr.sin_port = htons(8080); // 8080�˿� ת�����ֽ�����
	saddr.sin_addr.s_addr = htonl(0); //ip

	//�󶨶˿�
	if (bind(sock, (sockaddr*)&saddr, sizeof(saddr))!=0)
	{
		std::cout << "bind port " << saddr.sin_port << "failed" << std::endl;
		return -2;
	}
	std::cout << "bind port " << saddr.sin_port << "success!" << std::endl;

	//����
	listen(sock,2);

	//ѭ����������
	for (;;)
	{
		//accept ��ȡ����������Ϣ
		sockaddr_in clientSaddr; // �洢�ͻ�����Ϣ
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
		std::thread sth(&TcpThread::TcpHandlerClient,th); // ��th��������TcpHandlerClient�������
		sth.detach();
	}
	closesocket(sock);
	

	//��������
	char buf[1024] = { 0 };
	for (;;)
	{
		int recvLen = recv(clientSock, buf, sizeof(buf) - 1, 0); // len:ʵ���յ����ݴ�С
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
		//������
		int sendLen = send(clientSock,"ok",3,0); // ÿ�η���3������
		std::cout << "recv :" << buf << std::endl;
	}
	
	//�ر��׽���
	closesocket(clientSock);
#endif
	XTcp server(8080);

	//ѭ����������
	//for (;;)
	//{
	//	XTcp clientConnect = server.Accept();
	//	TcpThread* th = new TcpThread(clientConnect);
	//	std::thread sth(&TcpThread::TcpHandlerClient, th); // ��th��������TcpHandlerClient�������
	//	sth.detach();
	//}
	//server.Close();
	std::cin.get();
	return 0;
}