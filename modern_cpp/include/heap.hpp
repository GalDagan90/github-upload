#ifndef __HEAP_H_RD5678_ILRD__
#define __HEAP_H_RD5678_ILRD__

#include <iostream>
#include <vector>
#include <mutex>

namespace ilrd_5678
{

/******************************************************************************
User need to provide T type that has a < operator in order to place the data
properly in the heap
******************************************************************************/

template <typename T>
class Heap
{
public:
	explicit Heap();
	~Heap();

	Heap(const Heap& other) = delete;
	Heap& operator=(const Heap& other) = delete;

	bool Empty() const;
	size_t Size() const;
	
	template <typename U>
	void Push(U&& data);
	T Peek() const;
	void Pop();

private:
	std::vector<T> m_arr;
	mutable std::recursive_mutex m_mutLock;

	void HeapifyUp();
	void HeapifyDown();
};

/******************************************************************************/
/*								Class Heap									  */
/******************************************************************************/

template <typename T>
Heap<T>::Heap() : m_arr(), m_mutLock()
{
	//empty Ctor
}

template <typename T>
Heap<T>::~Heap()
{
	//empty Dtor
}

template <typename T>
bool Heap<T>::Empty() const
{
	std::lock_guard<std::recursive_mutex> lk(m_mutLock);
	return (m_arr.empty());
}

template <typename T>
size_t Heap<T>::Size() const
{
	std::lock_guard<std::recursive_mutex> lk(m_mutLock);
	return (m_arr.size());
}

template <typename T>
T Heap<T>::Peek() const
{
	std::lock_guard<std::recursive_mutex> lk(m_mutLock);
	if (Empty())
	{
		throw "Error! Heap is Empty";
	}

	return m_arr.front();
}

template <typename T>
template <typename U>
void Heap<T>::Push(U&& data)
{
	std::lock_guard<std::recursive_mutex> lk(m_mutLock);
	m_arr.emplace_back(std::forward<U>(data));
	HeapifyUp();
}

template <typename T>
void Heap<T>::Pop()
{
	std::lock_guard<std::recursive_mutex> lk(m_mutLock);
	if (!Empty())
	{
		auto last = m_arr.size() - 1;
		std::swap(m_arr[0], m_arr[last]);
		m_arr.pop_back();
		HeapifyDown();
	}
}

/******************************************************************************/
template <typename T>
void Heap<T>::HeapifyUp()
{
	auto GetParentIndex = [](size_t index){ 
		return (index > 0)? ((index - 1) / 2): 0 ;
	};
	
	auto index = m_arr.size() - 1;
	auto parentIndex = GetParentIndex(index);

	while (m_arr[index] < m_arr[parentIndex])
	{
		std::swap(m_arr[index], m_arr[parentIndex]);

		index = parentIndex;
		parentIndex = GetParentIndex(index);
	}
	
}

template <typename T>
void Heap<T>::HeapifyDown()
{
	auto FindMin = [this](size_t left, size_t right){
		return (this->m_arr[left] < this->m_arr[right])? left : right;
	};

	size_t index = 0;
	bool swapped = true;
	while (swapped && index < (m_arr.size() / 2))
	{
		swapped = false;
		auto left = index * 2 + 1;
		auto right = (left == (m_arr.size() - 1)) ? left : left + 1;
		auto minIndex = FindMin(left, right);

		if (m_arr[minIndex] < m_arr[index])
		{
			std::swap(m_arr[index], m_arr[minIndex]);
			swapped = true;
			index = minIndex;
		}
	}
}

} // namespace ilrd_5678


#endif /* __HEAP_H_RD5678_ILRD__ */
