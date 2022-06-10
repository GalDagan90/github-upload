/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __STRING_LIB_H_RD5678_ILRD__
#define __STRING_LIB_H_RD5678_ILRD__


#include <string>

namespace ilrd_5678
{

inline int StrCmp (const std::string& str1, const std::string& str2)
{
	auto runner1 = str1.begin();
	auto runner2 = str2.begin();
	for (; *runner1 == *runner2 && runner1 != str1.end(); 
		++runner1, ++runner2)
	{
		//empty loop
	}

	return (*runner1 - *runner2);
}

inline size_t StrLen (const std::string& str)
{
	size_t len = 0;
	for (auto it : str)
	{
		static_cast<void>(it);
		++len;
	}

	return len;
}

inline void StrCpy (char *dest, const std::string& source)
{
	for (auto& it: source)
	{
		*dest = it;
		++dest;
	}
}

} // namespace ilrd_5678


#endif /* __STRING_LIB_H_RD5678_ILRD__ */
