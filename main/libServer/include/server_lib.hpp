#include <string>
#include <vector>
#include <array>
#include <functional>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fmt/format.h>

#include "network.hpp"
#ifdef _WIN32
#include "winnetworkconfig.hpp"
#endif
#include "Message.hpp"

//class Client {
//public:
//	Client(SOCKET s, Message::Team team) noexcept;
//	SOCKET getSocket() const;
//	Message::Team getTeam() const;
//	~Client() noexcept;
//	
//private:
//	SOCKET m_socket;
//	Message::Team m_team;
//};

namespace thunderchat
{
	class ThunderChatServer
	{
	private:
		using connectCallbackType = std::function<void(const std::string & client)>;
		using disconnectCallbackType = std::function<void(const std::string & client)>;

		std::vector<connectCallbackType> m_onConnectCallbacks;
		std::vector<disconnectCallbackType> m_onDisconnectCallbacks;
		std::vector<std::tuple<SOCKET, sockaddr_in, std::string, Message::Team>> m_listeClient;

		SOCKET m_socket;

		bool m_success = true;
		bool Accept_Client() noexcept;
		bool Receive_Client() noexcept;
		bool Send_to_Client(Message msg) noexcept;
		void Disconnect_Client(std::tuple<SOCKET, sockaddr_in, std::string, Message::Team> client) noexcept;
		void RunServer() noexcept;
		int getNfds(std::vector<SOCKET> listeSockets) noexcept;
		std::unique_ptr<std::thread> m_ServerThread;

		int nbEquipeA = 0;
		int nbEquipeB = 0;

	public:
		ThunderChatServer(std::string servAddress, u_short port) noexcept;
		~ThunderChatServer() noexcept;
		void OnConnect(connectCallbackType connectCallback) noexcept;
		void OnDisconnect(disconnectCallbackType disconnectCallback) noexcept;
		void Stop() noexcept;
	};

} // namespace ThunderChat


