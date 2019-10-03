#ifdef _WIN32
#include "network.hpp"
#include "winnetworkconfig.hpp"

namespace network
{
	WORD WinNetworkConfig::s_version = MAKEWORD(2, 2);

	WinNetworkConfig::WinNetworkConfig() noexcept: m_failure(false), m_data()
	{
		m_failure = (WSAStartup(s_version, &m_data) >= 0);
	}

	WinNetworkConfig::~WinNetworkConfig() noexcept
	{
		WSACleanup();
	}

	WinNetworkConfig::WinNetworkConfig(WinNetworkConfig&& other) noexcept:
		m_failure(other.m_failure), m_data(other.m_data)
	{}

	WinNetworkConfig& WinNetworkConfig::operator=(WinNetworkConfig&& rhs) noexcept
	{
		m_failure = rhs.m_failure;
		m_data = rhs.m_data;
		return *this;
	}
}
#endif // _WIN32