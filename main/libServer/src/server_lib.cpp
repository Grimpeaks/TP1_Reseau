#include "server_lib.hpp"


namespace thunderchat
{
ThunderChatServer::ThunderChatServer(std::string servAddress, u_short port)
{
	#ifdef _WIN32
    WORD versionRequested;
    WSADATA wsaData;
    versionRequested = MAKEWORD(2, 2);
    if (WSAStartup(versionRequested, &wsaData) < 0)
    {
        std::cout << "Error";
        // return EXIT_FAILURE;
    }
	#endif

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        std::cout << "Error";
        // return EXIT_FAILURE;
    }

    sockaddr_in addrv4;
    addrv4.sin_family = AF_INET;
    addrv4.sin_port = htons(port);
    if (inet_pton(AF_INET, servAddress.c_str(), &(addrv4.sin_addr)) < 0)
    {
        std::cout << "Error";
        // return EXIT_FAILURE;
    }

    if (bind(s, reinterpret_cast<sockaddr*>(&addrv4), sizeof(sockaddr)) < 0)
    {
        std::cout << "Error";
        closesocket(s);
        // return EXIT_FAILURE;
    }
}

void ThunderChatServer::OnConnect()
{

}

void ThunderChatServer::OnDisconnect() {}

void ThunderChatServer::Stop() 
{

}
} // namespace thunderchat