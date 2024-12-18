#include <iostream>
#include <fstream>

#include "wordle.hpp"


namespace wordle_game
{

/*******************************************************************************
*								Class Wordle								   *
*******************************************************************************/
Wordle::Wordle(int selfPort, const char *selfIP, int clientPort, 
				const char *clientIP)
	: m_mediator{}, m_udp{selfPort, selfIP, clientPort, clientIP}, 
	m_gameON{true}, m_win{false}
{

}

void Wordle::GameMenu() const
{

}

void Wordle::Play()
{
	auto turns = 6;

	while (turns && !m_win)
	{
		char buffer[10] = {'\0'};
		m_udp.Recv(buffer, 10);
		std::string currGuess = buffer;
		if (Mediator::GuessOutcome::NOT_VALID == m_mediator.AcceptGuess(currGuess))
		{
			std::cout << "Not A word!\n";
			m_udp.Send(reinterpret_cast<char *>(&m_win), sizeof(m_win));
			continue;
		}

		--turns;
		m_win = m_mediator.CheckWin();

		m_udp.Send(reinterpret_cast<char *>(&m_win), sizeof(m_win));
	}

	if (0 == turns && !m_win)
	{
		char lose = 1;
		m_udp.Send(reinterpret_cast<char *>(&lose), sizeof(char));
	}
}

/******************************************************************************/

}// namespace wordle_game