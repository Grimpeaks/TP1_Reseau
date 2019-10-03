#include "server_lib.hpp"

namespace thunderchat
{
ThunderChatServer::ThunderChatServer(std::string servAddress, u_short port) noexcept
{
#ifdef _WIN32
    network::WinNetworkConfig networkInit = network::WinNetworkConfig();
#endif
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


	this->m_ServerThread = std::make_unique<std::thread>([this]() {RunServer(); });
    
}



ThunderChatServer::~ThunderChatServer()
{
	if (m_ServerThread != nullptr && m_ServerThread->joinable()) {
		m_ServerThread->join();
	}
    for (auto& client : m_listeClient) { Disconnect_Client(client); }
    shutdown(m_socket, SD_BOTH);
    closesocket(m_socket);
}

void ThunderChatServer::OnConnect(connectCallbackType connectCallback) noexcept
{
	m_onConnectCallbacks.push_back(connectCallback);
}

void ThunderChatServer::OnDisconnect(disconnectCallbackType disconnectCallback) noexcept
{
    m_onDisconnectCallbacks.push_back(disconnectCallback);
}

void ThunderChatServer::Stop() noexcept { this->~ThunderChatServer(); }

bool ThunderChatServer::Accept_Client() noexcept
{
    if (this->nbEquipeA + this->nbEquipeB >= 10) { return true; }
    fd_set set;
    timeval time = {0};
	std::vector<SOCKET> listeSockets;
    FD_ZERO(&set);
    FD_SET(m_socket, &set);
	listeSockets.push_back(m_socket);
    int selectReady = select(getNfds(listeSockets), &set, nullptr, nullptr, &time);
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
        if (client <= 0)
        {
            std::cout << "Error Accept Client";
            m_success = false;
            return false;
        }

        std::array<char, 1024> buffer;
        int receivedBytes = recv(client, buffer.data(), 1024, 0);
        if (receivedBytes < 0)
        { 
            std::cout << "Error Accept Client Team " << receivedBytes << std::endl;
            m_success = false;
            return false;
        }

        std::string msgstr(buffer.begin(), buffer.begin() + receivedBytes);
        nlohmann::json msg = nlohmann::json::parse(msgstr, nullptr, false);
        if (msg != nlohmann::detail::value_t::discarded)
        {
            Message fullMessage = Message(msg);
			std::tuple tupleClient = std::make_tuple(client, fullMessage.get_username(), fullMessage.get_team());
            if (this->nbEquipeA < 5 && fullMessage.get_team() == Message::A)
            { this->nbEquipeA += 1; }
            else if (this->nbEquipeB < 5 && fullMessage.get_team() == Message::B)
            {
                this->nbEquipeB += 1;
            }
            else
            {
                Disconnect_Client(tupleClient);
                return true;
            }
			this->m_listeClient.push_back(tupleClient);

			std::for_each(m_onConnectCallbacks.begin(), m_onConnectCallbacks.end(), [tupleClient](connectCallbackType callback) {
				callback(std::get<1>(tupleClient));
			});

			Send_to_Client(Message(fullMessage.get_username(), Message::PARTY,fullMessage.get_team(),"IS CONNECTED"));
        }
    }
}

