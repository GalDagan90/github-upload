#include <iostream>
#include <string>
#include <cstring>

#include "udp_connector.hpp"

using namespace wordle_game;

const int selfPort = 4950;
const int clientPort = 5000;
const char *IP = "192.168.14.101";
char text[] = "Hello From client";

struct server_test
{
	int id;
};

struct sender_test
{
	std::string text;
};

int main()
{
	sender_test sender_pack;
	sender_pack.text = text;

	UDPConnector client(selfPort, IP, clientPort, IP);

	for (size_t i = 0; i < 5; ++i)
	{
		client.Send(text, strlen(text) + 1);

		server_test server_pack;
		client.Recv(reinterpret_cast<char *>(&server_pack), sizeof(server_pack));
		std::cout << "Client got back: " << server_pack.id << "\n";
	}

	return 0;
}