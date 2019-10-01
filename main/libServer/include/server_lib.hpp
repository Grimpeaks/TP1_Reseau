#include "network.hpp"
#include "winnetworkconfig.hpp"
#include "Message.hpp"

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
  using connectCallbackType =  std::function<void(const std::string& client)>;
  using disconnectCallbackType = std::function<void(const std::string& client)>;

  std::vector<connectCallbackType> m_onConnectCallbacks;
  std::vector<disconnectCallbackType> m_onDisconnectCallbacks;
  std::vector<SOCKET> m_listeClient;
  SOCKET m_socket;

  bool m_success=true;
  bool Accept_Client();
  bool Recieve_Client();
  bool Send_to_Client();

  int nbEquipeA;
  int nbEquipeB;

public:
    ThunderChatServer(std::string servAddress, u_short port);

    void OnConnect(connectCallbackType connectCallback);

    void OnDisconnect(disconnectCallbackType disconnectCallback);

    void Stop();
};

} // namespace ThunderChat


class Client {
public:
	Client(SOCKET s, Message::Team team);
	SOCKET getSocket();
	Message::Team getTeam();
	~Client();
private:
	SOCKET m_socket;
	Message::Team m_team;
};