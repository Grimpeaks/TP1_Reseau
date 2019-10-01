#include "Message.hpp"

Message::Message(std::string username, MsgType msg_type, Team team, std::string msg){
    if (username.size() < 1 || username.size() > 24 || m_msg.size() > 512 || m_msg.size() < 1)
    { this->m_isValide = false;
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
    this->m_username = json["username"];
    this->m_msg_type = json["msg_type"];
    this->m_team = json["team"];
    this->m_msg = json["msg"];
}

const nlohmann::json Message::to_JSON() {
	if (this->m_isValide) {
            nlohmann::json j = {{"username", this->m_username},
                                {"msg_type", this->m_msg_type},
                                {"team", this->m_team},
                                {"msg", this->m_msg}};
            return j;
	}
        return nullptr;
	
}

bool Message::Is_Message_Valide() { return this->m_isValide; }
