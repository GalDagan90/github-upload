/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __TUPLE_H_RD5678_ILRD__
#define __TUPLE_H_RD5678_ILRD__

#include <iostream>
#include <type_traits>

#include "typelist.hpp"

namespace ilrd_5678
{

template<typename... Args>
class Tuple;

template<>
class Tuple<>{};

template<typename T0, typename... T1toN>
class Tuple<T0, T1toN...>
{
public:
	Tuple() = default;
	
	template<typename U0, typename... U1toN>
	constexpr explicit Tuple(U0&& head, U1toN&&... tail) : 
		m_head{std::forward<U0>(head)},
		m_tail{std::forward<U1toN>(tail)...}
	{}

	template<typename U0, typename... U1toN>
	Tuple(const Tuple<U0, U1toN...>& other) : 
		m_head{other.GetHead()},
		m_tail{other.GetTail()}
	{}

	T0& GetHead()
	{
		return m_head;
	}

	const T0& GetHead() const
	{
		return m_head;
	}

	Tuple<T1toN...>& GetTail()
	{
		return m_tail;
	}

	const Tuple<T1toN...>& GetTail() const
	{
		return m_tail;
	}

private:
	T0 m_head;
	Tuple<T1toN...> m_tail;
};

/*******************************************************************************
 *								Deduction Guide 
 ******************************************************************************/ 
template<typename T0, typename... T1toN>
Tuple(T0&& head, T1toN&&... tail) -> Tuple<std::unwrap_ref_decay_t<T0>, std::unwrap_ref_decay_t<T1toN>...>;

/*******************************************************************************
 *									MakeTuple 
 ******************************************************************************/ 
template<typename... Args>
auto MakeTuple(Args&&... args)
{
	return Tuple<std::unwrap_ref_decay_t<Args>...>(std::forward<Args>(args)...);
}

/*******************************************************************************
 *								Forward Declarations 
 ******************************************************************************/ 
template <typename Tuple>
constexpr bool IsEmpty(Tuple&&);

template <typename Tuple>
constexpr auto TupleSize(Tuple&&);

template <typename Tuple, typename T>
constexpr auto pushFront(const Tuple& tup, T t);

template <typename Tuple>
constexpr auto popFront(const Tuple& tup);

template <typename Tuple>
constexpr auto popBack(Tuple&& tup);

template <typename Tuple, typename T>
constexpr auto pushBack(Tuple&& tup, T t);

template <typename Tuple>
constexpr auto ReverseTuple(Tuple&& tup);

/*******************************************************************************
 *										Get 
 ******************************************************************************/ 

template<std::size_t Idx>
struct GetImpl
{
	template<typename T>
	constexpr static decltype(auto) Front(T&& tuple)
	{
		return GetImpl<Idx-1>::Front(tuple.GetTail());
	}
};

template<>
struct GetImpl<0>
{
	template<typename T>
	constexpr static decltype(auto) Front(T&& tuple)
	{
		return tuple.GetHead();
	}
};


template<std::size_t Idx, typename Tuple>
constexpr decltype(auto) Get(Tuple&& tuple)
{
	return GetImpl<Idx>::Front(std::forward<Tuple>(tuple));
}

/*******************************************************************************
 *									Tuple Size 
 ******************************************************************************/
template<typename Tuple>
struct Tuple_Size;

template<typename... Types>
struct Tuple_Size<Tuple<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)>
{};

template<typename Tuple>
inline constexpr std::size_t Tuple_Size_v = Tuple_Size<Tuple>::value;

/*******************************************************************************
 *								Tuple Algorithms 
 ******************************************************************************/

////////////////////////////////// Detail Namespace ////////////////////////////

