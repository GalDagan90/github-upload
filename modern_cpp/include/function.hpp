/****************************************************************************
*Author : Gal Dagan
*Date : 20/01/2024
*
*Description :

****************************************************************************/
#ifndef __FUNCTION_H_RD5678_ILRD__
#define __FUNCTION_H_RD5678_ILRD__

#include <iostream>

namespace ilrd_5678
{

/****************************************************************************/
template<typename Ret, typename... Args>
class FunctionBridge;

template<typename callable, typename Ret, typename... Args> 
class ConcreteFunctionBridge;
/****************************************************************************/
template<typename Signature>
class Function;

template<typename Ret, typename... Args>
class Function<Ret(Args...)>
{
public:
	Function() : m_bridge(nullptr) {}

	Function(const Function& other) : m_bridge(other.m_bridge->clone())
	{}

	Function(Function& other) : Function(static_cast<const Function&>(other))
	{}

	Function& operator=(const Function& other)
	{
		delete m_bridge;
		m_bridge = other.m_bridge->clone();
		return *this;
	}

	Function(Function&& other) : m_bridge(other.m_bridge)
	{
		other.m_bridge = nullptr;
	}

	Function& operator=(Function&& other)
	{
		delete m_bridge;
		m_bridge = other.m_bridge;
		other.m_bridge = nullptr;
		return *this;
	}

	~Function()
	{
		delete m_bridge;
	}

	template<typename F>
	Function(F&& f) : m_bridge(nullptr)
	{
		using callable = std::decay_t<F>;
		using bridge = ConcreteFunctionBridge<callable, Ret, Args...>;
		m_bridge = new bridge(std::forward<F>(f));
	}

	Ret operator()(Args...args) const
	{
		return m_bridge->invoke(std::forward<Args>(args)...);
	}

private:
	FunctionBridge<Ret, Args...>* m_bridge;
};


template<typename Ret, typename... Args>
class FunctionBridge
{
public:
	virtual ~FunctionBridge() {}
	virtual FunctionBridge* clone() const = 0;
	virtual Ret invoke(Args... args) const = 0;
};

template<typename callable, typename Ret, typename... Args> 
class ConcreteFunctionBridge : public FunctionBridge<Ret, Args...>
{
public:
	template<typename callableFwd>
	ConcreteFunctionBridge(callableFwd&& f) : m_functor(std::forward<callableFwd>(f))
	{}

	ConcreteFunctionBridge* clone() const override
	{
		return new ConcreteFunctionBridge(m_functor);
	}

	Ret invoke(Args... args) const override
	{
		return m_functor(std::forward<Args>(args)...);
	}

private:
	callable m_functor;
};

} // namespace ilrd_5678


#endif /* __FUNCTION_H_RD5678_ILRD__ */
