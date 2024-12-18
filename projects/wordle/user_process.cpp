#include <iostream>
#include <fstream>

#include "user_process.hpp"

namespace wordle_game
{

/*******************************************************************************
*							Class UserInterface								   *
*******************************************************************************/

UserInterface::UserInterface(int selfPort, const char *selfIP, int clientPort, 
							const char *clientIP) : 
								m_udp{selfPort, selfIP, clientPort, clientIP}
{
	//empty Ctor
}

void UserInterface::Play()
{

	char win = 0;
	while (!win)
	{
		std::string guess = MakeGuess();
		m_udp.Send(guess.c_str(), guess.length() + 1);

		m_udp.Recv(&win, sizeof(char));
	}
}


/******************************************************************************/
std::string UserInterface::MakeGuess()
{
	std::string ret;
	std::getline(std::cin, ret);

	return ret;
}

} //namespace wordle_game