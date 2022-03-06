#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "SynQueue.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <list>
#include <iostream>
#include <functional>


class ThreadPool{
public:
	using Task = std::function<void()>;	
	ThreadPool(int thread_num = std::thread::hardware_concurrency(), int max_requests = 10000);
	~ThreadPool();
//	bool append( T* request );
//	static void* worker(void* arg);
	void run();
	void Stop();
	void AddTask(const Task& task);
	void AddTask(Task && task);

private:
	void Start(int numThreads);
	void StopThreads();

	void RunInThread();
	void RunInThread_list();

private:
	int m_thread_num;
	int m_max_requests;
	std::atomic_bool m_running;
	std::once_flag m_flag;
	std::list<std::shared_ptr<std::thread>> m_threads;
	SynQueue<Task> m_queue;
	
};


ThreadPool::ThreadPool(int thread_num, int max_requests) : m_thread_num(thread_num), m_queue(max_requests) {
	Start(thread_num);
}


ThreadPool::~ThreadPool(){
	Stop();
}



void ThreadPool::Stop(){
	std::call_once(m_flag, [this] { StopThreads(); });
}

void ThreadPool::AddTask(const Task& task){
	m_queue.Put(task);
}

void ThreadPool::AddTask(Task && task){
	m_queue.Put(std::forward<Task>(task));
}


void ThreadPool::StopThreads(){
	m_queue.Stop();
	m_running = false;
	for (auto thread : m_threads){
		if (thread)
			thread->join();
	}
	m_threads.clear();
}

void ThreadPool::Start(int numThreads){
	m_running = true;
	std::cout << "初始化线程池..." << std::endl;
	for (int i = 0; i < numThreads; ++i){
		m_threads.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
	}
}

void ThreadPool::RunInThread_list()
{
	std::cout << "取出所有任务并执行..." << std::endl;
	while (m_running)
	{
		std::list<Task> list;
		m_queue.Take(list);
		for (auto &task : list)
		{
			if (!m_running)
			{
				return;
			}

			task();
		}
	}
}

void ThreadPool:: RunInThread()
{
	while (m_running)
	{
		Task task;
		if (!m_running)
			return;

		m_queue.Take(task);

		task();
	}
}
#endif
