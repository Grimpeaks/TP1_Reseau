#include "server_lib.hpp"


namespace thunderchat
{
ThunderChatServer::ThunderChatServer(std::string servAddress, u_short port)
{
	network::WinNetworkConfig networkInit = network::WinNetworkConfig();
    this->m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0)
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

    if (bind(m_socket, reinterpret_cast<sockaddr*>(&addrv4), sizeof(sockaddr)) < 0)
    {
        std::cout << "Error";
        closesocket(m_socket);
		m_success = false;
		return;
    }

	if (listen(m_socket, 255) < 0)
	{
		std::cout << "Error";
		closesocket(m_socket);
		m_success = false;
		return;
	}
	
	while (true)
	{
		Accept_Client();
		if (m_listeClient.size() > 0) {
			Recieve_Client();
		}
		
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

bool ThunderChatServer::Accept_Client() {
	fd_set set;
	timeval time = { 0 };
	FD_ZERO(&set);
	FD_SET(m_socket, &set);
	int selectReady = select(0, &set, nullptr, nullptr, &time);
	if (selectReady == -1)
	{
		std::cout << "Error select accept" << std::endl;
		return false;
	}
	else if (selectReady > 0)
	{
		sockaddr clientAddr;
		socklen_t clientAddrSize = sizeof(clientAddr);
		SOCKET client = accept(m_socket, &clientAddr, &clientAddrSize);
		std::cout << "Accept_Client ";
		if (client <= 0)
		{
			std::cout << "Error";
			closesocket(this->m_socket);
			m_success = false;
			return false;
		}

		std::array<char, 512> ipClientStr;
		std::cout << "Client " << inet_ntop(clientAddr.sa_family, &clientAddr, ipClientStr.data(), 512)
			<< " is connected" << std::endl;
		this->m_listeClient.push_back(client);
	}
}

bool ThunderChatServer::Recieve_Client() {
	fd_set setReads;
	fd_set setErrors;
	FD_ZERO(&setReads);
	FD_ZERO(&setErrors);
	int highestFd = 0;
	timeval timeout = { 0 };
	for (auto& client : m_listeClient)
	{
		FD_SET(client, &setReads);
		FD_SET(client, &setErrors);
	}

	int selectResult = select(highestFd + 1, &setReads, nullptr, &setErrors, &timeout);
	if (selectResult == -1) {
		this->m_success = false;
		return false;
	}

	else if (selectResult > 0) {
		for (auto& client : m_listeClient)
		{
			if (FD_ISSET(client, &setErrors)) {
				this->m_success = false;
				return false;
			}
			else if (FD_ISSET(client, &setReads)) {
				std::array<char, 1024> buffer;
				int receivedBytes = recv(client, buffer.data(), 1024, 0);
				if (receivedBytes < 0)
				{
					printf(" RECVBIG recv() error %ld.\n", WSAGetLastError());
					std::cout << "Error1 " << receivedBytes << std::endl;
					closesocket(this->m_socket);
					return false;
				}

				std::cout << "I received : " << std::string(buffer.data(), receivedBytes).c_str() << std::endl;
				
				Send_to_Client();
			}
		}
	}

}

bool ThunderChatServer::Send_to_Client()
{
    fd_set setWrite;
    fd_set setErrors;
    FD_ZERO(&setWrite);
    FD_ZERO(&setErrors);
    int highestFd = 0;
    timeval timeout = {0};
    for (auto& client : this->m_listeClient)
    {
        FD_SET(client, &setWrite);
        FD_SET(client, &setErrors);
    }

    int selectResult = select(highestFd + 1, nullptr, &setWrite, &setErrors, &timeout);
    if (selectResult == -1)
    {
        this->m_success = false;
        return false;
    }

    else if (selectResult > 0)
    {
        for (auto& client : m_listeClient)
        {
            if (FD_ISSET(client, &setErrors))
            {
                this->m_success = false;
                return false;
            }
            else if (FD_ISSET(client, &setWrite))
            {

				Message msg = Message("SALEPUTE", Message::PARTY, Message::A, "SALEPUTE");
                std::string msgjson = msg.to_JSON().dump();
                int sentJson = send(client, msgjson.c_str(), msgjson.size(), 0);

                int sentBytes = send(client, msg.to_JSON().dump().c_str(),msg.to_JSON().dump().size(), 0);
                if (msgjson.size() != sentBytes)
                {
                    std::cout << "Error2" << std::endl;
                    closesocket(client);
                    return EXIT_FAILURE;
                }
            }
        }
    }
}

} // namespace thunderchat