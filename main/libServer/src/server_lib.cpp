#include "server_lib.hpp"

namespace thunderchat
{
	ThunderChatServer::ThunderChatServer(std::string servAddress, u_short port) noexcept
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

		while (m_success)
		{
			Accept_Client();
			if (m_listeClient.size() > 0) { Receive_Client(); }
		}
	}

	ThunderChatServer::~ThunderChatServer()
	{
		for (auto& client : m_listeClient) { Disconnect_Client(client, " "); }
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
	}

	void ThunderChatServer::OnConnect(connectCallbackType connectCallback) noexcept
	{
		m_onDisconnectCallbacks.push_back(connectCallback);
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
				printf(" RECVBIG recv() error %ld.\n", WSAGetLastError());
				std::cout << "Error Accept Client -1 " << receivedBytes << std::endl;
				m_success = false;
				return false;
			}

			std::string msgstr(buffer.begin(), buffer.begin() + receivedBytes);
			nlohmann::json msg = nlohmann::json::parse(msgstr, nullptr, false);
			if (msg != nlohmann::detail::value_t::discarded)
			{
				Message fullMessage = Message(msg);
				if (this->nbEquipeA < 5 && fullMessage.get_team() == Message::A)
				{
					this->nbEquipeA += 1;
				}
				else if (this->nbEquipeB < 5 && fullMessage.get_team() == Message::B)
				{
					this->nbEquipeB += 1;
				}
				else
				{
					Disconnect_Client(client, fullMessage.get_username());
					return true;
				}
				this->m_listeClient.push_back(client);
				this->m_listeTeam.push_back(fullMessage.get_team());
				std::cout << fullMessage.get_username() << " Is Connected !"
					<< " Team " << fullMessage.get_team() << std::endl;
			}
		}
	}

	std::string GetLastErrorAsString()
	{
		// Get the error message, if any.
		DWORD errorMessageID = ::GetLastError();
		std::cout << GetLastErrorAsString() << std::endl;
		if (errorMessageID == 0) return std::string(); // No error message has been recorded

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		// Free the buffer.
		LocalFree(messageBuffer);

		return message;
	}

	bool ThunderChatServer::Receive_Client() noexcept
	{
		fd_set setReads;
		fd_set setErrors;
		FD_ZERO(&setReads);
		FD_ZERO(&setErrors);
		int highestFd = 0;
		timeval timeout = { 2 };
		for (auto& client : m_listeClient)
		{
			FD_SET(client, &setReads);
			FD_SET(client, &setErrors);
		}

		int selectResult = select(highestFd + 1, &setReads, nullptr, &setErrors, &timeout);
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

				if (FD_ISSET(client, &setErrors))
				{
					this->m_success = false;
					return false;
				}
				else if (FD_ISSET(client, &setReads))
				{
					std::array<char, 1024> buffer;
					int receivedBytes = recv(client, buffer.data(), 1024, 0);
					if (receivedBytes < 0)
					{

						std::cout << "Error Receive " << receivedBytes << std::endl;

						Disconnect_Client(client, " ");

						this->m_listeClient.erase(m_listeClient.begin() + index);
						this->m_listeTeam.erase(m_listeTeam.begin() + index);

						// break;
						return true;
					}

					std::cout << "I received : " << std::string(buffer.data(), receivedBytes).c_str()
						<< std::endl;

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
		timeval timeout = { 0 };
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
			int index = 0;
			for (auto& client : m_listeClient)
			{
				if (FD_ISSET(client, &setErrors))
				{
					this->m_success = false;
					return false;
				}
				else if (FD_ISSET(client, &setWrite))
				{

					if (msg.get_msg_type() == Message::PARTY ||
						(msg.get_msg_type() == Message::TEAM && m_listeTeam[index] == msg.get_team()))
					{

						std::string msgjson = msg.to_JSON().dump();
						int sentBytes =
							send(client, msg.to_JSON().dump().c_str(), msg.to_JSON().dump().size(), 0);

						if (msgjson.size() != sentBytes)
						{
							std::cout << "Error2" << std::endl;
							this->m_success = false;
							return false;
						}
						std::cout << "Send_to_Client " + msg.get_msg() << std::endl;
					}
				}
				index += 1;
			}
		}
	}

	void ThunderChatServer::Disconnect_Client(SOCKET client, std::string username) noexcept
	{
		std::cout << username + " SHUTDOWN" << std::endl;
		shutdown(client, SD_BOTH);
		closesocket(client);
	}

} // namespace thunderchat

Client::Client(SOCKET s, Message::Team team) noexcept
{
	this->m_socket = s;
	this->m_team = team;
}

SOCKET Client::getSocket() const { return this->m_socket; }

Message::Team Client::getTeam() const { return this->m_team; }

Client::~Client() noexcept
{

	shutdown(this->m_socket, SD_BOTH);
	closesocket(this->m_socket);
}