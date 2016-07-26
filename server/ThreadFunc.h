#ifndef THREADFUNC_H
#define THREADFUNC_H

#include <vector>
#include <string>
#include <fstream>
#include <pthread.h>
#include <sys/epoll.h>

#include "../parser.h"
#include "Db.h"
#include "MatrixServer.h"
#include "Bundle.h"

#define 	RM_USER		0

class ThreadFunc {
	Db* a_db;
	MatrixServer* a_matrix;
	int a_epfd;

	void rmUser(int fd);
	void roomMsg(int fd, std::vector<std::string>& msgTokens);
	void presenceUpdate(int fd, std::vector<std::string>& msgTokens);
	void newRoom(std::vector<std::string>& msgTokens);
	void inviteToRoom(std::vector<std::string>& msgTokens);
	void leaveRoom(int fd, std::vector<std::string>& msgTokens);
	void beMyFriend(int fd, std::vector<std::string>& msgTokens);
	void weAreFriends(int fd, std::vector<std::string>& msgTokens);
	void weAreNotFriends(int fd, std::vector<std::string>& msgTokens);
	void hello(int fd, std::vector<std::string>& msgTokens);
	void newUser(int fd, std::vector<std::string>& msgTokens);
	void deleteMe(int fd);

public:
	ThreadFunc(Bundle* bundle);
	~ThreadFunc() {} //destructor should NOT destroy the matrix / db !
	int operator()(int parserDef, int fd, std::vector<std::string>& msgTokens);
	int epfd() const { return a_epfd; }

};

void* threadStart(void *arg);
void getSocketMsg(std::string& msg, int fd);
void rearmFD(int epfd, int fd);
void emptySocket(int fd);

#endif
