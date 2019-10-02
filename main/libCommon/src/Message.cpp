#include "Message.hpp"

Message::Message(std::string username, MsgType msg_type, Team team, std::string msg) noexcept
{
	if (username.size() <= 0 || username.size() > 24 || msg.size() > 512 || msg.size() <= 0)
	{
		this->m_isValide = false;
		return;
	}
	this->m_username = username;
	this->m_msg_type = msg_type;
	this->m_team = team;
	this->m_msg = msg;
}

Message::Message(nlohmann::json json) noexcept
{
	if ((json.contains("username") && json.contains("msg_type") && json.contains("team") && json.contains("msg")) &&
		!(json.at("username").is_null() && json.at("msg_type").is_null() && json.at("team").is_null() && json.at("msg").is_null()))
	{
		if ((json.at("username").size() > 0 || json.at("username").size() <= 24 || json.at("msg").size() <= 512 || json.at("msg").size() > 0))
		{
			this->m_username = json.at("username");
			this->m_msg_type = json.at("msg_type");
			this->m_team = json.at("team");
			this->m_msg = json.at("msg");
		}
	}
	else
	{
		this->m_isValide = false;
		std::cout << "Message : message recu non valide" << std::endl;
		return;
	}
}

nlohmann::json Message::to_JSON() const noexcept {
	if (this->m_isValide) {
		nlohmann::json j = { {"username", this->m_username},
							{"msg_type", this->m_msg_type},
							{"team", this->m_team},
							{"msg", this->m_msg} };
		return j;
	}
	return nullptr;
}

bool Message::isMessageValide() const noexcept { return this->m_isValide; }
