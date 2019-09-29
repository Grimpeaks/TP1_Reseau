#pragma once

#include <array>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <memory>

#include "network.hpp"
#include "winnetworkconfig.hpp"
#include "Message.hpp"


namespace thunderchat
{



class ThunderChatClient
{
private:
	using msgCallbackType = std::function<void(/*const Message & msg*/)>;
	using disconnectCallbackType = std::function<void()>;

    std::unique_ptr<std::thread> m_receiveThread;
    std::string m_servAddress;
    std::string m_userName;
    Message::Team m_team;
    SOCKET s;
    sockaddr_in addrv4Serv;
	std::vector<msgCallbackType> m_onMessageCallbacks;
	std::vector<disconnectCallbackType> m_onDisconnectCallbacks;

	void sendJson(nlohmann::json json);

public:
    ThunderChatClient(std::string servAddr, std::string usrName, Message::Team team);
    ~ThunderChatClient();
    bool Connect();
    void OnMessage(msgCallbackType);
    void OnDisconnect(disconnectCallbackType);
    void SendToParty(const std::string& msg);
    void SendToTeam(const std::string& msg);
};
} // namespace thunderchat