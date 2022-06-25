/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#include <iostream>
#include <chrono>
#include <algorithm>

#include "thread_safe_queue.hpp"

using namespace design_pattern;

class Task
{
public:
	Task(int id): m_id{id} {}

	void operator()()
	{
		std::cout << "Task: " << m_id << "\n";
	}

private:
	int m_id;
};

class Enqueue
{
public:
	Enqueue(ThreadSafePQ<int>& pq) : m_pq{pq}
	{}

	void operator()()
	{
		for (int i = 0; i < 6; ++i)
		{
			
			if (i % 3 == 0)
				m_pq.Push(i, Priority::LOW);
			else if (i % 3 == 1)
				m_pq.Push(i, Priority::MEDIUM);
			else
				m_pq.Push(i, Priority::HIGH);

			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	}
private:
	ThreadSafePQ<int>& m_pq;
};

class Dequeue
{
public:
	Dequeue(ThreadSafePQ<int>& pq) : m_pq{pq}
	{}

	void operator()()
	{
		bool running = true;
		while(running)
		{
			auto val = m_pq.Pop();
			std::cout << val << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			if (m_pq.Empty())
				running = false;
		}
	}
private:
	ThreadSafePQ<int>& m_pq;
};

int main()
{
	ThreadSafePQ<int> pq{};
	std::vector<std::thread> push_threads;
	std::vector<std::thread> pop_threads;

	for(int i = 0; i < 5; ++i)
	{
		push_threads.emplace_back(Enqueue(std::ref(pq)));
	}

	pop_threads.emplace_back(Dequeue(std::ref(pq)));


	std::for_each(push_threads.begin(), push_threads.end(), 
		[](std::thread& th){
			th.join();
			});

	std::for_each(pop_threads.begin(), pop_threads.end(), 
		[](std::thread& th){
			th.join();
			});
	// std::cout << "Empty: " << pq.Empty() << "\n";
	// pq.Push(99, Priority::LOW);
	// pq.Push(73, Priority::HIGH);
	// pq.Push(666, Priority::MEDIUM);
	// std::cout << "Empty: " << pq.Empty() << "\n";

	// Task t1 = pq.Pop();
	// t1();
	// Task t2 = pq.Pop();
	// t2();

	return 0;
}