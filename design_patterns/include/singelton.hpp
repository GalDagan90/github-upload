/****************************************************************************
*Author : Gal Dagan
*Date : 15/06/2022

****************************************************************************/
#ifndef __SINGELTON_H_RD5678_ILRD__
#define __SINGELTON_H_RD5678_ILRD__

#include <memory>
#include <mutex>

namespace design_pattern
{

template <typename T>
class Singelton
{
public:

	template <typename... params>
	static T& Instance(params... paramsList)
	{
		std::call_once(Singelton<T>::m_isCreated, 
					[&paramsList...](){m_obj.reset(new T(paramsList...));}
					);

		return *m_obj;
	}

private:
	Singelton();
	~Singelton() = default;
	Singelton(const Singelton& other) = delete;
	Singelton& operator=(const Singelton& other) = delete;

	static std::once_flag m_isCreated;
	static std::unique_ptr<T> m_obj;
};

template <typename T>
std::once_flag Singelton<T>::m_isCreated;

template <typename T>
std::unique_ptr<T> Singelton<T>::m_obj = nullptr; 

} // namespace design_pattern


#endif /* __SINGELTON_H_RD5678_ILRD__ */
