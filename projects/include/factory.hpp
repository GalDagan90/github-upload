/****************************************************************************
*Author : Gal Dagan
*Date : 30.12.21
*Reviewer : Navot Yaari
*
*Description : Factory design implementation
*
*Lab : RD5678
****************************************************************************/
#ifndef __ILRD_RD5678_FACTORY_HPP__
#define __ILRD_RD5678_FACTORY_HPP__

#include <boost/noncopyable.hpp> 		// boost::noncopyable
#include <boost/unordered_map.hpp> 	// boost::unordered_map

namespace framework
{

template <typename Key, typename Base>
class Factory : private boost::noncopyable
{
public:
    void Add(const Key& key, Base *(*creator)());
    Base *Create(const Key& key) const;

private:
    boost::unordered_map<Key, Base *(*)()> m_map;
};

/*******************************************************************************
*							Factory Class									   *
*******************************************************************************/
template <typename Key, typename Base>
void Factory<Key, Base>::Add(const Key& key, Base *(*creator)())
{
	if (m_map.find(key) != m_map.end())
	{
		throw "Key allready exists";
	}

	m_map[key] = creator;
}

template <typename Key, typename Base>
Base *Factory<Key, Base>::Create(const Key& key) const
{
	if (m_map.find(key) == m_map.end())
	{
		throw "Key not found";
	}

	return m_map.at(key)();
}

} // framework

#endif // __ILRD_RD5678_FACTORY_HPP__
