#include <iostream>

#include "word_painter.hpp"

namespace wordle_game
{

/*******************************************************************************
*								Class WrodPainter							   *
*******************************************************************************/

WordPainter::WordPainter(std::string winningWord)
	: m_winningWord(winningWord) ,m_guesses(), m_letteresUsed(), m_colors()
{
	InitColorArr();
}

void WordPainter::Display()
{
	DisplayGuesses();
	DisplayLetters();
}

void WordPainter::AddGuess(std::string str)
{
	CharColorPairVec vec;

	int counter = 0;
	for (auto it = str.begin(); it != str.end(); ++it, ++counter)
	{
		if (IsLetterInWord(*it))
		{
			if (IsLetterInPlace(str, counter))
			{
				MakePair(vec, static_cast<char>(*it), GREEN);
			}
			else
			{
				MakePair(vec, static_cast<char>(*it), YELLOW);
			}
		}
		else
		{
			MakePair(vec, static_cast<char>(*it), WHITE);
		}

		m_letteresUsed.insert(static_cast<char>(*it));
	}

	m_guesses.emplace_back(vec);
}

/******************************************************************************/

void WordPainter::InitColorArr()
{
	m_colors[WHITE] = std::string("\033[0m");
	m_colors[YELLOW] = std::string("\033[0;33m");
	m_colors[GREEN] = std::string("\033[0;32m");
}

void WordPainter::DisplayGuesses()
{
	for (auto outerIt : m_guesses)
	{
		for (auto innerIt : outerIt)
		{
			std::cout << m_colors[innerIt.second] << innerIt.first;
		}
		std::cout << "\n";
	}
}

void WordPainter::DisplayLetters()
{
	std::cout << m_colors[WHITE] <<"\nLetters Used:\n";
	std::cout <<  "[";
	for (auto it : m_letteresUsed)
	{
		std::cout << it << " ";
	}
	std::cout << "]\n";
}

bool WordPainter::IsLetterInWord(char ch)
{
	return (m_winningWord.find(ch) != std::string::npos);
}

bool WordPainter::IsLetterInPlace(std::string& str, int index)
{
	return (m_winningWord[index] == str[index]);
}

void WordPainter::MakePair(CharColorPairVec& vec, char letter, int color)
{
	auto ChColpair = std::make_pair(letter, color);
	vec.emplace_back(ChColpair);
}

}