#include "client_lib.hpp"

namespace thunderchat
{
	ThunderChatClient::ThunderChatClient(std::string servAddress, std::string userName, Team team)
		: m_receiveThread(nullptr), m_servAddress(servAddress), m_userName(userName), m_team(team), s(), addrv4Serv() {}

	ThunderChatClient::~ThunderChatClient()
	{
		if (m_receiveThread != nullptr && m_receiveThread->joinable())
		{
			m_receiveThread->join();
		}

		shutdown(s, SD_BOTH);
		closesocket(s);
	}

	bool ThunderChatClient::Connect()
	{
		s = socket(AF_INET, SOCK_STREAM, 0);
		if (s <= 0)
		{
			std::cout << "Error2";
			return false;
		}

		sockaddr_in addrv4Serv;
		addrv4Serv.sin_family = AF_INET;
		addrv4Serv.sin_port = htons(8888);
		if (inet_pton(AF_INET, "127.0.0.1", &(addrv4Serv.sin_addr)) < 0)
		{
			std::cout << "Error3";
			return false;
		}

		if (connect(s, reinterpret_cast<sockaddr*>(&addrv4Serv), sizeof(sockaddr)) < 0)
		{
			std::cout << "Client : Connection ERROR";
			//closesocket(s);
			return false;
		}

		m_receiveThread = std::make_unique<std::thread>([this]() {recvOnThread(); });
		return true;
	}

	void ThunderChatClient::recvOnThread()
	{
		while (true)
		{
			std::array<char, 1024> buffer;
			size_t receivedBytes = recv(s, buffer.data(), 1024, 0);

			if (receivedBytes > 0)
			{
				std::string str(buffer.begin(), buffer.begin() + receivedBytes);
				std::for_each(m_onMessageCallbacks.begin(), m_onMessageCallbacks.end(), [&str](msgCallbackType callback) {
					callback(str);
				});
			}
		}
	}

	void ThunderChatClient::OnMessage(msgCallbackType msgCallback)
	{
		m_onMessageCallbacks.push_back(msgCallback);
	}

	void ThunderChatClient::OnDisconnect(disconnectCallbackType disconnectCallback)
	{
		m_onDisconnectCallbacks.push_back(disconnectCallback);
	}

	void ThunderChatClient::SendToParty(const std::string& msg)
	{
		nlohmann::json j = { {"username", m_userName},
							{"msg_type", PARTY},
							{"team", m_team},
							{"msg", msg} };
		sendJson(j);
	}

	void ThunderChatClient::SendToTeam(const std::string& msg)
	{
		nlohmann::json j = { {"username", m_userName},
							{"msg_type", TEAM},
							{"team", m_team},
							{"msg", msg} };
		sendJson(j);
	}

	void ThunderChatClient::SendString(const std::string& msg)
	{
		std::array<char, 1024> buffer{};
		std::cout << "Type here : ";
		std::cin >> buffer.data();
		int sentBytes = send(s, buffer.data(), buffer.size(), 0);
		if (sentBytes < 0)
		{
			printf(" RECVBIG recv() error %ld.\n", WSAGetLastError());
			std::cout << "Error5" << std::endl;
			//closesocket(s);
			//return EXIT_FAILURE;
		}
	}

	void ThunderChatClient::sendJson(nlohmann::json json)
	{
		int sentJson = send(s, json.dump().c_str(), json.size(), 0);
		if (sentJson < 0)
		{
			std::cout << "Client : send ERROR" << std::endl;
			//closesocket(s);
			// return EXIT_FAILURE;
		}
	}
} // namespace thunderchat