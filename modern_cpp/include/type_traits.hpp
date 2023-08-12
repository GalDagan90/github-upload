
#ifndef __TYPE_TRAITS_H_RD5678_ILRD__
#define __TYPE_TRAITS_H_RD5678_ILRD__

#include <type_traits>
#include <utility>

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
/*								Is Convertible								  */
/******************************************************************************/
template<typename From,typename To>
struct IsConvertible
{
private:
	static void HelperFunc(To);

	template<typename F, 
			typename = decltype(HelperFunc(std::declval<F>()))>
	static char Test(void*);
	
	template<typename>
	static long Test(...);
public:
	enum {value = IsSame_v<decltype(Test<From>(nullptr)), char> };
};

template<typename From,typename To>
inline constexpr bool IsConvertible_v = IsConvertible<From, To>::value;

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
/*								Is DefaultConstructible						  */
/******************************************************************************/

template<typename T>
struct IsDefaultConstructible
{
private:
	//Test() will accept a type that has a default constructor
	//T_ is introduced because T is allready  deduced at this point
	template<typename T_, typename = decltype( T_() )>
	static char Test(void*);

	//otherwise Test will default to this implementation
	template<typename>
	static long Test(...);
public:
	static constexpr bool value = IsSame_v< decltype(Test<T>(nullptr)), char>;
};

template<typename T>
inline constexpr bool IsDefaultConstructible_v = IsDefaultConstructible<T>::value;
/******************************************************************************/
/*									Is Class								  */
/******************************************************************************/

template<typename T, typename = std::void_t<>>
struct IsClass : std::false_type 
{};

//only class can have a pointer to member
template<typename T>
struct IsClass<T, std::void_t<int T::*>> : std::true_type{};

template<typename T>
inline constexpr bool IsClass_v = IsClass<T>::value;

} // namespace ilrd_5678


#endif /* __TYPE_TRAITS_H_RD5678_ILRD__ */
