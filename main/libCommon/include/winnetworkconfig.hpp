#pragma once

namespace network
{
	// Windows Network API configuration class
	class WinNetworkConfig
	{
	public:
		WinNetworkConfig() noexcept;
		~WinNetworkConfig() noexcept;
		WinNetworkConfig(WinNetworkConfig&& other) noexcept;
		WinNetworkConfig& operator=(WinNetworkConfig&& rhs) noexcept;

		WinNetworkConfig(const WinNetworkConfig& other) = delete;
		WinNetworkConfig& operator=(const WinNetworkConfig& rhs) = delete;

	private:
		bool m_failure; // Indicate if the API has been initialized successful or not
		WSADATA m_data; // Data get by init of the API

		static WORD s_version; // Requested version of the API
	};
}