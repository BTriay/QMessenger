#ifndef SERVER_USER_H
#define SERVER_USER_H

#include "../User.h"
#include <pthread.h>
#include <unistd.h>
#include <algorithm>

#include <iostream>

class UserServer : public User {
	int a_socket;
	std::vector<UserServer*> a_onlineFriends;
	pthread_mutex_t mtxUser;

public:
	UserServer(int socket, const std::string& name);
	virtual ~UserServer();

	void addOnlineFriend(UserServer* u);
	void getOnlineFriends(std::vector<UserServer*>& users) const;
	void offlineFriend(UserServer* u);
	void sendMsg(const char* msg, size_t sz);
};

#endif
