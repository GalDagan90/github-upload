#include <fstream>
#include <random>

#include "wordle_db.hpp"

namespace wordle_game
{

/*******************************************************************************
*							Class WordleDatabase							   *
*******************************************************************************/

WordleDataBase::WordleDataBase(const std::string path)
	: m_wordDB(LoadDB(path))
{
	//empty Ctor
}

std::string WordleDataBase::ChooseWord()
{
	std::string chosenWord;

	chosenWord = GenerateRandomWord();

	return chosenWord;
}

bool WordleDataBase::IsValidWord(const std::string& word) const
{
	auto it = m_wordDB.find(word);

	return (it != m_wordDB.end());

}

/******************************************************************************/

std::unordered_set<std::string> WordleDataBase::LoadDB(const std::string& path)
{
	std::unordered_set<std::string> word_db;
	std::fstream file(path, std::fstream::in);

	std::string word;
	while (std::getline(file, word))
	{
		word_db.insert(word);
	}

	file.close();

	return word_db;
}

std::string WordleDataBase::GenerateRandomWord()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0,1 + m_wordDB.size());

	auto randNum = dist(rng);
	auto it = m_wordDB.begin();
	std::advance(it, randNum);

	return *it;
}

}//namespace wordle_game