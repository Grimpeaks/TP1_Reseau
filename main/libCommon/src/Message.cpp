#include "Message.hpp"

Message::Message(std::string username, MsgType msg_type, Team team, std::string msg)
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

Message::Message(nlohmann::json json)
{
	if (json["username"].is_null() || json["msg_type"].is_null() || json["team"].is_null() ||
		json["msg"].is_null())
	{
		this->m_isValide = false;
		return;

	}
    this->m_username = json.at("username");
    this->m_msg_type = json.at("msg_type");
    this->m_team = json.at("team");
    this->m_msg = json.at("msg");
}

nlohmann::json Message::to_JSON() const {
	if (this->m_isValide) {
            nlohmann::json j = {{"username", this->m_username},
                                {"msg_type", this->m_msg_type},
                                {"team", this->m_team},
                                {"msg", this->m_msg}};
            return j;
	}
        return nullptr;
}

bool Message::Is_Message_Valide() const
{
	return this->m_isValide;
}

std::string Message::get_username() const
{
	return this->m_username;
}

Message::MsgType Message::get_msg_type() const
{
	return this->m_msg_type;
}

Message::Team Message::get_team() const
{
	return this->m_team;
}

std::string Message::get_msg() const
{
	return this->m_msg;
}
