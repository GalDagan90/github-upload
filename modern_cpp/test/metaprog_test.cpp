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

#include "metaprog.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

int main()
{
	static_assert(Factorial<2>::value == 2, " ");
	static_assert(Factorial<4>::value == 24, " ");
	static_assert(Factorial<7>::value == 5040, " ");

	/**********************************************/

	constexpr Fibonacci<1> fib;
	constexpr auto res = fib.Get();
	static_assert(res == 1, " ");
	
	static_assert(Fibonacci<3>().Get() == 2, " ");
	static_assert(Fibonacci<8>().Get() == 21, " ");
	static_assert(Fibonacci<10>().Get() == 55, " ");

	/**********************************************/

	static_assert(Sum(1.0f, 7, 40l) == 48, " ");
	static_assert(Sum(8.0f, 40l) == 48, " ");
	static_assert(Sum(3) == 3, " ");

	static_assert(Diff(10, 7, 3l) == 0, " ");
	static_assert(Diff(8.0f, 40l) == -32, " ");
	static_assert(Diff(3) == 3, " ");
	static_assert(Diff() == 0, " ");

	/**********************************************/

	static_assert(IsPrime<2>::value);
	static_assert(IsPrime<7>::value);
	static_assert(IsPrime<17>::value);
	static_assert(IsPrime<25>::value == false);


	return 0;
}