namespace detail
{

template<std::size_t N, typename Seq> 
struct Offset_Sequence;

template<std::size_t N, std::size_t... Ints>
struct Offset_Sequence<N, std::index_sequence<Ints...>>
{
	using type = std::index_sequence<Ints + N...>;
};

template<std::size_t N, typename Seq>
using Offset_Sequence_t = typename Offset_Sequence<N, Seq>::type;

template <typename Tuple, std::size_t... Is>
constexpr auto select(Tuple&& tup, std::index_sequence<Is...>)
{
	return MakeTuple(Get<Is>(std::forward<Tuple>(tup))...);
}
/*
template <typename Tuple, std::size_t... Is>
constexpr auto popFrontImpl(Tuple&& tup, std::index_sequence<Is...>)
{
	return MakeTuple(Get<Is>(std::forward<Tuple>(tup))...);
}


template <typename Tuple, std::size_t... Is>
constexpr auto popBackImpl(Tuple&& tup, std::index_sequence<Is...>)
{
	return MakeTuple(Get<Is>(std::forward<Tuple>(tup))...);
}
*/
template <typename Tuple, std::size_t... Is>
constexpr auto ReverseTupleImpl(Tuple&& tup, std::index_sequence<Is...>)
{
	return MakeTuple(Get<sizeof...(Is) - Is - 1>(std::forward<Tuple>(tup))...);
}


struct Tuple_Cat_Impl
{
	template<typename Tuple1, typename Tuple2>
	static auto apply(Tuple1&& tup1, Tuple2&& tup2)
	{
		return Cat_From_indices(std::forward<Tuple1>(tup1),
								std::forward<Tuple2>(tup2),
								std::make_index_sequence<Tuple_Size_v<std::remove_cvref_t<Tuple1>>>{},
								std::make_index_sequence<Tuple_Size_v<std::remove_cvref_t<Tuple2>>>{});
	}

	template<typename Tuple1, typename Tuple2, std::size_t... Is1, std::size_t... Is2>
	static auto Cat_From_indices(Tuple1&& tup1, Tuple2&& tup2, std::index_sequence<Is1...>, std::index_sequence<Is2...>)
	{
		return Tuple(Get<Is1>(std::forward<Tuple1>(tup1))... , Get<Is2>(std::forward<Tuple2>(tup2))...);
	}
};

}//namespace detail

/////////////////////////////////////////////////////////////////////////////

template <typename Tuple>
constexpr bool IsEmpty(Tuple&&)
{
	return Tuple_Size_v<std::remove_cvref_t<Tuple>> == 0;
}

template <typename Tuple>
constexpr auto TupleSize(Tuple&&)
{
	return Tuple_Size_v<std::remove_cvref_t<Tuple>>;
}

template <typename Tuple, typename T>
constexpr auto pushFront(Tuple&& tup, T t)
{
	return TupleCat(MakeTuple(t), std::forward<Tuple>(tup));
}

template <typename Tuple>
constexpr auto popFront(Tuple&& tup)
{
	constexpr auto Size = Tuple_Size_v<std::remove_cvref_t<Tuple>>;
	using indices = detail::Offset_Sequence_t<1, std::make_index_sequence<Size - 1>>;

	return detail::select(std::forward<Tuple>(tup), indices{});
}

template <typename Tuple>
constexpr auto popBack(Tuple&& tup)
{
	constexpr auto Size = Tuple_Size_v<std::remove_cvref_t<Tuple>>;
	using indices = std::make_index_sequence<Size - 1>;

	return detail::select(std::forward<Tuple>(tup), indices{});
}

template <typename Tuple, typename T>
constexpr auto pushBack(Tuple&& tup, T t)
{
	return TupleCat(std::forward<Tuple>(tup), MakeTuple(t));
}

template <typename Tuple>
constexpr auto ReverseTuple(Tuple&& tup)
{
	constexpr auto Size = Tuple_Size_v<std::remove_cvref_t<Tuple>>;
	using indices = std::make_index_sequence<Size>;

	return detail::ReverseTupleImpl(std::forward<std::remove_const_t<Tuple>>(tup), indices{});
}

template <typename... Tuples>
constexpr auto TupleCat(Tuples&&... tup)
{
	return detail::Tuple_Cat_Impl::apply(std::forward<Tuples>(tup)...);
}

/*******************************************************************************
 *									Print Tuple 
 ******************************************************************************/
namespace detail
{

template<typename Ch, typename Tr, typename Tuple, size_t... Is>
void printTupImpl(std::basic_ostream<Ch, Tr>& os, const Tuple& tup, std::index_sequence<Is...>)
{
	((os << (Is == 0 ? "" : ", ") << Get<Is>(tup)),...);
}


} // namespace Detail


template<typename Ch, typename Tr, typename Tuple>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, const Tuple& tup)
{
	constexpr auto Size = Tuple_Size_v<std::remove_cvref_t<Tuple>>;
	using indices = std::make_index_sequence<Size>;
	detail::printTupImpl(os, tup, indices{});

	return os;
}


} // namespace ilrd_5678


#endif /* __TUPLE_H_RD5678_ILRD__ */
