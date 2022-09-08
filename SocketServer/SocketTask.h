#pragma once
#include"Task.h"
#include"XTcp.h"
class SocketTask:public Task
{
public:
	SocketTask(const std::string& task_name,XTcp* server);
	~SocketTask();
	void run()override;

private:
	XTcp* m_serveXTcp;
};