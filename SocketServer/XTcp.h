#pragma once
#include<string>
class  XTcp
{
public:
	XTcp();
	virtual ~XTcp();

public:
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();
	void Close();
	int Recv(char* buf,int bufsize);
	int Send(const char* buf,int sendsize);
	bool Connect(const char* ip,unsigned short port);
public:
	int sock = 0; //用于服务端监听的sock
	unsigned short port = 0;
	//std::string ip;
	char ip[16];
};