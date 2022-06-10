/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __SIMPLE_STRING_H_RD5678_ILRD__
#define __SIMPLE_STRING_H_RD5678_ILRD__

#include <iostream>
#include <utility>

namespace ilrd_5678
{

class String
{
public:
	String(const char *str = nullptr);
	~String();

	String(const String& other_);
	String(String&& other_);
	String& operator=(String other_);

	String& operator+=(const String& str);
	const char& operator[](size_t index) const;
	char& operator[](size_t index);

	const char *Cstr() const;
	size_t Length() const;

private:
	char *m_str;

	static char *DupStr(const char *str);

	void Set(size_t index, char ch);
};

String operator+(const String& lhs, const String& rhs);

class Human
{
public:
	Human(const String& str);
	~Human();
	Human(String&& str);

	const char *GetName() const;

private:
	String m_data;
};

} // namespace ilrd_5678


#endif /* __SIMPLE_STRING_H_RD5678_ILRD__ */
