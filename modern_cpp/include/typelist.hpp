
#ifndef __TYPELIST_H_RD5678_ILRD__
#define __TYPELIST_H_RD5678_ILRD__

#include <type_traits>
#include <cstddef>
#include "type_traits.hpp"

namespace ilrd_5678
{

///////////////////////// TypeList definition //////////////////////

template <typename... Types>
struct TypeList{};

///////////////////////// Is_Empty //////////////////////

template <typename LIST>
struct Is_Empty : std::false_type{};

template<template<typename...> class List>
struct Is_Empty<List<>> : std::true_type{};

template <typename LIST>
inline constexpr bool Is_Empty_v = Is_Empty<LIST>::value;
///////////////////////// Size /////////////////////////////

template <typename LIST>
struct ListSize;

template<template<typename...> class List>
struct ListSize<List<>>
{
	enum { value = 0 };
};

template<template<typename...> class List, typename T0, typename... T1toN>
struct ListSize<List<T0, T1toN...>>
{
	enum { value = 1 + ListSize<List<T1toN...>>::value };
};

///////////////////////// Is_All_same //////////////////////
template <typename LIST>
struct Is_All_Same;

template <template<typename...> class List>
struct Is_All_Same<List<>> : std::false_type
{};

template <template<typename...> class List, typename T>
struct Is_All_Same<List<T>> : std::true_type
{};

template <template<typename...> class List, typename T0, typename... T1toN>
struct Is_All_Same<List<T0, T1toN...>>
{
	enum { value = Conjunction_v<IsSame<T0, T1toN>...> };
};

template <typename... Ts>
inline constexpr bool Is_All_Same_v = Is_All_Same<Ts...>::value;

////////////////////  Pop_Front //////////////////////////////////

template <typename LIST>
struct Pop_Front;

template <template<typename...> class List, typename T0, typename... T1toN>
struct Pop_Front<List<T0, T1toN...>>
{
	using type = List<T1toN...>;
};

template <typename LIST>
using Pop_Front_t = Pop_Front<LIST>::type;

////////////////////  Push_Front //////////////////////////////////

template <typename T, typename LIST>
struct Push_Front;

template<template<typename...> class List, typename T>
struct Push_Front<T, List<>>
{
	using type = List<T>;
};

template <template<typename...> class List, typename T, typename... Ts>
struct Push_Front<T, List<Ts...>>
{
	using type = List<T, Ts...>;
};

template <typename T, typename LIST>
using Push_Front_t = Push_Front<T, LIST>::type;

////////////////////  Push_Back //////////////////////////////////

template <typename T, typename LIST>
struct Push_Back;

template <template<typename...> class List, typename T>
struct Push_Back<T, List<>>
{
	using type = List<T>;
};

template <template<typename...> class List, typename T, typename T0, typename... T1toN>
struct Push_Back<T, List<T0, T1toN...>>
{
private:
	using TailList = Push_Back<T, List<T1toN...>>::type;
public:
	using type = typename Push_Front<T0, TailList>::type;
};

template <typename T, typename LIST>
using Push_Back_t = Push_Back<T, LIST>::type;

////////////////////  Peek //////////////////////////////////
template <typename LIST>
struct Peek;

template <template<typename...> class LIST ,typename T0, typename... T1toN>
struct Peek<LIST<T0, T1toN...>>{
	using type = T0;
};

template <typename LIST>
using Peek_t = Peek<LIST>::type;

////////////////////  Peek_Back //////////////////////////////////

template<typename LIST>
struct PeekBack;

template<template<typename...> class List>
struct PeekBack<List<>>
{
	using type = nullptr_t;
};

template<template<typename...> class List, typename T>
struct PeekBack<List<T>>
{
	using type = T;
};

template<typename LIST>
struct PeekBack : PeekBack<Pop_Front_t<LIST>>
{};

template<typename LIST>
using PeekBack_t = PeekBack<LIST>::type;

////////////////////  Pop_Back //////////////////////////////////

template <typename LIST>
struct Pop_Back;

template <template<typename...> class List>
struct Pop_Back<List<>>
{
	using type = List<>;
};

template <template<typename...> class List, typename T0>
struct Pop_Back<List<T0>>
{
	using type = List<>;
};

template <template<typename...> class List, typename... Ts>
struct Pop_Back<List<Ts...>>
{
private:
	using Front_t = Peek_t<List<Ts...>>;
	using TailList = Pop_Back<Pop_Front_t<List<Ts...>>>::type;
public:
	using type = Push_Front_t<Front_t, TailList>;
};

template <typename LIST>
using Pop_Back_t = Pop_Back<LIST>::type;

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

template <template<typename...> class List, typename SEARCH>
struct Contains<List<>, SEARCH> : std::false_type{};

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

template <template<typename...> class List, typename U>
struct Erase<U, List<>>
{
	using type = List<>;
};

template <template<typename...> class List, typename U, typename... T1toN>
struct Erase<U, List<U, T1toN...>>
{
	using type = Pop_Front_t<List<U, T1toN...>>;
};


template <template<typename...> class List, typename U, typename T0, typename... T1toN>
struct Erase<U, List<T0, T1toN...>>
{
private:
	using TailList = Erase<U, List<T1toN...>>::type;
public:
	using type = Push_Front_t<T0, TailList>;
};

template <typename U, typename LIST>
using Erase_t = Erase<U, LIST>::type;

//////////////////// Erase_All  //////////////////////////////////

template <typename U, typename LIST>
struct Erase_All;

template <template<typename...> class List, typename U>
struct Erase_All<U, List<>>
{
	using type = List<>;
};

template <template<typename...> class List, typename U, typename... T1toN>
struct Erase_All<U, List<U, T1toN...>>
{
private:
	using TailList = Pop_Front_t<List<U, T1toN...>>;
public:
	using type = Erase_All<U, TailList>::type;
};

template <template<typename...> class List, typename U, typename T0, typename... T1toN>
struct Erase_All<U, List<T0, T1toN...>>
{
private:
	using TailList = Erase_All<U, List<T1toN...>>::type;
public:
	using type = Push_Front_t<T0, TailList>;
};

template <typename U, typename LIST>
using Erase_All_t = Erase_All<U, LIST>::type;

//////////////////// Remove_Dup  //////////////////////////////////
template <typename LIST>
struct Remove_Dup;

template<template<typename...> class List>
struct Remove_Dup<List<>>
{
	using type = List<>;
};

template <template<typename...> class List, typename T0, typename... T1toN>
struct Remove_Dup<List<T0, T1toN...>>
{
private:
	using NoDupList = Erase_All_t<T0, List<T1toN...>>;
public:
	using type = Push_Front_t<T0, typename Remove_Dup<NoDupList>::type>;
};

template <typename LIST>
using Remove_Dup_t = Remove_Dup<LIST>::type;


//////////////////// Replace  //////////////////////////////////

/* Replaces type U with T*/

template <typename U, typename T, typename LIST>
struct Replace;

template<template<typename...> class List, typename U, typename T>
struct Replace<U, T, List<>>
{
	using type = List<>;
};

template <template<typename...> class List, typename U, typename T, typename... T1toN>
struct Replace<U, T, List<U, T1toN...>>
{
	using type = List<T, T1toN...>;
};

template <template<typename...> class List, typename U, typename T, typename T0, typename... T1toN>
struct Replace<U, T, List<T0, T1toN...>>
{
private:
	using ReplacedList = Replace<U, T, List<T1toN...>>::type;
public:
	using type = Push_Front_t<T0, ReplacedList>;
};

template <typename U, typename T, typename LIST>
using Replace_t = Replace<U, T, LIST>::type;

//////////////////// Replace_All  //////////////////////////////////

/* Replaces type U with T*/

template <typename U, typename T, typename LIST>
struct Replace_All;

template<template<typename...> class List, typename U, typename T>
struct Replace_All<U, T, List<>>
{
	using type = List<>;
};

template <template<typename...> class List, typename U, typename T, typename... T1toN>
struct Replace_All<U, T, List<U, T1toN...>>
{
	using type = Push_Front_t<T, typename Replace_All<U, T, List<T1toN...>>::type >;
};

template <template<typename...> class List, typename U, typename T, typename T0, typename... T1toN>
struct Replace_All<U, T, List<T0, T1toN...>>
{
private:
	using ReplacedList = Replace_All<U, T, List<T1toN...>>::type;
public:
	using type = Push_Front_t<T0, ReplacedList>;
};

template <typename U, typename T, typename LIST>
using Replace_All_t = Replace_All<U, T, LIST>::type;

//////////////////// Reverse  //////////////////////////////////

template<typename LIST>
struct Reverse;

template<template<typename...> class List>
struct Reverse<List<>>
{
	using type = List<>;
};

template<template<typename...> class List, typename T0, typename... T1toN>
struct Reverse<List<T0, T1toN...>>
{
private:
	using ReverseList = Reverse<List<T1toN...>>::type;
public:
	using type = Push_Back_t<T0, ReverseList>;
};

template<typename LIST>
using Reverse_t = Reverse<LIST>::type;

//////////////////// Transform  //////////////////////////////////
template<typename LIST, template<typename T> class MetaFunc, 
		bool Empty = Is_Empty<LIST>::value>
struct Transform;

template<typename LIST, template<typename T> class MetaFunc>
struct Transform<LIST, MetaFunc, true>
{
	using type = LIST;
};

template<typename LIST, template<typename T> class MetaFunc>
struct Transform<LIST, MetaFunc, false>
{
private:
	using TransformedHeadType = typename MetaFunc<Peek_t<LIST>>::type;
	using TransformedTailList = Transform<Pop_Front_t<LIST>, MetaFunc>::type;
public:
	using type = Push_Front_t<TransformedHeadType, TransformedTailList>;
};

template<typename LIST, template<typename T> class MetaFunc>
using Transform_t = Transform<LIST, MetaFunc>::type;


///////////////////////////////////////////////////////////////////


} // namespace ilrd_5678




#endif /* __TYPELIST_H_RD5678_ILRD__ */
