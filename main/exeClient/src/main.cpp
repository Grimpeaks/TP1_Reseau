#include <iostream>
#include "client_lib.hpp"

int main(void)
{
    std::cout << "Client : Hello world" << std::endl;

	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
	thunderchat::ThunderChatClient APIclient = thunderchat::ThunderChatClient("127.0.0.1", "POUPOU", thunderchat::A);
	APIclient.Connect();

}