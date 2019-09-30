#include "server_lib.hpp"


namespace thunderchat
{
ThunderChatServer::ThunderChatServer(std::string servAddress, u_short port)
{

	network::WinNetworkConfig networkInit = network::WinNetworkConfig();

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        std::cout << "Error";
		m_success = false;
		return;
    }

    sockaddr_in addrv4;
    addrv4.sin_family = AF_INET;
    addrv4.sin_port = htons(port);
    if (inet_pton(AF_INET, servAddress.c_str(), &(addrv4.sin_addr)) < 0)
    {
        std::cout << "Error";
		m_success = false;
		return;
    }

    if (bind(s, reinterpret_cast<sockaddr*>(&addrv4), sizeof(sockaddr)) < 0)
    {
        std::cout << "Error";
        closesocket(s);
		m_success = false;
		return;
    }
}

void ThunderChatServer::OnConnect(connectCallbackType connectCallback)
{
    m_onDisconnectCallbacks.push_back(connectCallback);
}

void ThunderChatServer::OnDisconnect(disconnectCallbackType disconnectCallback)
{ 
	m_onDisconnectCallbacks.push_back(disconnectCallback);
}

void ThunderChatServer::Stop() 
{


}


} // namespace thunderchat