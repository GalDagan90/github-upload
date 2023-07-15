#include <iostream>
#include <memory>
#include <string>
#include <cassert>
#include "shared_ptr.hpp"

using namespace ilrd_5678;

#define UNUSED(x) (void)(x)


class Base
{
public:
	Base()
	{
		std::cout << "Base()\n";
	}

	virtual ~Base()
	{
		std::cout << "~Base()\n";
	}

	virtual void Print() const
	{
		std::cout << "I am base\n";
	}
};

class Derived : public Base
{
public:
	Derived()
	{
		std::cout << "Derived()\n";
	}

	~Derived()
	{
		std::cout << "~Derived()\n";
	}

	void Print() const override
	{
		std::cout << "I am Derived\n";
	}
};

void TestBasicConstruction(void);
void TestCopyAndAssignment(void);
void TestMoveAndAssignment(void);
void TestMakeShared(void);

int main()
{
	TestBasicConstruction();
	TestCopyAndAssignment();
	TestMoveAndAssignment();
	TestMakeShared();

	return 0;
}

void TestBasicConstruction(void)
{
	Shared_ptr<int> Shptr1{};
	Shared_ptr<int> Shptr2{nullptr};
	Shared_ptr<int> Shptr3{new int(5)};

	auto del = [](float* pf){ 
					std::cout << "Costum deleter\n";
					delete pf; 
				};
	float* fp = new float();
	Shared_ptr<float, decltype(del)> Shptr4{fp, del};

	Shared_ptr<Base> Shptr5{new Derived};
}

void TestCopyAndAssignment(void)
{
	Shared_ptr<int> Shptr1{new int(5)};
	assert(Shptr1.UseCount() == 1);
	Shared_ptr<int> Shptr2{Shptr1};
	assert(Shptr1.UseCount() == 2);
	assert(Shptr2.UseCount() == 2);


	Shared_ptr<Derived> Shptr3{new Derived()};
	Shared_ptr<Derived> Shptr4{Shptr3};
	Shared_ptr<Base> Shptr5{Shptr3};
	assert(Shptr3.UseCount() == 3);
	
	
	Shared_ptr<int> Shptr6{new int(42)};
	assert(Shptr6.UseCount() == 1);
	Shptr6 = Shptr1;
	assert(Shptr6.UseCount() == 3);


}

void TestMoveAndAssignment(void)
{
	Shared_ptr<int> Shptr1{new int(5)};
	assert(Shptr1.UseCount() == 1);
	Shared_ptr<int> Shptr2{std::move(Shptr1)};
	assert(Shptr2.UseCount() == 1);

	Shared_ptr<int> Shptr3{Shptr2};
	assert(Shptr2.UseCount() == 2);
	assert(Shptr3.UseCount() == 2);
	Shared_ptr<int> Shptr4{std::move(Shptr2)};
	assert(Shptr3.UseCount() == 2);
	assert(Shptr4.UseCount() == 2);

	Shared_ptr<int> Shptr5{Shptr4};
	Shared_ptr<int> Shptr6{Shptr4};
	assert(Shptr3.UseCount() == 4);
	assert(Shptr4.UseCount() == 4);
	assert(Shptr5.UseCount() == 4);
	assert(Shptr6.UseCount() == 4);

	Shared_ptr<int> Shptr7{new int(10)};
	assert(Shptr7.UseCount() == 1);
	Shptr6 = std::move(Shptr7);
	assert(Shptr6.UseCount() == 1);
	assert(Shptr3.UseCount() == 3);
}

void TestMakeShared(void)
{
	struct A
	{
		A(int i, bool b, float f) : m_i{i}, m_b{b}, m_f{f} {}
		
		int m_i;
		bool m_b;
		float m_f;
	};

	auto Shptr1 = Make_shared<A>(4,true,1.2f);

	assert(Shptr1->m_b == true);
	assert((*Shptr1).m_f == 1.2f);
	assert(Shptr1.Get()->m_i == 4);

}