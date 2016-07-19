#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

//enum class presence {"online", "offline", "away", "do_not_disturb"};

class User {

protected:
	std::string a_username;	
//	presence a_presence; //online, offline, away, do not disturb
	std::string a_status;
	std::string a_nickname;

public:
	User(std::string name) { a_username = name; }
	virtual ~User();

	void setStatus(std::string status) { a_status = status; }
	void setNickname(std::string nickname) { a_nickname = nickname; }
	std::string getUsername() const { return a_username; }
	std::string getNickname() const { return a_nickname; }
	std::string getStatus() const { return a_status; }

};

#endif
