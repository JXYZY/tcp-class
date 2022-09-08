#include"ThreadPool.h"
#include "Task.h"
ThreadPool::ThreadPool(int num_thread)
	:m_numThread(num_thread)
	,m_isRun(true)
{
	CreateThread();
}

ThreadPool::~ThreadPool()
{
	StopAllThread();
}

void ThreadPool::CreateThread()
{
	for (int index = 0; index < m_numThread; ++index)
	{
		std::thread* t = new std::thread(&ThreadPool::ThreadFun,this);
		m_threads.push_back(t);
	}
}

void ThreadPool::AddTask(Task* task)
{
	std::unique_lock<std::mutex> lck(m_threadPoolMutex);
	m_tasks.push_back(task);
	lck.unlock();
	m_conditionVar.notify_one(); // 有任务的时候尝试唤醒一个线程
}

void ThreadPool::ThreadFun()
{
	while (1)
	{
		std::unique_lock<std::mutex> lck(m_threadPoolMutex);
		if (m_tasks.empty()&& m_isRun)
		{
			m_conditionVar.wait(lck);
		}
		if (m_isRun == false)
		{
			lck.unlock();
			break;
		}
		Task* task = m_tasks.front();
		m_tasks.pop_front();
		lck.unlock();
		task->run(this);
	}
}

void ThreadPool::StopAllThread()
{
	std::unique_lock<std::mutex> lck(m_threadPoolMutex);
	m_isRun = false;
	lck.unlock();

	m_conditionVar.notify_all();

	for (auto& th:m_threads)
	{
		th->join();
	}
	for (auto& th : m_threads)
	{
		delete th;
		th = nullptr;
	}
}