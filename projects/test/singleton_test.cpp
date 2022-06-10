/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#include <iostream>
#include <cassert>
#include <boost/thread/thread.hpp>
#include <vector>

#include "singleton.hpp"

#define UNUSED(x) (void)(x)

using namespace framework;
using namespace std;

class Monkey
{
public:
	Monkey() : m_legs(2), m_voice("")
	{
		//
	}

	void GetNumOfLegs() 
	{ 
		std::cout << "A Monkey has: " << m_legs << " legs" << std::endl;
	}

	void Speak() 
	{
		std::cout << "A Monkey says : I love Banana" << std::endl;
	}

private:
	int m_legs;
	std::string m_voice;
};


void ThreadFunc()
{
	Monkey *monkey = Singleton<Monkey>::GetInstance();

	monkey->Speak();
	cout << monkey <<endl;

}



int main(void)
{
	boost::thread arr[5];
	for (int i = 0; i < 5; ++i)
	{
		arr[i] = boost::thread(ThreadFunc);
	}

	sleep(1);
	for (int i = 0; i < 5; ++i)
	{
		arr[i].join();
	}


	cout << "\nAll Good\n" << endl;


	return 0;
}
