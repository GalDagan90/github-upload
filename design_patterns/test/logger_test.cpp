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

#include "logger.hpp"
#include "singelton.hpp"

using namespace design_pattern;

#define UNUSED(x) (void)(x)

int main()
{
	std::string addr{"/home/user/github/design_patterns/logger_2.txt"};
	Logger& log = Singelton<Logger>::Instance(addr);
	log.Log("First log\n");
	log.SetAddress("/home/user/github/design_patterns/logger.txt");
	log.Log("Second log\n");

	return 0;
}



