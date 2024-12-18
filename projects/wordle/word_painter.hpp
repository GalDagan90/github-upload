#ifndef __WORDLE_WORD_PAINTER_HPP__
#define __WORDLE_WORD_PAINTER_HPP__

#include <string>
#include <vector>
#include <unordered_set>

namespace wordle_game
{

class WordPainter
{
public:
	using CharColorPairVec = std::vector<std::pair<char, int>>;

	explicit WordPainter(std::string winningWord);

	enum Color
	{
		WHITE,
		YELLOW,
		GREEN,
		COLOR_NUM
	};

	void Display();
	void AddGuess(std::string str);

private:
	std::string m_winningWord;
	std::vector<CharColorPairVec> m_guesses;
	std::unordered_set<char> m_letteresUsed;
	std::array<std::string, COLOR_NUM> m_colors;

	void InitColorArr();
	void DisplayGuesses();
	void DisplayLetters();

	bool IsLetterInWord(char ch);
	bool IsLetterInPlace(std::string& str, int index);
	void MakePair(CharColorPairVec& vec, char letter, int color);
};

} //namespace

#endif /* __WORDLE_WORD_PAINTER_HPP__ */