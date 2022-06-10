/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#include "master.hpp"

#define UNUSED(x) (void)(x)
#define MASTER_PORT1 5000
#define MINION_PORT1 4950

#define MASTER_PORT2 5001
#define MINION_PORT2 4951

#define MASTER_PORT3 5002
#define MINION_PORT3 4952

#define THREAD_N 4

using namespace framework;

const char *ip = "127.0.0.1";

int main()
{
	Master master("/dev/nbd0", 128000000, THREAD_N, ip, MASTER_PORT1, MINION_PORT1);

	master.Run();

	return 0;
}



