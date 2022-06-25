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

int main()
{
	ThreadSafePQ<Task> pq{};

	std::cout << "Empty: " << pq.Empty() << "\n";
	pq.Push(99, Priority::LOW);
	pq.Push(73, Priority::HIGH);
	pq.Push(666, Priority::MEDIUM);
	std::cout << "Empty: " << pq.Empty() << "\n";

	Task t1 = pq.Pop();
	t1();
	Task t2 = pq.Pop();
	t2();

	return 0;
}