/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __SHARED_PTR_H_RD5678_ILRD__
#define __SHARED_PTR_H_RD5678_ILRD__

#include <iostream>
#include <functional>

#include "shared_ptr_impl.hpp"

namespace ilrd_5678
{

class DefaultDeleter
{
public:
	template <typename PTR>
	void operator()(PTR* p)
	{
		if (nullptr != p)
		{
			delete p;
		}
	}
};

class DefaultArrDeleter
{
public:
	template <typename PTR>
	void operator()(PTR* p)
	{
		delete[] p;
	}
};

template <class T, typename Deleter = DefaultDeleter>
class Shared_ptr
{
public:
	Shared_ptr();
	
	Shared_ptr(std::nullptr_t);
	
	template <class Y, class D = DefaultDeleter> 
	explicit Shared_ptr(Y* ptr, D del = D{});

	~Shared_ptr();

	//Copy ctors
	Shared_ptr(Shared_ptr const& rhs);
	
	template<class Y> 
	Shared_ptr(Shared_ptr<Y> const& rhs);

	//Assignments
	Shared_ptr& operator=(Shared_ptr const & rhs);
	template<class Y> 
	Shared_ptr& operator=(Shared_ptr<Y> const& rhs);

	//Move ctors
	Shared_ptr(Shared_ptr&& rhs);
	template<class Y> 
	Shared_ptr(Shared_ptr<Y>&& rhs);

	//Move assignment
	Shared_ptr& operator=(Shared_ptr&& rhs); 
	template<class Y> 
	Shared_ptr& operator=(Shared_ptr<Y>&& rhs);

	T* Get() const;
	T* operator->() const;
	T& operator*() const;
	std::size_t UseCount() const;

	template <typename Y, typename D>
	friend class Shared_ptr;

private:
	T* m_ptr;
	ControlBlock* m_controlBlock;
	Deleter m_del;
	
	template <typename U>
	void CopyImpl(const Shared_ptr<U>& rhs);

	template <typename U>
	void MoveImpl(Shared_ptr<U>& rhs);

