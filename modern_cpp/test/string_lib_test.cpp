/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#include <iostream>

#include "string_lib.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)

int main()
{
	std::string s1("Rachel");
	std::string s2("Gal");

	std::cout << ((StrCmp(s1, s2) != 0)? "Passed" : "Failed") << std::endl;

	std::cout << "strlen: " << StrLen(s1) << std::endl;
	std::cout << "strlen: " << s1.size() << std::endl;

	char buffer[256];
	StrCpy(buffer, s1);
	std::cout << "strcpy: " << buffer << std::endl;
	// s1.copy(buffer, s1.size(), 0);
	// std::cout << buffer << std::endl;


	return 0;
}



