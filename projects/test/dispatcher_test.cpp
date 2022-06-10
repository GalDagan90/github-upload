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

#include "dispatcher.hpp"

#define UNUSED(x) (void)(x)

using namespace framework;
using namespace std;

void CallBack1(int val)
{
	cout << val << endl;
}

void CallBack2(int val)
{
	cout << val << endl;
}

void CallBack3(int val)
{
	cout << val << endl;
}

void DeathCallBack()
{
	cout << "Killed" << endl;
}

int main()
{
	Dispatcher<int> *disp = new Dispatcher<int>;
	Observer<int> *obs1 = new Observer<int>(*disp, CallBack1, DeathCallBack);
	Observer<int> *obs2 = new Observer<int>(*disp, CallBack2, DeathCallBack);
	Observer<int> *obs3 = new Observer<int>(*disp, CallBack3, DeathCallBack);

	disp->NotifyAll(1);

	delete obs1;

	disp->NotifyAll(2);

	delete disp;
	delete obs2;
	delete obs3;

	return 0;
}

