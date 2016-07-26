#include "ThreadFunc.h"

ThreadFunc::ThreadFunc(Bundle* bundle) {
	a_db = bundle->db;
	MatrixServer* a_matrix = bundle->matrix;
	a_epfd = bundle->epfd;
}

int ThreadFunc::operator() (int parserDef, int fd, std::vector<std::string>& msgTokens) {
	switch (parserDef) {
		case RM_USER:
			rmUser(fd);
			break;
		case ROOM_MSG:
			roomMsg(fd, msgTokens);
			break;
		case PRESENCE_UPDATE:
			presenceUpdate(fd, msgTokens);
			break;
		case NEW_ROOM:
			newRoom(msgTokens);
			break;
		case INVITE_USER_TO_ROOM:
			inviteToRoom(msgTokens);
			break;
		case LEAVE_THE_ROOM:
			leaveRoom(fd, msgTokens);
			break;
		case BE_MY_FRIEND:
			beMyFriend(fd, msgTokens);
			break;
		case I_AM_YOUR_FRIEND:
			weAreFriends(fd, msgTokens);
			break;
		case WE_ARE_NOT_FRIENDS:
			weAreNotFriends(fd, msgTokens);
			break;
		case HELLO:
			hello(fd, msgTokens);
			break;
		case NEW_USER:
			newUser(fd, msgTokens);
			break;
		case DELETE_ME:
			deleteMe(fd);
			break;
		}
}

void ThreadFunc::rmUser(int fd) {
std::cout << "rmUser\n";
}

void ThreadFunc::roomMsg(int fd, std::vector<std::string>& msgTokens) {
std::cout << "roomMsg\n";
}

void ThreadFunc::presenceUpdate(int fd, std::vector<std::string>& msgTokens) {
std::cout << "presenceUpdate\n";
}

void ThreadFunc::newRoom(std::vector<std::string>& msgTokens) {
std::cout << "newRoom\n";
}

void ThreadFunc::inviteToRoom(std::vector<std::string>& msgTokens) {
std::cout << "inviteToRoom\n";
}

void ThreadFunc::leaveRoom(int fd, std::vector<std::string>& msgTokens) {
std::cout << "leaveRoom\n";
}

void ThreadFunc::beMyFriend(int fd, std::vector<std::string>& msgTokens) {
std::cout << "beMyFriend\n";
}

void ThreadFunc::weAreFriends(int fd, std::vector<std::string>& msgTokens) {
std::cout << "weAreFriends\n";
}

void ThreadFunc::weAreNotFriends(int fd, std::vector<std::string>& msgTokens) {
std::cout << "weAreNotFriends\n";
}

void ThreadFunc::hello(int fd, std::vector<std::string>& msgTokens) {
std::cout << "hello\n";
//check that the user exists
//check that the password is correct
}

void ThreadFunc::newUser(int fd, std::vector<std::string>& msgTokens) {
std::cout << "newUser\n";
}

void ThreadFunc::deleteMe(int fd) {
std::cout << "deleteMe\n";
}
