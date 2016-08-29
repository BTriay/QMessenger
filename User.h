#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "parserConst.h"

class User {

protected:
	std::string a_username;	
	int a_presence; //online, offline, away, do not disturb, invisible

public:
    User(const std::string& name) : a_username{name}, a_presence{ONLINE} {}
	virtual ~User();

	std::string getUsername() const { return a_username; }
  void setPresence(int presence) { a_presence = presence; }
	int getPresence() const { return a_presence; }

};

#endif
