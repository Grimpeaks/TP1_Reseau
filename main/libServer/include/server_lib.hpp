#include "network.hpp"
#include "winnetworkconfig.hpp"
#include "Message.hpp"

#include <string>
#include <vector>
#include <array>
#include <functional>
#include <iostream>
#include <vector>

class Client {
public:
	Client(SOCKET s, Message::Team team) noexcept;
	SOCKET getSocket() const;
	Message::Team getTeam() const;
	~Client() noexcept;
	
private:
	SOCKET m_socket;
	Message::Team m_team;
};

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
  std::vector<Message::Team> m_listeTeam;
  SOCKET m_socket;

  bool m_success=true;
  bool Accept_Client() noexcept;
  bool Receive_Client() noexcept;
  bool Send_to_Client(Message msg) noexcept;
  void Disconnect_Client(SOCKET client, std::string username)noexcept;

  int nbEquipeA=0;
  int nbEquipeB=0;

public:
    ThunderChatServer(std::string servAddress, u_short port) noexcept;
    void OnConnect(connectCallbackType connectCallback) noexcept;
    void OnDisconnect(disconnectCallbackType disconnectCallback) noexcept;
    void Stop() noexcept;
};

} // namespace ThunderChat


