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
#include <vector>
#include "std_function.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

void InitTest(void);
void InvokeTest(void);
void UpTo(int n, FunctionPtr<void(int)> callback);
void Print(int i);

class Functor
{
public:
	void operator()(int i) const
	{
		std::cout << i << "\n";
	}
};

int main()
{
	InitTest();
	InvokeTest();

	return 0;
}

void InitTest(void)
{
	auto f = [](int n){ std::cout << n << std::endl; };

	FunctionPtr<void(int)> f1;
	f1 = f;
	
	FunctionPtr<void(int)> f2(f1);

	auto f3(std::move(f2));

	FunctionPtr<void(int)> f4 = Print;
	f4 = std::move(f3);

	FunctionPtr<void(int)> f5 = Functor();

	std::cout << "Success\n";
}


void InvokeTest(void)
{
	std::vector<int> values;

	UpTo(5, [&values](int i){ values.emplace_back(i); } );

	UpTo(5, Print);

	std::cout << "\n";
}

void UpTo(int n, FunctionPtr<void(int)> callback)
{
	for (int i = 0; i < n; i++)
	{
		callback(i);
	}
}

void Print(int i)
{
	std::cout << i << " ";
}



