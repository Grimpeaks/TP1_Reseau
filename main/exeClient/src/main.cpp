#include <iostream>
#include "client_lib.hpp"

int main(void)
{
    //std::cout << "Client : Hello world" << std::endl;

	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
	thunderchat::ThunderChatClient client = thunderchat::ThunderChatClient("127.0.0.1:8888", "POUPOU", Message::A);
	
	std::function<void(const Message&)> printCallback = [](const Message& msg)
	{
		std::cout << msg.to_JSON().at("username") << " : " << msg.to_JSON().at("msg") << std::endl;
	};

	client.OnMessage(printCallback);
	bool sucess;
	sucess = client.Connect();

	if (sucess)
	{
		client.SendToTeam("COUCOU");
		std::cin.ignore();
		
	}
}