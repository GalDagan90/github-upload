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
#include <array>
#include <chrono>
#include <thread>

#include "heap.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

class HeapTest
{
public:
	HeapTest(int val) : m_id(val), m_timeStamp(std::chrono::system_clock::now())
	{
		//empty Ctor
	}

	bool operator <(const HeapTest& other)
	{
		return (m_timeStamp < other.m_timeStamp);
	}

	void operator()()
	{
		std::cout << m_id << std::endl;
	}

private:
	int m_id;
	std::chrono::time_point<std::chrono::system_clock> m_timeStamp;
};


int main()
{
	Heap<HeapTest> taskHeap;

	std::cout << "Empty? " << taskHeap.Empty() << std::endl;
	std::cout << "Size: " << taskHeap.Size() << std::endl;

	HeapTest task_0(10);
	HeapTest task_1(20);
	HeapTest task_2(100);
	HeapTest task_3(0);

	taskHeap.Push(50);
	taskHeap.Push(task_3);
	taskHeap.Push(task_2);
	taskHeap.Push(task_1);
	taskHeap.Push(task_0);

	for (int i = 0; i < 5; ++i)
	{
		auto task = taskHeap.Peek();
		task();
		taskHeap.Pop();
	}
	
	return 0;
}

/*
Heap<int> heap;

std::cout << "Empty? " << heap.Empty() << std::endl;
std::cout << "Size: " << heap.Size() << std::endl;

std::array<int, 10> intArr = {50, 2, 10, -4, 7, 26, 30, 101, 0, -12};

for (auto it : intArr)
{
	heap.Push(it);
	auto minVal = heap.Peek();
	std::cout << "minVal: " << minVal << std::endl;
}

std::cout << "Empty? " << heap.Empty() << std::endl;
std::cout << "Size: " << heap.Size() << std::endl;

for (auto it : intArr)
{	
	UNUSED(it);
	auto minVal = heap.Peek();
	std::cout << "minVal: " << minVal << std::endl;
	heap.Pop();
} */
