#pragma once
#include<iostream>
//#include"ThreadPool.h"
class ThreadPool;
class Task
{
public:
	Task(const std::string& task_name) :m_taskName(task_name){}
	virtual ~Task(){}
	void SetSocketFd(int socket_fd) { m_socketFd = socket_fd; }
	int GetSocketFd() { return m_socketFd; }

protected:
	int m_socketFd;
public:
	virtual void run(ThreadPool* threadpool) =0;
private:
	std::string m_taskName;
	  //用于通信的文件描述符

};