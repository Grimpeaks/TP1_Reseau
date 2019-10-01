#include <nlohmann/json.hpp>
#include <string>

class Message
{
public:
	enum Team {
		A = 0,
		B = 1
	};
	enum MsgType {
		PARTY = 0,
		TEAM = 1
	};

	Message(std::string username, MsgType msg_type, Team  team, std::string msg);
    Message(nlohmann::json json);
	nlohmann::json to_JSON() const ;
    bool Is_Message_Valide() const;
	std::string get_username() const;
	MsgType get_msg_type() const;
	Team get_team() const;
	std::string get_msg() const;

private:
	std::string m_username;
	MsgType m_msg_type;
	Team m_team;
	std::string m_msg;
    bool m_isValide=true;
};