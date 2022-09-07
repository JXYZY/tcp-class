#pragma once
#include<string>
#include<thread>
#include<QtCore/QByteArray>
class  XTcp
{
public:
	XTcp();  //accept���ص�ʱ����
	XTcp(unsigned short port);//�����������
	XTcp(char* ip,unsigned short port);//�����ͻ��˵�ʱ����
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
	int m_sock = 0; //���ڷ���˼�����sock
	int m_clientSocket;//����accept֮��Ŀͻ���
	int m_clientTempSocketToServerUse;//��ʱ��ֵ�����������õ�
	unsigned short m_port = 0;
	//std::string ip;
	char m_ip[16];
	std::thread* m_connectManageThread = nullptr;
	bool m_flag;
};