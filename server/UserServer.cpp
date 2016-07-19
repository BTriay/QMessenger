#include "UserServer.h"

UserServer::UserServer(int socket, std::string name): User(name), a_socket{socket} {
	int s = pthread_mutex_init(&mtxUser, NULL);
	if (s != 0)
		std::cout << "serverUser mutex init problem\n";

	std::cout << "serverUser constructor\n";
}

UserServer::~UserServer() {
//	User::~User();
	int s = pthread_mutex_destroy(&mtxUser);
	if (s != 0)
		std::cout << "serverUser mutex destroy problem\n";

	std::cout << "serverUser destructor for " << this->getUsername() << std::endl;
	std::cout << "closing socket\n";
}

void UserServer::addOnlineFriend(int socket) {
	int s;
	s = pthread_mutex_lock(&mtxUser);
	if (s != 0)
		std::cout << "mutex lock pb\n";

	a_onlineFriends.push_back(socket);

	s = pthread_mutex_unlock(&mtxUser);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}

void UserServer::getOnlineFriend(std::vector<int>& sockets) const {
	sockets = a_onlineFriends;
}

void UserServer::offlineFriend(int socket) {
	int s;
	std::vector<int>::iterator it;
	bool found = false;

	s = pthread_mutex_lock(&mtxUser);
	if (s != 0)
		std::cout << "mutex lock pb\n";

	it = a_onlineFriends.begin();
	while (it != a_onlineFriends.end() && !found) {
		if (*it == socket) {
			a_onlineFriends.erase(it);
			found = true;
		}
		it++;
	}

	s = pthread_mutex_unlock(&mtxUser);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}

void UserServer::sendMsg(std::string msg) {
	write(a_socket, msg.c_str(), msg.size());
}

