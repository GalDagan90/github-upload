
#ifndef __TYPELIST_H_RD5678_ILRD__
#define __TYPELIST_H_RD5678_ILRD__

#include <type_traits>
#include "type_traits.hpp"

namespace ilrd_5678
{

///////////////////////// TypeList definition //////////////////////

template <typename... Types>
struct TypeList{};

///////////////////////// Is_Empty //////////////////////

template <typename LIST>
struct Is_Empty : std::false_type{};

template<>
struct Is_Empty<TypeList<>> : std::true_type{};

///////////////////////// Size /////////////////////////////

template <typename LIST>
struct ListSize;

template<>
struct ListSize<TypeList<>>
{
	enum { value = 0 };
};

template<typename T0, typename... T1toN>
struct ListSize<TypeList<T0, T1toN...>>
{
	enum { value = 1 + ListSize<TypeList<T1toN...>>::value };
};

///////////////////////// Is_All_same //////////////////////
template <typename LIST>
struct Is_All_Same;

template <>
struct Is_All_Same<TypeList<>> : std::false_type
{};

template <typename T>
struct Is_All_Same<TypeList<T>> : std::true_type
{};

template <typename T0, typename... T1toN>
struct Is_All_Same<TypeList<T0, T1toN...>>
{
	enum { value = Conjunction_v<IsSame<T0, T1toN>...> };
};

template <typename... Ts>
inline constexpr bool Is_All_Same_v = Is_All_Same<Ts...>::value;

////////////////////  Pop_Front //////////////////////////////////

template <typename LIST>
struct Pop_Front;

template <typename T0, typename... T1toN>
struct Pop_Front<TypeList<T0, T1toN...>>{
	using type = TypeList<T1toN...>;
};

template <typename LIST>
using Pop_Front_t = Pop_Front<LIST>::type;

////////////////////  Push_Front //////////////////////////////////

template <typename T, typename LIST>
struct Push_Front;

template<typename T>
struct Push_Front<T, TypeList<>>
{
	using type = TypeList<T>;
};

template <typename T, typename... Ts>
struct Push_Front<T, TypeList<Ts...>>
{
	using type = TypeList<T, Ts...>;
};

template <typename T, typename LIST>
using Push_Front_t = Push_Front<T, LIST>::type;

////////////////////  Push_Back //////////////////////////////////

template <typename T, typename LIST>
struct Push_Back;

template <typename T>
struct Push_Back<T, TypeList<>>
{
	using type = TypeList<T>;
};

template <typename T, typename T0, typename... T1toN>
struct Push_Back<T, TypeList<T0, T1toN...>>
{
private:
	using TailList = Push_Back<T, TypeList<T1toN...>>::type;
public:
	using type = typename Push_Front<T0, TailList>::type;
};

template <typename T, typename LIST>
using Push_Back_t = Push_Back<T, LIST>::type;

////////////////////  Peek //////////////////////////////////
template <typename LIST>
struct Peek;

template <typename T0, typename... T1toN>
struct Peek<TypeList<T0, T1toN...>>{
	using type = T0;
};

template <typename LIST>
using Peek_t = Peek<LIST>::type;

//////////////////// At  //////////////////////////////////
template <typename LIST, std::size_t IDX>
struct At;

template <typename LIST>
struct At<LIST, 0>{
	using type = Peek_t<LIST>;
};

template <typename LIST, std::size_t IDX>
struct At : At<Pop_Front_t<LIST>, IDX - 1>
{};

template <typename LIST, std::size_t IDX>
using At_t = At<LIST, IDX>::type;

//////////////////// Contains  //////////////////////////////////
template <typename LIST, typename SEARCH>
struct Contains;

template <typename SEARCH>
struct Contains<TypeList<>, SEARCH> : std::false_type{};

template <typename LIST, typename SEARCH>
struct Contains :
			Conditional_t< 	
					IsSame_v<Peek_t<LIST>, SEARCH>,
					std::true_type,
					Contains<Pop_Front_t<LIST>, SEARCH>
				 >{};

template <typename LIST, typename SEARCH>
inline constexpr bool Contains_v = Contains<LIST, SEARCH>::value;

//////////////////// Erase  //////////////////////////////////

template <typename U, typename LIST>
struct Erase;

template <typename U>
struct Erase<U, TypeList<>>
{
	using type = TypeList<>;
};

template <typename U, typename... T1toN>
struct Erase<U, TypeList<U, T1toN...>>
{
	using type = Pop_Front_t<TypeList<U, T1toN...>>;
};


template <typename U, typename T0, typename... T1toN>
struct Erase<U, TypeList<T0, T1toN...>>
{
private:
	using TailList = Erase<U, TypeList<T1toN...>>::type;
public:
	using type = Push_Front_t<T0, TailList>;
};

template <typename U, typename LIST>
using Erase_t = Erase<U, LIST>::type;

//////////////////// Erase_All  //////////////////////////////////

template <typename U, typename LIST>
struct Erase_All;

template <typename U>
struct Erase_All<U, TypeList<>>
{
	using type = TypeList<>;
};

template <typename U, typename... T1toN>
struct Erase_All<U, TypeList<U, T1toN...>>
{
private:
	using TailList = Pop_Front_t<TypeList<U, T1toN...>>;
public:
	using type = Erase_All<U, TailList>::type;
};

template <typename U, typename T0, typename... T1toN>
struct Erase_All<U, TypeList<T0, T1toN...>>
{
private:
	using TailList = Erase_All<U, TypeList<T1toN...>>::type;
public:
	using type = Push_Front_t<T0, TailList>;
};

template <typename U, typename LIST>
using Erase_All_t = Erase_All<U, LIST>::type;

//////////////////// Remove_Dup  //////////////////////////////////
template <typename LIST>
struct Remove_Dup;

template<>
struct Remove_Dup<TypeList<>>
{
	using type = TypeList<>;
};

template <typename T0, typename... T1toN>
struct Remove_Dup<TypeList<T0, T1toN...>>
{
private:
	using NoDupList = Erase_All_t<T0, TypeList<T1toN...>>;
public:
	using type = Push_Front_t<T0, typename Remove_Dup<NoDupList>::type>;
};

template <typename LIST>
using Remove_Dup_t = Remove_Dup<LIST>::type;


} // namespace ilrd_5678




#endif /* __TYPELIST_H_RD5678_ILRD__ */
