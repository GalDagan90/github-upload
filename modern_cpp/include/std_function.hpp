/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __STD_FUNCTION_H_RD5678_ILRD__
#define __STD_FUNCTION_H_RD5678_ILRD__

#include <utility>

namespace ilrd_5678
{

//abstract functionBridge interface
template<typename R, typename... Args>
class FunctionBridge
{
public:
	virtual ~FunctionBridge() {}
	virtual FunctionBridge* clone() const = 0;
	virtual R invoke(Args... args) const = 0;
};

//concrete function bridge interface
template<typename Functor, typename R, typename... Args>
class SpecificFunctionBridge : public FunctionBridge<R, Args...>
{
private:
	Functor m_functor;

public:
	template<typename FunctorFwd>
	SpecificFunctionBridge(FunctorFwd&& f) : m_functor(std::forward<FunctorFwd>(f))
	{}

	SpecificFunctionBridge* clone() const override
	{
		return new SpecificFunctionBridge(m_functor);
	}
	
	R invoke(Args... args) const override
	{
		return m_functor(std::forward<Args>(args)...);
	}
};

template<typename Signature>
class FunctionPtr;

template<typename ReturnType, typename... Args>
class FunctionPtr<ReturnType(Args...)>
{
private:
	FunctionBridge<ReturnType, Args...>* m_bridge;
public:
	FunctionPtr(): m_bridge(nullptr)
	{}

	template<typename F>
	FunctionPtr(F&& f) : m_bridge(nullptr)
	{
		using Functor = std::decay_t<F>;
		using Bridge = SpecificFunctionBridge<Functor, ReturnType, Args...>;
		m_bridge = new Bridge(std::forward<F>(f));
	}

	~FunctionPtr()
	{
		if (m_bridge != nullptr)
			delete m_bridge;
	}

	FunctionPtr(const FunctionPtr& other) : m_bridge(other.m_bridge->clone())
	{}

	FunctionPtr(FunctionPtr& other) : FunctionPtr(static_cast<const FunctionPtr&>(other))
	{}

	FunctionPtr& operator=(const FunctionPtr& rhs)
	{
		FunctionPtr tmp(rhs);
		Swap(*this, rhs);
		return *this;
	}

	template<typename F>
	FunctionPtr& operator=(F&& f)
	{
		FunctionPtr tmp(std::forward<F>(f));
		Swap(*this, tmp);

		return *this;
	}

	FunctionPtr(FunctionPtr&& other) noexcept : m_bridge(other.m_bridge)
	{
		other.m_bridge = nullptr;
	}

	FunctionPtr& operator=(FunctionPtr&& rhs) noexcept
	{
		if (m_bridge != nullptr)
			delete m_bridge;

		m_bridge = rhs.m_bridge;
		rhs.m_bridge = nullptr;

		return *this;
	}

	ReturnType operator()(Args... args) const
	{
		m_bridge->invoke(std::forward<Args>(args)...);
	}

	friend void Swap(FunctionPtr& f1, FunctionPtr& f2)
	{
		std::swap(f1.m_bridge, f2.m_bridge);
	}
};


} // namespace ilrd_5678


#endif /* __STD_FUNCTION_H_RD5678_ILRD__ */
