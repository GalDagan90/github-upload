/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/

#include "simple_string.hpp"

#include <cstring>

namespace ilrd_5678
{

/******************************************************************************
*								Class String    							  *
*******************************************************************************/
String::String(const char *str)
	: m_str(DupStr(str))
{
	//empty ctor
}

String::~String()
{
	delete[] m_str;
}

String::String(const String& other_)
	: m_str(DupStr(other_.Cstr()))
{
	std::cout << "=== Cctor called ===" << std::endl;
}

String::String(String&& other_)
	: m_str(other_.m_str)
{
	std::cout << "=== Move ctor called ===" << std::endl;
	other_.m_str = nullptr;
}

String& String::operator=(String other_)
{
	if (this != &other_)
	{
		std::swap(m_str, other_.m_str);
	}

	return *this;
}


const char *String::Cstr() const
{
	return m_str;
}

size_t String::Length() const
{
	return strlen(Cstr());
}

String& String::operator+=(const String& str)
{
	String temp(std::move(*this + str));

	*this = temp;

	return *this;
}

const char& String::operator[](size_t index) const
{
	return m_str[index];
}

char& String::operator[](size_t index)
{
	return m_str[index];
}

void String::Set(size_t index, char ch)
{
	m_str[index] = ch;
}
/******************************************************************************/

char *String::DupStr(const char *str)
{
	char *res = new char[strlen(str) + 1];

	std::memcpy(res, str, strlen(str) + 1);

	return res;
}

/******************************************************************************/

String operator+(const String& lhs, const String& rhs)
{
	size_t len = lhs.Length() + rhs.Length() + 1;
	char *temp = new char[len];

	memcpy(temp, lhs.Cstr(), lhs.Length());
	memcpy(temp + lhs.Length(), rhs.Cstr(), rhs.Length() + 1);

	String res(temp);
	delete[] temp;

	return res;
}

/******************************************************************************
*								Class Human	    							  *
*******************************************************************************/

Human::Human(const String& str)
	: m_data(str)
{
	//empty ctor
}

Human::~Human()
{
	//empty dtor
}

Human::Human(String&& str)
	: m_data(std::move(str))
{

}

const char *Human::GetName() const
{
	return m_data.Cstr();
}

} // namespace ilrd_5678