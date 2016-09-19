#ifndef ROOMSERVER_H
#define ROOMSERVER_H

#include <vector>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <algorithm>
#include "../parser.h"
#include "UserServer.h"

class RoomServer {
	int a_roomNo;
	std::vector<UserServer*> a_users;
	pthread_mutex_t mtxRoom;

public:
	RoomServer(int roomNo) : a_roomNo{roomNo} { pthread_mutex_init(&mtxRoom, NULL); }
	~RoomServer() { pthread_mutex_destroy(&mtxRoom); }
	void addUser(UserServer* u);
	void addUser(std::vector<UserServer*> users);
	int rmUser(UserServer* u);
	void sendMsg(const std::vector<std::string>& tokens);
	void getUsersName(std::vector<std::string>& users) const;
	void roomUsers(std::vector<std::string>& tokens);
	void sendNewJoinerName(UserServer* u);
	void sendUsersName(UserServer* u);
	void sendUsersName();
};

#endif
