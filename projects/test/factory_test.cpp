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
#include <string>
#include <boost/container/throw_exception.hpp>

#include "factory.hpp"

#define UNUSED(x) (void)(x)

using namespace framework;

class Animal
{
public:
	virtual ~Animal() {}

	virtual void GetNumOfLegs() {}
	virtual void Speak() {}
};


class Dog : public Animal
{
public:
	Dog() : m_legs(4), m_voice("Woof")
	{
		//empty
	}

	void GetNumOfLegs() 
	{ 
		std::cout << "A Dog has: " << m_legs << " legs" << std::endl;
	}

	void Speak() 
	{
		std::cout << "A Dog says: " + m_voice << std::endl;
	}

private:
	int m_legs;
	std::string m_voice;
};

class Spider : public Animal
{
public:
	Spider() : m_legs(8), m_voice("Im spider-Man")
	{
		//empty
	}

	void GetNumOfLegs() 
	{ 
		std::cout << "A Spider has: " << m_legs << " legs" << std::endl;
	}

	void Speak() 
	{
		std::cout << "A Spider says: " + m_voice << std::endl;
	}

private:
	int m_legs;
	std::string m_voice;
};

class Monkey : public Animal
{
public:
	Monkey() : m_legs(2), m_voice("I love banana")
	{
		//empty
	}

	void GetNumOfLegs() 
	{ 
		std::cout << "A Monkey has: " << m_legs << " legs" << std::endl;
	}

	void Speak() 
	{
		std::cout << "A Monkey says: " + m_voice << std::endl;
	}

private:
	int m_legs;
	std::string m_voice;
};

Animal *DogCreator()
{
	return new Dog;
}

Animal *SpiderCreator()
{
	return new Spider;
}

Animal *MonkeyCreator()
{
	return new Monkey;
}

int main()
{
	Factory<std::string, Animal> animal_maker;

	animal_maker.Add("Dog", &DogCreator);
	animal_maker.Add("Spider", &SpiderCreator);
	animal_maker.Add("Monkey", &MonkeyCreator);

	Animal *dog = animal_maker.Create("Dog");
	dog->GetNumOfLegs();
	dog->Speak();
	delete dog;

	Animal *spider = animal_maker.Create("Spider");
	spider->GetNumOfLegs();
	spider->Speak();
	delete spider;

	Animal *monkey = animal_maker.Create("Monkey");
	monkey->GetNumOfLegs();
	monkey->Speak();
	delete monkey;

	std::cout << "\n==== Adding key that is allready present" << std::endl;
	try
	{
		animal_maker.Add("Dog", DogCreator);
	}
	catch(const char *e)
	{
		std::cerr << "Error!!! : " << e << '\n';
	}

	std::cout << "\n==== Creating A key that is not in the map" << std::endl;
	try
	{
		Animal *cat = animal_maker.Create("Cat");
		cat->GetNumOfLegs();
		cat->Speak();
		delete cat;
	}
	catch(const char *e)
	{
		std::cerr << "Error!!! : " << e << '\n';
	}

	return 0;
}