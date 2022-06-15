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

template <typename Object>
class Singelton
{
public:
	static Object& Instance()
	{
		std::call_once(Singelton<Object>::m_isCreated, 
						[](){m_obj.reset(new Object);}
					);

		return *m_obj;
	}

private:
	Singelton();
	~Singelton() = default;
	Singelton(const Singelton& other) = delete;
	Singelton& operator=(const Singelton& other) = delete;

	static std::once_flag m_isCreated;
	static std::unique_ptr<Object> m_obj;
};

template <typename Object>
std::once_flag Singelton<Object>::m_isCreated;

template <typename Object>
std::unique_ptr<Object> Singelton<Object>::m_obj = nullptr; 

} // namespace design_pattern


#endif /* __SINGELTON_H_RD5678_ILRD__ */
