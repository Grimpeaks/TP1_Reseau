#include <iostream>
#include "client_lib.hpp"

int main(void)
{
    std::cout << "Client : Hello world" << std::endl;

	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
	thunderchat::ThunderChatClient client = thunderchat::ThunderChatClient("127.0.0.1", "POUPOU", thunderchat::A);
	
	std::function<void(std::string)> y = [](std::string& msg)
	{
		std::cout << msg << std::endl;
	};

	client.OnMessage(y);
	bool sucess;
	sucess = client.Connect();

	if (sucess)
	{
		std::string msgg = "YPOOOOOoooooooooooooooo";
		client.SendString(msgg);
	}



}