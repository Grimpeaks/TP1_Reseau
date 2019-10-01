#include <iostream>
#include <server_lib.hpp>
using namespace thunderchat;

int main(void)
{
    //std::cout << "Server : Hello world" << std::endl;
	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
	thunderchat::ThunderChatServer server = thunderchat::ThunderChatServer("127.0.0.1", 8888);
}