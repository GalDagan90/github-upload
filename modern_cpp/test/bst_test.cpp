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

#include "bst.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

class Test
{
public:
	Test(int val) : m_val{val} {}

	void operator()()
	{
		std::cout << m_val << std::endl;
	}
private:
	int m_val;
};


int main()
{
	BST<int> bst_t;

	std::cout << "Is Empty: " << bst_t.IsEmpty() << std::endl;

	bst_t.Insert(20);
	bst_t.Insert(6);
	bst_t.Insert(30);
	bst_t.Insert(-5);
	bst_t.Insert(15);
	bst_t.Insert(8);
	bst_t.Insert(12);
	bst_t.Insert(22);
	bst_t.Insert(40);
	bst_t.Insert(100);

	std::cout << "Size: " << bst_t.Size() << std::endl;

	int i = bst_t.Find(12);
	std::cout << "Found: " << i << std::endl;

	std::cout << "Size: " << bst_t.Size() << std::endl;

	BST<int> bst_t2(std::move(bst_t));

	std::cout << "Size: " << bst_t2.Size() << std::endl;

	bst_t2.ClearTree();


	return 0;
}