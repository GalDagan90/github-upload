#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

std::string linuxDictFile = "/usr/share/dict/american-english";


int main()
{
	std::fstream dictionaryFile(linuxDictFile, std::fstream::in);
	std::fstream wordResv("word_stash.txt", std::fstream::out);

	std::string line;
	while (std::getline(dictionaryFile, line))
	{
		if (!isupper(line[0]) && line.length() == 5 && line.find('\'') == std::string::npos )
		{
			wordResv << line << "\n";
		}
	}

	dictionaryFile.close();
	wordResv.close();

	return 0;
}
