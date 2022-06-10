#ifndef __WORDLE_DB_HPP__
#define __WORDLE_DB_HPP__

#include <string>
#include <unordered_set>

namespace wordle_game
{

class WordleDataBase
{
public:
	explicit WordleDataBase(const std::string path = "word_stash.txt");

	std::string ChooseWord();
	bool IsValidWord(const std::string& word) const;

private:
	std::unordered_set<std::string> m_wordDB;

	static std::unordered_set<std::string> LoadDB(const std::string& path);
	std::string GenerateRandomWord();
};


} //namespace wordle_game

#endif /* __WORDLE_DB_HPP__ */