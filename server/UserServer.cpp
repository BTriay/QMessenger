#include "UserServer.h"

UserServer::UserServer(int socket, const std::string& name): User(name), a_socket{socket} {
	int i = pthread_mutex_init(&mtxUser, NULL);
}

UserServer::~UserServer() {
	int i = pthread_mutex_destroy(&mtxUser);
	close(a_socket);
}

void UserServer::addOnlineFriend(UserServer* u) {
	Locker l(&mtxUser);
	a_onlineFriends.push_back(u);
}

void UserServer::getOnlineFriends(std::vector<UserServer*>& users) const {
	users = a_onlineFriends;
}

void UserServer::offlineFriend(UserServer* u) {
	Locker l(&mtxUser);
	std::vector<UserServer*>::iterator it = std::find(a_onlineFriends.begin(), a_onlineFriends.end(), u);
	if ( it != a_onlineFriends.end() )
		a_onlineFriends.erase(it);
	pthread_mutex_unlock(&mtxUser);
}

void UserServer::sendMsg(const char* msg, size_t sz) {
	size_t j = 0;
	Locker l(&mtxUser);
	while (size_t i = send(a_socket, msg + j, sz, MSG_NOSIGNAL)) {
		j += i;
		sz -= i;
	}
}

