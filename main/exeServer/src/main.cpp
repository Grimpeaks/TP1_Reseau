#include <iostream>
#include <server_lib.hpp>
using namespace thunderchat;

int main(void)
{
    //std::cout << "Server : Hello world" << std::endl;
#ifdef _WIN32
	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
#endif
	thunderchat::ThunderChatServer server = thunderchat::ThunderChatServer("127.0.0.1", 8888);

	std::function<void(const std::string&)> CallbackConnected = [](const std::string& client)
	{
		std::cout << client << " IS CONNECTED" << std::endl;
	};

	std::function<void(const std::string&)> CallbackDisctonnected = [](const std::string& client)
	{
		std::cout << client << " IS DISCONNECTED" << std::endl;
	};

	server.OnConnect(CallbackConnected);
	server.OnDisconnect(CallbackDisctonnected);
	std::cin.ignore();
}

