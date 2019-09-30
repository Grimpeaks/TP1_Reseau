#include "network.hpp"
#include "winnetworkconfig.hpp"

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

  bool m_success=true;

public:
    ThunderChatServer(std::string servAddress, u_short port);

    void OnConnect(connectCallbackType connectCallback);

    void OnDisconnect(disconnectCallbackType disconnectCallback);

    void Stop();
};
} // namespace ThunderChat