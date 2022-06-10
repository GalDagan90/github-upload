#include <iostream>
#include <string>

#include "wordle.hpp"

using namespace wordle_game;

int port = 5000;
int clientPort = 5050;
const char *ip = "192.168.14.101";

int main()
{
	Wordle game(port, ip, clientPort, ip);

	game.Play();

	return 0;
}