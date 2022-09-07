#pragma once
#include<string>
#include<thread>
#include<QtCore/QByteArray>
class  XTcp
{
public:
	XTcp();  //accept返回的时候用
	XTcp(unsigned short port);//创建服务端用
	XTcp(char* ip,unsigned short port);//创建客户端的时候用
	virtual ~XTcp();

public:
	bool Connect(const char* ip, unsigned short port);
	int Recv(char* buf, int bufsize);
	int Send(const char* buf, int sendsize);
	void Close();
	void HandlerReceiveDataChar(char* data);
public:
	virtual void HandleReceiveData(const QByteArray& data) {/*do nothing*/}

private:
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();
	void NewConnectionHandler();
	
private:
	static bool first;
public:
	int m_sock = 0; //用于服务端监听的sock
	int m_clientSocket;//用于accept之后的客户端
	int m_clientTempSocketToServerUse;//临时赋值给服务器端用的
	unsigned short m_port = 0;
	//std::string ip;
	char m_ip[16];
	std::thread* m_connectManageThread = nullptr;
	bool m_flag;
};