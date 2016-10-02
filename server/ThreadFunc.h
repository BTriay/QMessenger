#ifndef THREADFUNC_H
#define THREADFUNC_H

#include <vector>
#include <string>
#include <fstream>
#include <pthread.h>
#include <sys/epoll.h>
#include <cstdint>
#include <pthread.h>

#include "../parser.h"
#include "Db.h"
#include "MatrixServer.h"
#include "Bundle.h"
#include "Locker.h"

#define 	USER_HANGUP	0

class ThreadFunc {
	Db* a_db;
	MatrixServer* a_matrix;
	int a_epfd;

	void inviteToRoom(std::vector<std::string>& tokens);
	void beMyFriend(int socket, const std::string& token);
	void weAreFriends(int socket, const std::vector<std::string>& tokens);
	void weAreNotFriends(int socket, const std::vector<std::string>& tokens);
	void friendsList(int socket);
	void newConnect(int socket, std::vector<std::string>& tokens, int identifier);
	void udpateOnlineFriends(int socket);
	void deleteMe(int socket);
	void confirmPotentialFriends(int socket);
	void confirmPotentialFriends(std::string username);
	std::string getUsername(int socket) const;

public:
	ThreadFunc(Bundle* bundle);
	~ThreadFunc() {} //destructor should NOT destroy the matrix / db !
	void operator()(int identifier, int socket, std::vector<std::string>& tokens);
	int epfd() const { return a_epfd; }

};

void* threadStart(void *arg);
void rearmFD(int epfd, int socket);

#endif