bool ThunderChatServer::Receive_Client() noexcept
{
    fd_set setReads;
    fd_set setErrors;
    FD_ZERO(&setReads);
    FD_ZERO(&setErrors);
    timeval timeout = {2};
	std::vector<SOCKET> listeSockets;
    for (auto& client : m_listeClient)
    {
        FD_SET(std::get<0>(client), &setReads);
        FD_SET(std::get<0>(client), &setErrors);
		listeSockets.push_back(std::get<0>(client));
    }

    int selectResult = select(getNfds(listeSockets), &setReads, nullptr, &setErrors, &timeout);
    if (selectResult == -1)
    {
        this->m_success = false;
        return false;
    }

    else if (selectResult > 0)
    {

        int index = -1;
        for (auto& client : m_listeClient)
        {
            index += 1;

            if (FD_ISSET(std::get<0>(client), &setErrors))
            {
                this->m_success = false;
                return false;
            }
            else if (FD_ISSET(std::get<0>(client), &setReads))
            {
                std::array<char, 1024> buffer;
                int receivedBytes = recv(std::get<0>(client), buffer.data(), 1024, 0);
                if (receivedBytes < 0)
                {
					Disconnect_Client(client);

                    this->m_listeClient.erase(m_listeClient.begin() + index);
                    return true;
                }
                std::string str(buffer.begin(), buffer.begin() + receivedBytes);
                nlohmann::json msg = nlohmann::json::parse(str, nullptr, false);
                if (msg.type() != nlohmann::detail::value_t::discarded)
                {
                    Message fullMessage = Message(msg);
                    Send_to_Client(fullMessage);
                }
                
            }
        }
    }
}

bool ThunderChatServer::Send_to_Client(Message msg) noexcept
{

    fd_set setWrite;
    fd_set setErrors;
    FD_ZERO(&setWrite);
    FD_ZERO(&setErrors);
    int highestFd = 0;
    timeval timeout = {0};
	std::vector<SOCKET> listeSockets;
    for (auto& client : this->m_listeClient)
    {
        FD_SET(std::get<0>(client), &setWrite);
        FD_SET(std::get<0>(client), &setErrors);
		listeSockets.push_back(std::get<0>(client));
    }

    int selectResult = select(getNfds(listeSockets), nullptr, &setWrite, &setErrors, &timeout);
    if (selectResult == -1)
    {
        this->m_success = false;
        return false;
    }

    else if (selectResult > 0)
    {
        int index = -1;
        for (auto& client : m_listeClient)
        { 
			index += 1;
            if (FD_ISSET(std::get<0>(client), &setErrors))
            {
                this->m_success = false;
                return false;
            }
            else if (FD_ISSET(std::get<0>(client), &setWrite))
            {

                if (msg.get_msg_type() == Message::PARTY ||
                    (msg.get_msg_type() == Message::TEAM && std::get<2>(client) == msg.get_team()))
                {

                    std::string msgjson = msg.to_JSON().dump();
                    int sentBytes =send(std::get<0>(client), msg.to_JSON().dump().c_str(), msg.to_JSON().dump().size(), 0);
                    if (msgjson.size() != sentBytes)
                    {
						//this->m_listeClient.erase(m_listeClient.begin() + index);
						return true;
                    }
                }
            }
           
        }
    }
}

void ThunderChatServer::Disconnect_Client(std::tuple<SOCKET, std::string, Message::Team> client) noexcept
{
	std::string username = std::get<1>(client);
	std::for_each(m_onDisconnectCallbacks.begin(), m_onDisconnectCallbacks.end(), [username](disconnectCallbackType callback) {
		callback(username);
	});

	Send_to_Client(Message(username, Message::PARTY, std::get<2>(client), " IS DISCONNECTED"));

    shutdown(std::get<0>(client), SD_BOTH);
    closesocket(std::get<0>(client));
}

int ThunderChatServer::getNfds(std::vector<SOCKET> listeSockets) noexcept
{
	#ifdef _WIN32
		return 0;
	#else
		return std::max_element(listeSockets) + 1;
	#endif // _WIN32
}

void ThunderChatServer::RunServer() noexcept
{
	while (m_success)
	{
		Accept_Client();
		if (m_listeClient.size() > 0) { Receive_Client(); }
	}
}

} // namespace thunderchat

//Client::Client(SOCKET s, Message::Team team) noexcept
//{
//	this->m_socket = s;
//	this->m_team = team;
//}
//
//SOCKET Client::getSocket() const { return this->m_socket; }
//
//Message::Team Client::getTeam() const { return this->m_team; }
//
//Client::~Client() noexcept
//{
//
//	shutdown(this->m_socket, SD_BOTH);
//	closesocket(this->m_socket);
//}