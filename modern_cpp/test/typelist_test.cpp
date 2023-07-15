#include <iostream>

#include "typelist.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

int main()
{
	TypeList <bool, double, char> t;

	static_assert(ilrd_5678::Is_Empty<TypeList<>>::value == true);
	static_assert(ilrd_5678::Is_Empty<TypeList<int, float>>::value == false);

	static_assert(Is_All_Same<TypeList<int>>::value == true);
	static_assert(Is_All_Same<TypeList<double, double>>::value == true);
	static_assert(Is_All_Same_v<TypeList<bool, bool, bool>> == true);
	static_assert(Is_All_Same<TypeList<bool, bool, double>>::value == false);
	static_assert(Is_All_Same<decltype(t)>::value == false);


	static_assert(ListSize<decltype(t)>::value == 3);
	static_assert(ListSize<TypeList<>>::value == 0);
	static_assert(ListSize<TypeList<int>>::value == 1);


	static_assert(std::is_same_v< Pop_Front<TypeList<int, float>>::type, 
								  TypeList<float> >);
	static_assert(std::is_same_v< Pop_Front_t<TypeList<int, float, bool>>, 
								  TypeList<float, bool> >);
	using t2 = Pop_Front_t<decltype(t)>;
	static_assert(std::is_same_v< Peek_t<t2>, double >);
	using t3 = Pop_Front_t<t2>;
	static_assert(std::is_same_v< Peek_t<t3>, char >);

	static_assert(std::is_same_v<Peek<TypeList<int>>::type,int>);
	static_assert(!std::is_same_v<Peek<TypeList<double, int>>::type, int>);
	static_assert(std::is_same_v<Peek_t<TypeList<bool, double, int>>, bool>);


	using t4 = Push_Front_t<int*, decltype(t)>;
	static_assert(std::is_same_v< Peek_t<t4>, int* >);
	static_assert(std::is_same_v< Push_Front<int, TypeList<>>::type, 
								TypeList<int>>);
	static_assert(std::is_same_v< Push_Front_t<int, TypeList<float>>, 
								TypeList<int, float>>);

	static_assert(std::is_same_v< Push_Back<int, TypeList<>>::type, 
								  TypeList<int> 
								>);
	static_assert(std::is_same_v< Push_Back<int, TypeList<float>>::type, 
								  TypeList<float, int> 
								>);
	static_assert(std::is_same_v< Push_Back<int, TypeList<float, char*>>::type, 
									TypeList<float, char*, int> 
								>);


	static_assert(std::is_same_v<At<TypeList<int>, 0U>::type, int>);
	static_assert(std::is_same_v<
								 At<TypeList<double, int>, 0>::type, 
								 double
				 >);
	static_assert(std::is_same_v<At_t<TypeList<bool, double, int>, 1U>, double>);


	static_assert(Contains<TypeList<float, int>, int>::value == true);
	static_assert(Contains_v<TypeList<float, bool, double>, int> == false);


	static_assert(std::is_same_v< Erase<char, TypeList<char, float, int, bool> >::type,
	 							  TypeList<float, int, bool>
	 							>);
	static_assert(std::is_same_v< Erase_t<int, TypeList<char, float, int, double*>>,
	 							  TypeList<char, float, double*>
	 							>);
	static_assert(std::is_same_v< Erase_All<char, TypeList<char, float, char,int, bool> >::type,
	 							  TypeList<float, int, bool>
	 							>);
	static_assert(std::is_same_v< Erase_All_t<int, TypeList<char, int, float, char,int, bool> >,
	 							  TypeList<char, float, char, bool>
	 							>);


	static_assert(std::is_same_v< Remove_Dup<TypeList<char, int, float, char, int, bool>>::type,
	 							  TypeList<char, int, float, bool>
	 							>);
	static_assert(std::is_same_v< Remove_Dup_t<TypeList<char, int, float, char*, int*, bool>>,
	 							  TypeList<char, int, float, char*, int*, bool>
	 							>);

	return 0;
}



