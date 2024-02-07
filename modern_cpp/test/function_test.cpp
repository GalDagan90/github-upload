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

#include "function.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

int multiply(int x, int y)
{
	return x * y;
}

auto lambda_func = [](int x, int y){ return x * y; };

class Functor
{
public:
	int operator()(int x, int y) const
	{
		return x * y;
	}
};

int main()
{
	Function<int(int, int)> f1{multiply};
	std::cout << f1(5,2) << "\n";

	Function<int(int, int)> f2{f1};
	std::cout << f2(5,3) << "\n";

	Functor func{};
	Function<int(int, int)> f3 = func;
	std::cout << f3(5,4) << "\n";

	Function<int(int, int)> f4{lambda_func};
	std::cout << f4(5,5) << "\n";

	return 0;
}



