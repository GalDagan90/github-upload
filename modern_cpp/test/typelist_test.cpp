#include <iostream>
#include <tuple>

#include "type_traits"
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


	static_assert(std::is_same_v< Replace<char, char*, TypeList<char, float, int, bool>>::type,
	 							  TypeList<char*, float, int, bool>
	 							>);
	static_assert(std::is_same_v< Replace_t<char, char*, TypeList<float, char, int, bool>>,
	 							  TypeList<float, char*, int, bool>
	 							>);
	static_assert(std::is_same_v< Replace_t<char, char*, TypeList<float, char*, int, bool>>,
	 							  TypeList<float, char*, int, bool>
	 							>);
	static_assert(std::is_same_v< Replace_All<char, char*, TypeList<char, float, char, int, bool>>::type,
	 							  TypeList<char*, float, char*, int, bool>
	 							>);
	static_assert(std::is_same_v< Replace_All_t<char, char*, TypeList<float, char, char, int, bool>>,
	 							  TypeList<float, char*, char*, int, bool>
	 							>);


	static_assert(std::is_same_v<Reverse<TypeList<char, float, char, int, bool>>::type,
										TypeList<bool, int, char, float, char>
								>);
	static_assert(std::is_same_v<Reverse<TypeList<char>>::type,
										TypeList<char>
								>);
	static_assert(std::is_same_v<Reverse_t<TypeList<char, char*>>,
										TypeList<char*, char>
								>);


	static_assert(std::is_same_v<PeekBack<TypeList<char, float, char, int, bool>>::type,
								bool
								>);
	static_assert(!std::is_same_v<PeekBack<TypeList<char, float>>::type,
								const float
								>);
	static_assert(std::is_same_v<PeekBack_t<TypeList<char, float, int>>,
								int
								>);


	static_assert(std::is_same_v<typename Transform<TypeList<char>, AddConst>::type,
				  TypeList<const char>
				  >);
	static_assert(std::is_same_v<Transform<TypeList<char, int, float>, AddConst>::type,
				  TypeList<const char, const int, const float>
				  >);


	std::tuple<int, float, bool> tup;
	Peek_t<decltype(tup)> first;
	static_assert(std::is_same_v<decltype(first), int>);
	static_assert(Is_Empty<decltype(tup)>::value == false);
	static_assert(ListSize<decltype(tup)>::value == 3);
	static_assert(Is_All_Same_v<decltype(tup)> == false);

	std::tuple<int, int, int> tup2;
	static_assert(Is_All_Same_v<decltype(tup2)> == true);

	Pop_Front_t<decltype(tup)> tup3;
	static_assert(Is_Empty<decltype(tup3)>::value == false);
	static_assert(ListSize<decltype(tup3)>::value == 2);
	static_assert(std::is_same_v<Peek_t<decltype(tup3)>, float>);

	Push_Front_t<short ,decltype(tup)> tup4;
	static_assert(Is_Empty<decltype(tup4)>::value == false);
	static_assert(ListSize<decltype(tup4)>::value == 4);
	static_assert(std::is_same_v<Peek_t<decltype(tup4)>, short>);

	Push_Back_t<short, decltype(tup)> tup5;
	static_assert(Is_Empty<decltype(tup5)>::value == false);
	static_assert(ListSize<decltype(tup5)>::value == 4);
	static_assert(std::is_same_v<PeekBack_t<decltype(tup5)>, short>);
	static_assert(std::is_same_v<At_t<decltype(tup5), 1>, float>);
	static_assert(std::is_same_v<At_t<decltype(tup5), 2>, bool>);
	static_assert(Contains_v<decltype(tup5), char*> == false);

	std::tuple<int, float, bool, char*, int*, const short> tup6;
	Erase_t<int*, decltype(tup6)> tup7;
	static_assert(Is_Empty<decltype(tup7)>::value == false);
	static_assert(ListSize<decltype(tup7)>::value == 5);
	static_assert(std::is_same_v<Peek_t<decltype(tup7)>, int>);
	static_assert(std::is_same_v<PeekBack_t<decltype(tup7)>, const short>);
	static_assert(Contains_v<decltype(tup7), int*> == false);

	std::tuple<int, float, int, char*> tup8;
	
	Erase_All_t<int, decltype(tup8)> tup9;
	static_assert(ListSize<decltype(tup9)>::value == 2);
	static_assert(std::is_same_v<Peek_t<decltype(tup9)>, float>);
	static_assert(std::is_same_v<PeekBack_t<decltype(tup9)>, char*>);
	Remove_Dup_t<decltype(tup8)> tup10;
	static_assert(ListSize<decltype(tup10)>::value == 3);
	static_assert(std::is_same_v<Peek_t<decltype(tup10)>, int>);
	static_assert(std::is_same_v<PeekBack_t<decltype(tup10)>, char*>);

	Replace_t<float, const float, decltype(tup8)> tup11;
	static_assert(ListSize<decltype(tup11)>::value == 4);
	static_assert(std::is_same_v<Peek_t<decltype(tup11)>, int>);
	static_assert(std::is_same_v<At_t<decltype(tup11), 1>, const float>);

	Replace_All_t<int, const int, decltype(tup8)> tup12;
	static_assert(ListSize<decltype(tup12)>::value == 4);
	static_assert(std::is_same_v<At_t<decltype(tup12), 0>, const int>);
	static_assert(std::is_same_v<At_t<decltype(tup12), 2>, const int>);

	Reverse_t<decltype(tup8)> tup13;
	static_assert(ListSize<decltype(tup13)>::value == 4);
	static_assert(std::is_same_v<At_t<decltype(tup13), 0>, char*>);
	static_assert(std::is_same_v<At_t<decltype(tup13), 1>, int>);
	static_assert(std::is_same_v<At_t<decltype(tup13), 2>, float>);
	static_assert(std::is_same_v<At_t<decltype(tup13), 3>, int>);

	Transform_t<decltype(tup8), AddConst> tup14;
	static_assert(ListSize<decltype(tup14)>::value == 4);
	static_assert(std::is_same_v<At_t<decltype(tup14), 0>, const int>);
	static_assert(std::is_same_v<At_t<decltype(tup14), 1>, const float>);
	static_assert(std::is_same_v<At_t<decltype(tup14), 2>, const int>);
	static_assert(std::is_same_v<At_t<decltype(tup14), 3>, char* const>);



	return 0;
}



