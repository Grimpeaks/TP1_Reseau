#include <iostream>
#include "client_lib.hpp"

int main(void)
{
    std::cout << "Client : Hello world" << std::endl;

	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
	thunderchat::ThunderChatClient client = thunderchat::ThunderChatClient("127.0.0.1:8888", "POUPOU", Message::A);
	
	std::function<void(std::string)> y = [](std::string& msg)
	{
		std::cout << msg << std::endl;
	};

	/*std::function<void(const Message&)> y = [](const Message& msg)
	{
		std::cout << msg << std::endl;
	};*/

	client.OnMessage(y);
	bool sucess;
	sucess = client.Connect();

	if (sucess)
	{
		std::string msgg = "YPOOOOOoooooooooooooooo";
		client.SendString(msgg);
		std::cin.ignore();
		client.SendString(msgg);
	}
}