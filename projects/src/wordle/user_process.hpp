#ifndef __USER_WORDLE_GAME_HPP__
#define __USER_WORDLE_GAME_HPP__

#include <string>

#include "udp_connector.hpp"

namespace wordle_game
{

class UserInterface
{
public:
	UserInterface(int selfPort, const char *selfIP, int clientPort, 
				const char *clientIP);

	void Play();

private:
	UDPConnector m_udp;

	std::string MakeGuess();
};

}//name space wordle_game

#endif /* __USER_WORDLE_GAME_HPP__ */