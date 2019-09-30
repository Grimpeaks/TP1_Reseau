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

enum Team
{
    A = 0,
    B = 1
};

enum MsgType
{
    PARTY = 0,
    TEAM = 1
};

class ThunderChatClient
{
private:
	using msgCallbackType = std::function<void(std::string & msg/*const Message & msg*/)>;
	using disconnectCallbackType = std::function<void()>;

    std::unique_ptr<std::thread> m_receiveThread;
    std::string m_servAddress;
    std::string m_userName;
    Team m_team;
    SOCKET s;
    sockaddr_in addrv4Serv;
	std::vector<msgCallbackType> m_onMessageCallbacks;
	std::vector<disconnectCallbackType> m_onDisconnectCallbacks;

	void sendJson(nlohmann::json json);
	void recvOnThread();

public:
    ThunderChatClient(std::string servAddr, std::string usrName, Team team);
    ~ThunderChatClient();
    bool Connect();
    void OnMessage(msgCallbackType);
    void OnDisconnect(disconnectCallbackType);
    void SendToParty(const std::string& msg);
    void SendToTeam(const std::string& msg);
	void SendString(const std::string& msg);
};
} // namespace thunderchat