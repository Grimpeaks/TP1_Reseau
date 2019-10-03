#pragma once

#include <array>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <string>
#include <thread>
#include <memory>
 
#include "network.hpp"
#ifdef _WIN32
#include "winnetworkconfig.hpp"
#endif
#include "Message.hpp"

namespace thunderchat
{
class ThunderChatClient
{
private:
	using msgCallbackType = std::function<void(const Message & msg)>;
	using disconnectCallbackType = std::function<void()>;

    std::unique_ptr<std::thread> m_receiveThread;
    std::string m_servAddress;
	int m_servPort;
    std::string m_userName;
    Message::Team m_team;
    SOCKET s;
    sockaddr_in addrv4Serv;
	std::vector<msgCallbackType> m_onMessageCallbacks;
	std::vector<disconnectCallbackType> m_onDisconnectCallbacks;
	bool m_success;

	void sendJson(nlohmann::json json) noexcept;
	void recvOnThread() noexcept;

public:
    ThunderChatClient(std::string servAddr, std::string usrName, Message::Team team) noexcept;
    ~ThunderChatClient() noexcept;
    bool Connect() noexcept;
    void OnMessage(msgCallbackType) noexcept;
    void OnDisconnect(disconnectCallbackType) noexcept;
    void SendToParty(const std::string& msg) noexcept;
    void SendToTeam(const std::string& msg) noexcept;
	void SendString(const std::string& msg) noexcept;
};
} // namespace thunderchat