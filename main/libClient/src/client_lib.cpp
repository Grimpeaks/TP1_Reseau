#include "client_lib.hpp"

namespace thunderchat
{
	ThunderChatClient::ThunderChatClient(std::string servAddress, std::string userName, Message::Team team) noexcept
		: m_receiveThread(nullptr), m_servAddress("0.0.0.0"), m_servPort(8888), m_userName(userName), m_team(team), s(), addrv4Serv(),
		m_onMessageCallbacks(), m_onDisconnectCallbacks(), m_success(true)
	{
		std::cout << "Client" << std::endl;

		std::size_t found = servAddress.find(":");
		m_servAddress = std::string(servAddress.begin(), servAddress.begin() + found);
		m_servPort = std::stoi(std::string(servAddress.begin() + found + 1, servAddress.end()));
	}

	ThunderChatClient::~ThunderChatClient() noexcept
	{
		std::for_each(m_onDisconnectCallbacks.begin(), m_onDisconnectCallbacks.end(), [](disconnectCallbackType callback) {
			callback();
		});

		if (m_receiveThread != nullptr && m_receiveThread->joinable())
		{
			m_receiveThread->join();
		}

		shutdown(s, SD_BOTH);
		closesocket(s);
	}

	bool ThunderChatClient::Connect() noexcept
	{
		s = socket(AF_INET, SOCK_STREAM, 0);
		if (s <= 0)
		{
			std::cout << "Client : sock ERROR" << std::endl;
			m_success = false;
			return false;
		}

		sockaddr_in addrv4Serv;
		addrv4Serv.sin_family = AF_INET;
		addrv4Serv.sin_port = htons(m_servPort);
		if (inet_pton(AF_INET, m_servAddress.c_str(), &(addrv4Serv.sin_addr)) < 0)
		{
			std::cout << "Client : addr ERROR" << std::endl;
			m_success = false;
			return false;
		}

		if (connect(s, reinterpret_cast<sockaddr*>(&addrv4Serv), sizeof(sockaddr)) < 0)
		{
			std::cout << "Client : Connection ERROR" << std::endl;
			m_success = false;
			return false;
		}

		SendToParty("CONNECTED");

		m_receiveThread = std::make_unique<std::thread>([this]() {recvOnThread(); });

		return true;
	}

	void ThunderChatClient::recvOnThread() noexcept
	{
		while (m_success)
		{
			std::array<char, 1024> buffer;
			size_t receivedBytes = recv(s, buffer.data(), 1024, 0);

			if (receivedBytes > 0 && receivedBytes <= 1024)
			{
				std::string str(buffer.begin(), buffer.begin() + receivedBytes);
				nlohmann::json msg = nlohmann::json::parse(str, nullptr, false);
				if (msg.type() != nlohmann::detail::value_t::discarded)
				{
					Message fullMessage = Message(msg);
					std::for_each(m_onMessageCallbacks.begin(), m_onMessageCallbacks.end(), [&fullMessage](msgCallbackType callback) {
						callback(fullMessage);
					});
					receivedBytes = 0;
				}
				else 
				{
					std::cout << "Client : message not correctly formatted" << std::endl;
				}
			}
		}
	}

	void ThunderChatClient::OnMessage(msgCallbackType msgCallback) noexcept
	{
		m_onMessageCallbacks.push_back(msgCallback);
	}

	void ThunderChatClient::OnDisconnect(disconnectCallbackType disconnectCallback) noexcept
	{
		m_onDisconnectCallbacks.push_back(disconnectCallback);
	}

	void ThunderChatClient::SendToParty(const std::string& msg) noexcept
	{
		Message message = Message(m_userName, Message::PARTY, m_team, msg);
		sendJson(message.to_JSON());
	}

	void ThunderChatClient::SendToTeam(const std::string& msg) noexcept
	{
		Message message = Message(m_userName, Message::TEAM, m_team, msg);
		sendJson(message.to_JSON());
	}

	void ThunderChatClient::sendJson(nlohmann::json json) noexcept
	{
		std::string msg = json.dump();
		int sentJson = send(s, msg.c_str(), msg.size(), 0);
		if (sentJson < 0)
		{
			std::cout << "Client : send ERROR" << std::endl;
			m_success = false;
		}
	}
} // namespace thunderchat