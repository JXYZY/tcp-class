#pragma once
#include<iostream>
class Task
{
public:
	Task(const std::string& task_name) :m_taskName(task_name){}
	virtual ~Task(){}
	void SetSocketFd(int socket_fd) { m_socketFd = socket_fd; }
	int GetSocketFd() { return m_socketFd; }

public:
	virtual void run() = 0;
private:
	std::string m_taskName;
	int m_socketFd;  //用于通信的文件描述符

};