	void Clean();
};


/******************************************************************************/
/*								Shared_ptr									  */
/******************************************************************************/
template <typename T, typename Deleter>
Shared_ptr<T, Deleter>::Shared_ptr() : 
							m_ptr{nullptr}, 
							m_controlBlock{new ControlBlock}, 
							m_del{DefaultDeleter()}
{

}

template <typename T, typename Deleter>
Shared_ptr<T, Deleter>::Shared_ptr(std::nullptr_t) : Shared_ptr<T, Deleter>()
{

}

template <typename T, typename Deleter>
template <class Y, class D> 
Shared_ptr<T, Deleter>::Shared_ptr(Y* ptr, D del) :
										m_ptr{ptr},
										m_controlBlock{new ControlBlock},
										m_del{del}
{
	++m_controlBlock->m_counter;
}

template <typename T, typename Deleter>
Shared_ptr<T, Deleter>::~Shared_ptr()
{
	Clean();
}

template <typename T, typename Deleter>
Shared_ptr<T, Deleter>::Shared_ptr(Shared_ptr const& rhs) : 
										m_ptr{rhs.m_ptr},
										m_controlBlock{rhs.m_controlBlock},
										m_del{rhs.m_del}
{
	++m_controlBlock->m_counter;
}
	
template <typename T, typename Deleter>
template <class Y> 
Shared_ptr<T, Deleter>::Shared_ptr(Shared_ptr<Y> const& rhs) :
										m_ptr{rhs.m_ptr},
										m_controlBlock{rhs.m_controlBlock},
										m_del{rhs.m_del}
{
	++m_controlBlock->m_counter;
}

template <typename T, typename Deleter>
typename Shared_ptr<T, Deleter>::Shared_ptr& Shared_ptr<T, Deleter>::operator=(Shared_ptr const & rhs)
{
	if (this != &rhs)
	{
		Clean();
		CopyImpl(rhs);
	}

	return *this;
}

template <typename T, typename Deleter>
template<class Y> 
typename Shared_ptr<T, Deleter>::Shared_ptr& Shared_ptr<T, Deleter>::operator=(Shared_ptr<Y> const& rhs)
{
	if (this != &rhs)
	{
		Clean();
		CopyImpl(rhs);
	}

	return *this;
}

template <typename T, typename Deleter>
Shared_ptr<T, Deleter>::Shared_ptr(Shared_ptr&& rhs) :
	m_ptr{std::move(rhs.m_ptr)}, m_controlBlock{std::move(rhs.m_controlBlock)},
	m_del{std::move(rhs.m_del)}
{
	rhs.m_ptr = nullptr;
	rhs.m_controlBlock = new ControlBlock;
}

template <typename T, typename Deleter>
template<class Y> 
Shared_ptr<T, Deleter>::Shared_ptr(Shared_ptr<Y>&& rhs) :
	m_ptr{std::move(rhs.m_ptr)}, m_controlBlock{std::move(rhs.m_controlBlock)},
	m_del{std::move(rhs.m_dels)}
{
	rhs.m_ptr = nullptr;
	rhs.m_controlBlock = new ControlBlock;
}

template <typename T, typename Deleter>
typename Shared_ptr<T, Deleter>::Shared_ptr& Shared_ptr<T, Deleter>::operator=(Shared_ptr&& rhs)
{
	if (this != &rhs)
	{
		MoveImpl(rhs);
	}

	return *this;
}

template <typename T, typename Deleter>
template<class Y> 
typename Shared_ptr<T, Deleter>::Shared_ptr& Shared_ptr<T, Deleter>::operator=(Shared_ptr<Y>&& rhs)
{
	if (this != &rhs)
	{
		MoveImpl(rhs);
	}

	return *this;
}

template <typename T, typename Deleter>
T* Shared_ptr<T, Deleter>::Get() const
{
	return m_ptr;
}

template <typename T, typename Deleter>
T* Shared_ptr<T, Deleter>::operator->() const
{
	return m_ptr;
}

template <typename T, typename Deleter>
T& Shared_ptr<T, Deleter>::operator*() const
{
	return *m_ptr;
}


template <typename T, typename Deleter>
std::size_t Shared_ptr<T, Deleter>::UseCount() const
{
	return m_controlBlock->m_counter;
}

/******************************************************************************/
/*							Shared_ptr private members						  */
/******************************************************************************/
template <typename T, typename Deleter>
void Shared_ptr<T, Deleter>::Clean()
{
	if (0 < m_controlBlock->m_counter)
	{
		--m_controlBlock->m_counter;
	}

	if (0 == m_controlBlock->m_counter)
	{
		delete m_controlBlock;
		m_del(m_ptr);
	}
}

template <typename T, typename Deleter>
template <typename U>
void Shared_ptr<T, Deleter>::CopyImpl(const Shared_ptr<U>& rhs)
{
	m_ptr = rhs.m_ptr;
	m_controlBlock = rhs.m_controlBlock;
	m_del = rhs.m_del;
	++m_controlBlock->m_counter;
}

template <typename T, typename Deleter>
template <typename U>
void Shared_ptr<T, Deleter>::MoveImpl(Shared_ptr<U>& rhs)
{
	m_ptr = std::move(rhs.m_ptr);
	rhs.m_ptr = nullptr;

	--m_controlBlock->m_counter;
	m_controlBlock = std::move(rhs.m_controlBlock);
	rhs.m_controlBlock = new ControlBlock{};

	m_del = std::move(rhs.m_del);
}



/*****************************************************************************
 * 							Non-member functions
*****************************************************************************/


template<typename T, typename... Args>
Shared_ptr<T> Make_shared(Args&&... args)
{
	T* obj = new T{std::forward<Args>(args)...};

	return Shared_ptr<T>(obj);
}

} // namespace ilrd_5678


#endif /* __SHARED_PTR_H_RD5678_ILRD__ */
