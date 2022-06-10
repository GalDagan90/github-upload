/******************************************************************************
 * Author: Yonatan Gross                                                      *
 * Reviewer: Gal                                   		                      *
 * Created: 9.1.2022                                                         *
 *                                                                            *
 * Description:																  *
 * Implementation for logger class.							          *
 *                                                                            *
 * Infinity Labs RD5678														  *
 ******************************************************************************/
#ifndef __ILRD_RD5678_REACTOR_HPP__
#define __ILRD_RD5678_REACTOR_HPP__

#include <boost/function.hpp>	 // boost::function
#include <boost/noncopyable.hpp> // boost::noncopyable

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>

#include <map> // std::map

namespace framework
{

	class Reactor : private boost::noncopyable
	{
	public:

		enum Mode
		{
			READ,
			WRITE,
			EXCEPTION
		};

		typedef boost::function<void(int)> func_void;

		explicit Reactor();
		~Reactor();

		void Add(int fd, Mode mode, func_void callback);
		void Remove(int fd, Mode mode);
		void Run();
		void Stop();

	private:
		typedef std::map<std::pair<int,Mode>, func_void> myMap;

		static const size_t MAX_SETS = 3;

		myMap m_eventHandlers;
		bool m_isRunning;
		fd_set m_handlersSets[MAX_SETS];

		int FindMax();
	};

} // framework

#endif // __ILRD_RD5678_REACTOR_HPP__
