#ifndef __WORDLE_MEDIATOR_HPP__
#define __WORDLE_MEDIATOR_HPP__

#include <string>

#include "wordle_db.hpp"
#include "word_painter.hpp"

namespace wordle_game
{

class Mediator
{
public:
	explicit Mediator();

	enum GuessOutcome
	{
		VALID,
		NOT_VALID,
		BINGO
	};

	int AcceptGuess(std::string word);
	bool CheckWin() const;

private:
	WordleDataBase m_db;
	std::string m_winningWord;
	WordPainter m_painter;
	std::string m_currWord;
};


}//namespace wordle_game


#endif /* __WORDLE_MEDIATOR_HPP__ */