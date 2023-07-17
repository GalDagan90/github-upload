/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#include <typeinfo>
#include <string>
#include "type_traits.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

struct Base
{};

struct Derived : public Base
{};

struct S{
	S() = delete;
};

template<typename T, typename... Ts>
constexpr bool all_types_are_same = Conjunction_v<IsSame<T, Ts>...>;

int main()
{
	static_assert(IsPointer<int*>::value == true);
	static_assert(IsPointer<int>::value == false);
	static_assert(IsPointer<const int*>::value == true);

	static_assert(std::is_same_v<RemoveConst_t<int>, int> == true);
	static_assert(std::is_same_v<RemoveConst_t<const int>, int> == true);
	static_assert(std::is_same_v<RemoveConst_t<const int&>, const int&> == true);

	static_assert(std::is_same_v<RemoveReference_t<const int&>, const int> == true);
	static_assert(std::is_same_v<RemoveConst_t<RemoveReference_t<const int&>>, int> == true);

	static_assert(IsSame_v<RemoveReference_t<const int&>, const int> == true);
	static_assert(IsSame_v<RemoveConst_t<RemoveReference_t<const int&>>, int> == true);
	static_assert(IsSame_v<RemoveReference_t<const int&>, int> == false);
	static_assert(IsSame_v<Base, Derived> == false);

	static_assert(IsReference_v<int&> == true);
	static_assert(IsReference_v<RemoveReference_t<int&>> == false);

	static_assert(IsConst_v<int&> == false);
	static_assert(IsConst_v<const float> == true);
	static_assert(IsConst_v<RemoveConst_t<const float>> == false);

	static_assert(IsSame_v<Conditional_t<bool(10 > 6), int, float>, int>);
	static_assert(IsSame_v<Conditional_t<bool(10 > 60), int, float>, int> == false);

	static_assert(IsConvertible_v<Derived, Base> == true);
	static_assert(IsConvertible_v<char, char*> == false);
	static_assert(IsConvertible<int, std::string>::value == false);
	static_assert(IsConvertible_v<char const*, std::string> == true);

	static_assert(all_types_are_same<int, int>);
	static_assert(!all_types_are_same<int, int&, int>);
	static_assert(all_types_are_same<int, 
									RemoveReference_t<int&>, 
									RemoveConst_t<const int>, 
									int>);

	static_assert(IsNullPtr<decltype(nullptr)>::value == true);
	static_assert(IsNullPtr<int*>::value == false);

	static_assert(IsDefaultConstructible<S>::value == false);
	static_assert(IsDefaultConstructible_v<Base> == true);



	return 0;
}



