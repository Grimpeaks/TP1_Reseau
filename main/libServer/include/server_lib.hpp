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
	Client(SOCKET s, Message::Team team);
	SOCKET getSocket() const;
	Message::Team getTeam() const;
	~Client();
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
  std::vector<Client> m_listeClient;
  SOCKET m_socket;

  bool m_success=true;
  bool Accept_Client();
  bool Recieve_Client();
  bool Send_to_Client();

  int nbEquipeA=0;
  int nbEquipeB=0;

public:
    ThunderChatServer(std::string servAddress, u_short port);

    void OnConnect(connectCallbackType connectCallback);

    void OnDisconnect(disconnectCallbackType disconnectCallback);

    void Stop();
};

} // namespace ThunderChat


