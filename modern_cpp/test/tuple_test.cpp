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
#include <tuple>
#include <cassert>
#include <cstring>

#include "tuple.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

// pretty-print a tuple
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch,Tr>& os,
                      const Tuple& t,
                      std::index_sequence<Is...>)
{
    ((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
}
 
template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os,
                 const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

int main()
{
	const Tuple<int, float, std::string> tup1{32, 87.5f, "Gal"};
	assert(Get<0>(tup1) == 32);
	assert(Get<1>(tup1) == 87.5f);
	assert(TupleSize(tup1) == 3);
	assert(IsEmpty(tup1) == false);


	auto tup2 = MakeTuple(27, 94.1f, "Rachel", true);
	assert(Get<0>(tup2) == 27);
	assert(Get<3>(tup2) == true);
	assert(TupleSize(tup2) == 4);
	assert(IsEmpty(tup2) == false);


	assert(Get<1>(Tuple{4.5, 'G', "Hello", 42, true}) == 'G');


	auto tup3 = pushFront(tup2, 'L');
	assert(Get<0>(tup3) == 'L');
	assert(Get<1>(tup3) == 27);

	auto tup4 = popFront(tup2);
	assert(Get<0>(tup4) == 94.1f);
	assert(strcmp(Get<1>(tup4), "Rachel") == 0);

	auto tup5 = popBack(tup2);
	assert(TupleSize(tup5) == 3);
	assert(Get<0>(tup5) == 27);
	assert(strcmp(Get<2>(tup5), "Rachel") == 0);

	auto tup6 = pushBack(tup2, 'A');
	assert(TupleSize(tup6) == 5);
	assert(Get<0>(tup6) == 27);
	assert(Get<4>(tup6) == 'A');

	auto tup7 = ReverseTuple(tup2);
	assert(TupleSize(tup7) == 4);
	assert(Get<0>(tup7) == true);
	assert(Get<3>(tup7) == 27);	

	assert('G' == Get<0>(popFront(Tuple{4.5, 'G', "Hello", 42, true})));
	assert(4.5 == Get<4>(ReverseTuple(Tuple{4.5, 'G', "Hello", 42, true})));

	auto tup1_2 = TupleCat(tup1, tup2);
	assert(TupleSize(tup1_2) == 3 + 4);
	assert(Get<0>(tup1_2) == 32);
	assert(Get<3>(tup1_2) == 27);
	assert(Get<6>(tup1_2) == true);


	std::cout << tup1 << "\n";
	std::cout << Tuple{4.5, 'G', "Hello", 42, true} << "\n";


	return 0;
}



