#include "mediator.hpp"

namespace wordle_game
{

/*******************************************************************************
*								Class Mediator								   *
*******************************************************************************/
Mediator::Mediator() 
	: m_db{}, m_winningWord{m_db.ChooseWord()}, m_painter{m_winningWord}, m_currWord{}
{
	//empty Ctor
}

int Mediator::AcceptGuess(std::string word)
{
	if (!m_db.IsValidWord(word))
	{
		return NOT_VALID;
	}

	m_currWord = std::move(word);
	m_painter.AddGuess(m_currWord);
	m_painter.Display();

	return VALID;
}

bool Mediator::CheckWin() const
{
	return (m_winningWord.compare(m_currWord) == 0);
}

}//namespace wordle_game
