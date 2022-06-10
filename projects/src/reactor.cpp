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
#include <iostream>

#include "reactor.hpp"

namespace framework
{

static void CopySet(fd_set *masterSet, fd_set *copySet, size_t len);

Reactor::Reactor() : m_eventHandlers(),
						m_isRunning(false),
						m_handlersSets()
{
	for (size_t i = 0; i < MAX_SETS; ++i)
	{
		FD_ZERO(&m_handlersSets[i]);
	}
}

Reactor::~Reactor()
{

}

void Reactor::Add(int fd, Mode mode, func_void callback)
{
	using namespace std;
	m_eventHandlers.insert(make_pair(pair<int, Mode>(fd, mode), callback));

	FD_SET(fd, &m_handlersSets[mode]);
}

void Reactor::Remove(int fd, Mode mode)
{
	using namespace std;
	m_eventHandlers.erase(make_pair(fd, mode));
	FD_CLR(fd, &m_handlersSets[mode]);
}

void Reactor::Run()
{
	m_isRunning = true;
	while (m_isRunning)
	{
		fd_set copySets[MAX_SETS];
		CopySet(m_handlersSets, copySets, MAX_SETS);

		struct timeval tv = {1, 0};
		int max = FindMax();
		int retval = select(max + 1, &copySets[READ], &copySets[WRITE], 
							&copySets[EXCEPTION], &tv);

		if (retval == -1)
		{
			throw std::runtime_error("select() error");
		}

		for(myMap::iterator it = m_eventHandlers.begin(); 
			it != m_eventHandlers.end() && m_isRunning; ++it) 
    	{
			if (FD_ISSET(it->first.first, &copySets[it->first.second]))
			{
				it->second(it->first.first);
			}
		}
	}
}

void Reactor::Stop()
{
	m_isRunning = false;
}

int Reactor::FindMax()
{
	int max_fd = 0;
	myMap::iterator it = m_eventHandlers.begin();

	for (; it != m_eventHandlers.end(); ++it)
	{
		if (it->first.first > max_fd)
		{
			max_fd = it->first.first;
		}
	}

	return max_fd;
}

static void CopySet(fd_set *masterSet, fd_set *copySet, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		copySet[i] = masterSet[i];
	}	
}


} // framework