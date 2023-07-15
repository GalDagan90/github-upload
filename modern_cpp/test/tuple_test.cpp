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

#include "tuple.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

int main()
{
	const Tuple<int, float, std::string> tp{5, 2.3f, "Gal"};
	auto v = tp.GetHead();
	std::cout << v << "\n";

	auto tp2 = MakeTuple("Rachel", 1996, 5.5f, 4.2);
	auto sub_tp2 = tp2.GetTail();
	auto head_sub_tp2 = sub_tp2.GetHead();
	std::cout << head_sub_tp2 << "\n";

	std::cout << std::boolalpha;
	std::cout << (Get<1>(tp) == 2.3f) << "\n";
	auto s = Get<0>(tp2);
	std::cout << s << "\n";

	auto temp = Get<3>(Tuple{0, "Gal", 32.7f, "Potato", 27.2f});
	std::cout << temp << "\n";


	return 0;
}



