#include "Message.hpp"

Message::Message(std::string username, MsgType msg_type, Team team, std::string msg) {
	this->m_username = username;
	this->m_msg_type = msg_type;
	this->m_team = team;
	this->m_msg = msg;

}

nlohmann::json Message::to_JSON() {
	nlohmann::json j = { {"username", this->m_username},
						{"msg_type", this->m_msg_type},
						{"team", this->m_team},
						{"msg", this->m_msg} };
	return j;
}