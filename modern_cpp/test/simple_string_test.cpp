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

#include "simple_string.hpp"

using namespace ilrd_5678;
using namespace std;

#define UNUSED(x) (void)(x)

int main()
{
	String s1("Gal");

	cout << "s1: " << s1.Cstr() << endl;

	String s2(s1);
	cout << "s2: " << s2.Cstr() << endl;

	String s3 = "Rachel";
	cout << "s3: " << s3.Cstr() << endl;

	String s4 = s1 + s3;
	cout << "s4: " << s4.Cstr() << endl;

	s4 += s1;
	cout << "s4: " << s4.Cstr() << endl;

	cout << "s4[2] : " << s4[2] << endl;
	
	s4[2] = 'x';
	cout << "s4[2] = 'x': " << s4.Cstr() << endl;

	s1[0] = s3[0];
	cout << "s1: " << s1.Cstr() << endl;

	Human h1("Human");
	Human h2(s2);

	return 0;
}
