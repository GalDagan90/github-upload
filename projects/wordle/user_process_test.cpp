#include <iostream>
#include <string>

#include "user_process.hpp"

using namespace wordle_game;

int port = 5000;
int clientPort = 5050;

const char *ip = "192.168.14.101";

int main()
{
	UserInterface user(clientPort, ip, port, ip);

	user.Play();

	return 0;
}