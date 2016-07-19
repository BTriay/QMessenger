#ifndef SERVER_USER_H
#define SERVER_USER_H

#include "../User.h"
#include <pthread.h>
#include <unistd.h>

#include <iostream>

class UserServer : public User {
	int a_socket;
	std::vector<int> a_onlineFriends;
	pthread_mutex_t mtxUser;

public:
	UserServer(int socket, std::string name);
	virtual ~UserServer();

	int getSocket() const { return a_socket; }
	void addOnlineFriend(int socket);
	void getOnlineFriend(std::vector<int>& sockets) const;
	void offlineFriend(int socket);
	void sendMsg(std::string msg);

};

#endif
