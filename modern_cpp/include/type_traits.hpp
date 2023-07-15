
#ifndef __TYPE_TRAITS_H_RD5678_ILRD__
#define __TYPE_TRAITS_H_RD5678_ILRD__

#include <type_traits>

namespace ilrd_5678
{

/******************************************************************************/
/*									Is Same									  */
/******************************************************************************/

template <typename T, typename U>
struct IsSame : std::false_type{};

template <typename T>
struct IsSame<T, T> : std::true_type{};

template <typename T, typename U>
inline constexpr bool IsSame_v = IsSame<T, U>::value;

/******************************************************************************/
/*									Is Reference							  */
/******************************************************************************/

template <typename T>
struct IsRefence : std::false_type{};

template <typename T>
struct IsRefence<T&> : std::true_type{};

template <typename T>
inline constexpr bool IsReference_v = IsRefence<T>::value;

/******************************************************************************/
/*									Is Const								  */
/******************************************************************************/

template <typename T>
struct IsConst : std::false_type{};

template <typename T>
struct IsConst<const T> : std::true_type{};

template <typename T>
inline constexpr bool IsConst_v = IsConst<T>::value;

/******************************************************************************/
/*									Is Pointer								  */
/******************************************************************************/

template <typename T>
struct IsPointer : std::false_type{};

template <typename T>
struct IsPointer<T*> : std::true_type{};

template <typename T>
inline constexpr bool IsPointer_v = IsPointer<T>::value;
/******************************************************************************/
/*								Remove Const								  */
/******************************************************************************/
template <typename T>
struct RemoveConst
{
	using type = T;
};

template <typename T>
struct RemoveConst<const T>
{
	using type = T;
};

template<typename T>
using RemoveConst_t = typename RemoveConst<T>::type;

/******************************************************************************/
/*								Remove Reference							  */
/******************************************************************************/
template <typename T>
struct RemoveReference
{
	using type = T;
};

template <typename T>
struct RemoveReference<T&>
{
	using type = T;
};

template<typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

/******************************************************************************/
/*								Conditional									  */
/******************************************************************************/
template<bool B,typename T, typename F>
struct Conditional;

template<typename T, typename F>
struct Conditional<true, T, F>
{
	using type = T;
};

template<typename T, typename F>
struct Conditional<false, T, F>
{
	using type = F;
};

template<bool B,typename T, typename F>
using Conditional_t = typename Conditional<B,T,F>::type;
/******************************************************************************/
/*								Conjunction									  */
/******************************************************************************/
template<typename...Ts>
struct Conjunction;

template<typename T>
struct Conjunction<T>
{
	static constexpr bool value = T::value;
};


template<typename T, typename... Ts>
struct Conjunction<T, Ts...> : 
		Conditional_t<bool(T::value), Conjunction<Ts...>, std::false_type>
{};

template<typename...Ts>
inline constexpr bool Conjunction_v = Conjunction<Ts...>::value;

/******************************************************************************/
/*								Is Convertiable								  */
/******************************************************************************/
template<typename From,typename To>
struct IsConvertiable
{
private:
	static void Test(...);
	static To Test(To);
	static From MakeFrom();
public:
	enum {value = IsSame_v<decltype(Test(MakeFrom())), To> };
};

template<typename From,typename To>
inline constexpr bool IsConvertiable_v = IsConvertiable<From, To>::value;

/******************************************************************************/
/*								Is Null_ptr									  */
/******************************************************************************/
template<typename T>
struct IsNullPtr
{
	enum {value = IsSame_v<RemoveConst_t<RemoveReference_t<T>>, std::nullptr_t>};
};

template<typename T>
inline constexpr bool IsNullPtr_v = IsNullPtr<T>::value;
/******************************************************************************/
/*																			  */
/******************************************************************************/



} // namespace ilrd_5678


#endif /* __TYPE_TRAITS_H_RD5678_ILRD__ */
