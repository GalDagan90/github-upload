#ifndef __MY_MINI_WORDLE_GAME_HPP__
#define __MY_MINI_WORDLE_GAME_HPP__

#include "mediator.hpp"
#include "udp_connector.hpp"

namespace wordle_game
{

class Wordle
{
public:
	explicit Wordle(int selfPort, const char *selfIP, int clientPort, 
				const char *clientIP);

	void GameMenu() const;
	void Play();

private:
	Mediator m_mediator;
	UDPConnector m_udp;
	bool m_gameON;
	bool m_win;

	std::string MakeGuess();
};
	

} // namespace wordle_game

#endif /* __MY_MINI_WORDLE_GAME_HPP__ */