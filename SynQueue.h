#ifndef SYNQUEUE_H
#define SYNQUEUE_H

#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

template<typename T>
class SynQueue{
public:
    SynQueue(int maxsize) : m_maxSize(maxsize), m_needStop(false) { }
    
	// add task
    void Put(const T&x){Add(x);}
    void Put(T &&x){ Add(x);}

	void Take(std::list< T > &list);
	void Take(T &t);

	void Stop();
	bool Empty();
	bool Full();
	size_t Size();

private:
	template<typename F>
	void Add(F &&x);
	bool IsFull() const;
	bool IsEmpty() const;

private:
    std::mutex m_mutex;                    //互斥锁
    std::list<T> m_queue;                  //队列，存放任务
    std::condition_variable m_notEmpty;    //队列不为空的条件变量
    std::condition_variable m_notFull;    //队列不为满的条件变量
    size_t m_maxSize;                                   //任务队列最大长度 
    bool m_needStop;                                    //终止标识
};

// take all the takes
template<typename T>
void SynQueue< T >::Take(std::list<T> &list){
	//有wait方法必须用unique_lock
	//unique_lock有定时等待等功能，lock_guard就仅仅是RAII手法的互斥锁
	//但unique_lock的性能稍低于lock_guard
	std::unique_lock<std::mutex> locker(m_mutex);
	//满足条件则唤醒，不满足阻塞
	m_notEmpty.wait(locker, [this] { return m_needStop || IsEmpty(); });

	if (m_needStop){
		return;
	}

	std::cout << "取出所有任务..." << std::endl;
	list = std::move(m_queue);
	//唤醒其他阻塞在互斥锁的线程
	m_notFull.notify_one();
}

// take one task
template<typename T>
void SynQueue< T >::Take(T &t){
	std::unique_lock<std::mutex> locker(m_mutex);
	m_notEmpty.wait(locker, [this]    { return m_needStop || IsEmpty(); });
	if (m_needStop){
		return;
	}
	
	std::cout << "取出一个任务..." << std::endl;
	t = m_queue.front();
	m_queue.pop_front();
	m_notFull.notify_one();
}

// stop all the thread's action in queue
template<typename T>
void SynQueue< T >::Stop(){
	do{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_needStop = true;
	}while(0);
	std::cout << "停止任务执行..." << std::endl;
	m_notFull.notify_all();
	m_notEmpty.notify_all();
}


// test the queue is empty or not
template<typename T>
bool SynQueue<T>::Empty(){
	std::lock_guard<std::mutex> locker(m_mutex);
	return m_queue.empty();
}

 
// test the queue is full or not
template<typename T>
bool SynQueue<T>::Full(){
	std::lock_guard<std::mutex> locker(m_mutex);
	return m_queue.size() == m_maxSize;
}

// size of queue
template<typename T>
size_t SynQueue<T>::Size(){
	std::lock_guard<std::mutex> locker(m_mutex);
	return m_queue.size();
}

// add a task int queue
template<typename T>
template<typename F>
void SynQueue<T>::Add(F &&x){
	std::unique_lock<std::mutex> locker(m_mutex);
	m_notFull.wait(locker, [this] { return m_needStop || IsFull(); });
	if (m_needStop){
		return;
	}
	std::cout << "添加一个任务..." << std::endl;
	m_queue.push_back(std::forward<F>(x));
	m_notEmpty.notify_one();
}

// test the queue is full or not
template<typename T>
bool SynQueue<T>::IsFull() const{
	bool full = m_queue.size() >= m_maxSize;
	if (full){
		std::cout << "任务池满，等待取出任务..." << "线程ID:" << std::this_thread::get_id() << std::endl;
	}
	return !full;
}

// test the queue is empty or not
template<typename T>
bool SynQueue<T>::IsEmpty() const{
	bool empty = m_queue.empty();
	if (empty){
		std::cout << "任务池为空，等待添加任务..." << "线程ID:" << std::this_thread::get_id() << std::endl;
	}
	return !empty;
}

#endif
