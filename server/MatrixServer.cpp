#include "MatrixServer.h"

MatrixServer::MatrixServer() {
	a_nextAvailableRoom = 0;
	int s = pthread_mutex_init(&mtxMatrix, NULL);
	if (s != 0)
		std::cout << "matrix mutex init problem\n";
	std::cout << "matrix constructor\n";
}

MatrixServer::~MatrixServer() {
	int s = pthread_mutex_destroy(&mtxMatrix);
	if (s != 0)
		std::cout << "matrix mutex destroy problem\n";
	std::cout << "matrix destructor\n";
}

void MatrixServer::createUser(int const socket, std::string const name) {
	int s;
	s = pthread_mutex_lock(&mtxMatrix);
	if (s != 0)
		std::cout << "mutex lock pb\n";

	std::cout << "matrix create user\n";
	a_socketUser.insert(std::make_pair(socket, new UserServer(socket, name)));
	a_nameUser[name] = socket;

	s = pthread_mutex_unlock(&mtxMatrix);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}

void MatrixServer::addOnlineFriends(int const socket, std::vector<std::string>& friends) {
	std::cout << "matrix add friend\n";
	UserServer* u = a_socketUser[socket];
	std::vector<std::string>::iterator it;
	for (it = friends.begin(); it != friends.end(); it ++)
		u->addOnlineFriend(a_nameUser[*it]);
}

void MatrixServer::rmUser(int socket) {
	std::vector<int> f;
	a_socketUser[socket]->getOnlineFriend(f);
	std::vector<int>::iterator it;
	for (it = f.begin(); it != f.end(); it++)
		a_socketUser[*it]->offlineFriend(socket);

	int s;
	s = pthread_mutex_lock(&mtxMatrix);
	if (s != 0)
		std::cout << "mutex lock pb\n";

	a_nameUser.erase(a_socketUser[socket]->getUsername());
	delete a_socketUser[socket];
	a_socketUser.erase(socket);

	s = pthread_mutex_unlock(&mtxMatrix);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}
