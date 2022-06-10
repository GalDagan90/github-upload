#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


#include "waitable_queue.hpp"

void ProdConsTest();
void Producer();
void Consumer();

framework::WaitableQueue<int, framework::PriorityQueue<int> > shared_q;
bool running = true;

int main(void)
{
	/* std::vector<int> vec;
	
	for(int i = 0; i < 20; ++i)
	{
		vec.push_back(std::rand());
	}

	framework::WaitableQueue<int> w_q;
	std::cout << "Is empty: " << ((w_q.Empty()) ? "Empty" : "Not Empty") << std::endl;

	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		w_q.Push(*it);
	}

	std::cout << "Is empty: " << ((w_q.Empty()) ? "Empty" : "Not Empty") << std::endl;

	while(w_q.Empty() == false)
	{
		int val = 0;
		w_q.Pop(val);
		std::cout << "popped val: " << val << std::endl;
	} */

	ProdConsTest();

	return 0;
}

void ProdConsTest()
{
	boost::thread producer_thrd(Producer);
	boost::thread consumer_thrd[10];

	for (int i = 0; i < 10; ++i)
	{
		consumer_thrd[i] = boost::thread(Consumer);
	}

	producer_thrd.join();
	for (int i = 0; i < 10; ++i)
	{
		consumer_thrd[i].join();
	}
}

void Producer()
{
	int loops = 10;
	while(loops)
	{
		int input_val;
		std::cout << "Enter a number\n";
		std::cin >> input_val;
		shared_q.Push(input_val);
		--loops;
	}

	running = false;
}


void Consumer()
{
	while (running)
	{
		boost::chrono::milliseconds ms = boost::chrono::milliseconds(3000);
		int out_val = 0;
		if (shared_q.Pop(out_val, ms))
		{
			std::cout << "Popped val: " << out_val << std::endl;
		}
		else
		{
			std::cout << "Timed out" << std::endl;
		}
	}
}