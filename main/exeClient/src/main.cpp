#include <iostream>
#include "client_lib.hpp"

int main(void)
{
    //std::cout << "Client : Hello world" << std::endl;

	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
		thunderchat::ThunderChatClient client = thunderchat::ThunderChatClient("127.0.0.1:8888", "POUPOU", Message::A);
		std::function<void(const Message&)> printCallback = [](const Message& msg)
		{
			std::cout << msg.get_username() << " : " << msg.get_msg() << std::endl;
		};
		client.OnMessage(printCallback);
		bool success;
		success = client.Connect();
		if (success)
		{
			std::cin.ignore();
			client.SendToTeam("COUCOU");
		}

	
	
}