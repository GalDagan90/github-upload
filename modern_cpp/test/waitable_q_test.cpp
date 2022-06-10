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
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

#include "waitable_q.hpp"

using namespace ilrd_5678;


template <typename T>
class Producer
{
public:
	Producer(WaitableQ<T>& wq): m_wq{wq} {}

	void operator()()
	{
		for (size_t i = 0; i < 5; ++i)
		{
			std::cout << "Pushing\n";
			m_wq.Push(i);
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	}
private:
	WaitableQ<T>& m_wq;
};

template <typename T>
class Consumer
{
public:
	Consumer(WaitableQ<T>& wq): m_wq{wq} {}

	void operator()()
	{
		for (size_t i = 0; i < 5; ++i)
		{
			std::cout << "Waiting\n";
			int res = -10;
			m_wq.Pop(res);
			std::cout << "ID: " << std::this_thread::get_id() << " Popped: " << res << std::endl;
		}
	}

private:
	WaitableQ<T>& m_wq;
};


int main()
{
	WaitableQ<int> wq;

	std::vector<std::thread> consVec;
	for (size_t i = 0; i < 3; ++i)
	{
		consVec.push_back(std::thread(Consumer<int>(std::ref(wq))));
	}

	std::thread prodTh(Producer<int>(std::ref(wq)));

	prodTh.join();
	std::for_each(consVec.begin(), consVec.end(), [](std::thread& th){
		th.join();
	});


	return 0;
}

