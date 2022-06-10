/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/

#include "logger.hpp"

#define UNUSED(x) (void)(x)

using namespace framework;

int main(void)
{
	Logger *myLogger = Singleton<Logger>::GetInstance();

	myLogger->Log("Test Log Gal", Logger::INFO);

	return 0;
}
