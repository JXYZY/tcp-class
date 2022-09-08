#pragma once
#include<vector>
#include<queue>
#include<thread>
#include<mutex>
#include"SocketTask.h"
class Task;
class ThreadPool
{
public:
	explicit ThreadPool(int num_thread);
	~ThreadPool();

	void AddTask(Task* task); 
	void StopAllThread();

protected:
	void ThreadFun();
private:
	void CreateThread();

private:
	std::vector<std::thread*> m_threads;
	std::deque<Task*> m_tasks;
	int m_numThread;
	std::mutex m_threadPoolMutex;
	std::condition_variable m_conditionVar;
	bool m_isRun;

	friend void SocketTask::run(ThreadPool* threadpool);
};