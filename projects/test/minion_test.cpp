/****************************************************************************
*Author : Gal Dagan
*Date : 20.1.22
*Reviewer :
*
*Description : Minion test
*
*Lab : RD5678
****************************************************************************/
#include <iostream>

#include "minion.hpp"

#define UNUSED(x) (void)(x)

using namespace framework;

const char *ip = "127.0.0.1";

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cout << "argv error!" << std::endl;
		return 0;
	}
	
	int masterPort = atoi(argv[1]);
	int minionPort = atoi(argv[2]);
	
	Minion minion(128000000, ip, minionPort, ip, masterPort);

	minion.Run();

	return 0;
}



