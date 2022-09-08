#pragma once
#include"Task.h"
#include"XTcp.h"
#include<QtCore/QTimer>
#include<QtCore/QTime>
#include<QtCore/QObject>
class SocketTask:public Task
{
public:
	SocketTask(const std::string& task_name,XTcp* server);
	~SocketTask();
	void run(ThreadPool* threadpool)override;
private:
	XTcp* m_serveXTcp;
	
};