#pragma once

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#else

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using SOCKET = int;
#define SD_BOTH SHUT_RDWR
#define closesocket(s) close(s)
#endif

#include <string>
#include <vector>
#include <array>
#include <functional>
#include <iostream>
#include <vector>

namespace thunderchat
{
class ThunderChatServer
{
private:

public:
    ThunderChatServer(std::string servAddress, u_short port);

    void OnConnect();

    void OnDisconnect();

    void Stop();
};
} // namespace ThunderChat