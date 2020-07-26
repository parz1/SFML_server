#include "pch.h"
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <Windows.h>
#define MAX 1000

using namespace std;
int main() {
	//SERVER

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char connectionType, mode;
	char buffer[2000];
	std::size_t received;
	struct ClientStat
	{
		string name;
		int x, y, height;
	};

	std::cout << "Welcome to GAME_SERVER" << std::endl;

	sf::TcpListener listener;
	sf::SocketSelector selector;
	bool done = false;
	vector<sf::TcpSocket*> clients;
	vector<string> clientAccount;
	listener.getLocalPort();
	listener.listen(2000);
	selector.add(listener);
	bool pause;


	while (!done) {
		if (selector.wait()) {
			if (selector.isReady(listener)) {
				sf::TcpSocket* socket = new sf::TcpSocket;

				listener.accept(*socket);
				sf::Packet packet;
				string game_status;
				string account;
				if (socket->receive(packet) == sf::Socket::Done) {
					packet >> game_status >> account;
				}
				std::cout << game_status << account << endl;
				clients.push_back(socket);
				clientAccount.push_back(account);
				selector.add(*socket);
			}
			else {
				for (int i = 0; i < clients.size(); i++) {
					sf::Packet packet, sendpacket;
					if (selector.isReady(*clients[i])) {
						if (clients[i]->receive(packet) == sf::Socket::Done) {
							int height;
							packet >> height;
							//std::cout << height << endl;
							HANDLE hOut;
							COORD pos = { 5,5*i+5 };
							hOut = GetStdHandle(STD_OUTPUT_HANDLE);
							SetConsoleCursorPosition(hOut, pos);
							std::cout << clientAccount[i] << ": " << height;
							sendpacket << clientAccount[i] << height;
							for (int j = 0; j < clients.size(); j++) {
								if (i != j) {
									clients[j]->send(sendpacket);
								}
							}
						}
					}
				}
			}
		}
	}

	for (vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++) {
		delete* it;
	}

	return 0;
}