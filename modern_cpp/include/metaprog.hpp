#ifndef __METAPROG_H_RD5678_ILRD__
#define __METAPROG_H_RD5678_ILRD__

#include <utility>
#include <type_traits>

namespace ilrd_5678
{

/****************************************************************************
*																			*
*****************************************************************************/
template <int N>
struct Factorial
{
	enum {value = N * Factorial<N - 1>::value};
};

template <>
struct Factorial<0>
{
	enum {value = 1};
};

/****************************************************************************
*																			*
*****************************************************************************/
template <int N>
class Fibonacci
{
public:
	constexpr Fibonacci() 
	{
		for (auto i = 0; i < N; ++i)
		{
			auto temp = m_next;
			m_next += m_curr;
			m_curr = temp;
		}
	}
	
	constexpr auto Get() const
	{
		return m_curr;
	}

private:
	std::size_t m_curr{0};
	std::size_t m_next{1};
};


/****************************************************************************
*																			*
*****************************************************************************/

template<typename T>
constexpr T Sum(T t)
{
	return t;
}

template<typename T, typename... Ts>
constexpr auto Sum(T t, Ts... ts)
{
	return t + Sum(ts...);
}


template<typename... Types>
constexpr auto Diff(Types... args)
{
	return (... - args);
}

template<>
constexpr auto Diff(void)
{
	return 0;
}

/****************************************************************************
*																			*
*****************************************************************************/

template<std::size_t Num, std::size_t Div>
struct IsNotDivisible
{
	static constexpr bool value = (Num % Div != 0);
};

template<std::size_t Num, std::size_t Div>
struct IsPrimeImpl;

template<std::size_t Num>
struct IsPrimeImpl<Num, 2> : IsNotDivisible<Num, 2>{};

template<std::size_t Num, std::size_t Div>
struct IsPrimeImpl
{
	static constexpr bool value = IsNotDivisible<Num, Div>::value && IsPrimeImpl<Num, Div - 1>::value;
};

template<std::size_t Num>
struct IsPrime : IsPrimeImpl<Num, Num/2>{};

template<>
struct IsPrime<1> : std::true_type{};

template<>
struct IsPrime<2> : std::true_type{};




} // namespace ilrd_5678


#endif /* __METAPROG_H_RD5678_ILRD__ */
