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
	explicit Tuple(U0&& head, U1toN&&... tail) : 
		m_head{std::forward<U0>(head)},
		m_tail{std::forward<U1toN>(tail)...}
	{}

	template<typename U0, typename... U1toN>
	Tuple(const Tuple<U0, U1toN...>& rhs) : 
		m_head{rhs.GetHead()},
		m_tail{rhs.GetTail()}
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
	return Tuple<Args...>(std::forward<Args>(args)...);
}

/*******************************************************************************
 *										Get 
 ******************************************************************************/ 

template<std::size_t Idx>
struct GetImpl
{
	template<typename T0, typename... T1toN>
	static decltype(auto) Front(const Tuple<T0, T1toN...>& tuple)
	{
		return GetImpl<Idx-1>::Front(tuple.GetTail());
	}
};

template<>
struct GetImpl<0>
{
	template<typename T0, typename... T1toN>
	static decltype(auto) Front(const Tuple<T0, T1toN...>& tuple)
	{
		return tuple.GetHead();
	}
};


template<std::size_t Idx, typename... Types>
decltype(auto) Get(const Tuple<Types...>& tuple)
{
	return GetImpl<Idx>::Front(tuple);
}





//Get<1>(t)




} // namespace ilrd_5678


#endif /* __TUPLE_H_RD5678_ILRD__ */
