/****************************************************************************
*Author : Gal Dagan
*Date : 
****************************************************************************/
#include <iostream>

#include "singelton.hpp"

using namespace design_pattern;

#define UNUSED(x) (void)(x)

class A
{
private:
	int m_x;
public:
	A(int x = 0) : m_x{x} {};

	void Print() const
	{
		std::cout << "m_x: " << m_x << "\n";
	}

	void Set(int x)
	{
		m_x = x;
	}
};

class B
{
private:
	int m_x;
public:
	B(int x) : m_x{x} {};
};

int main()
{
	A& single_A = Singelton<A>::Instance();

	A& second_A = Singelton<A>::Instance();
	
	single_A.Set(7);
	single_A.Print();
	second_A.Print();

	return 0;
}
