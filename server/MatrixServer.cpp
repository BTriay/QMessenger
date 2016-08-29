#include "MatrixServer.h"

MatrixServer::MatrixServer() {
	a_nextAvailableRoom = 0;
	int s = pthread_mutex_init(&mtxMatrix, NULL);
#ifdef TEST_COUT
	if (s != 0)
		std::cout << "matrix mutex init problem\n";
#endif
}

MatrixServer::~MatrixServer() {
	int s = pthread_mutex_destroy(&mtxMatrix);
#ifdef TEST_COUT
	if (s != 0)
		std::cout << "matrix mutex destroy problem\n";
#endif
}

void MatrixServer::createUser(int socket, const std::string& name) {
	pthread_mutex_lock(&mtxMatrix);
	a_socketUser.insert(std::make_pair(socket, new UserServer(socket, name)));
	a_nameUser[name] = a_socketUser[socket];
	pthread_mutex_unlock(&mtxMatrix);
}

void MatrixServer::rmUser(int socket) {
#ifdef TEST_COUT
std::cout << "user hangup\n";
#endif
	UserServer* u;
	try {
		u = a_socketUser.at(socket);
	}
	catch (const std::out_of_range) {
		return;
	}
	u->setPresence(OFFLINE);
	sendUserPresence(socket);

	std::vector<UserServer*> userFriends;
	u->getOnlineFriends(userFriends);
	for (std::vector<UserServer*>::iterator it = userFriends.begin(); it != userFriends.end(); it++)
		(*it)->offlineFriend(u);
	for (std::map<int, RoomServer*>::iterator it = a_rooms.begin(); it != a_rooms.end(); it++)
		it->second->rmUser(u);

	pthread_mutex_lock(&mtxMatrix);
	a_nameUser.erase(u->getUsername());
	delete(u);
	a_socketUser.erase(socket);
	pthread_mutex_unlock(&mtxMatrix);
}

bool MatrixServer::userLogged(int socket) const {
	try {
		UserServer* u = a_socketUser.at(socket);
		return true;
	}
	catch (const std::out_of_range) {
		return false;
	}
}

void MatrixServer::addOnlineFriends(int const socket, std::vector<std::string>& friends) {
	UserServer* u = a_socketUser[socket];
	for (std::vector<std::string>::iterator it = friends.begin(); it != friends.end(); it ++)
		if (a_nameUser.find(*it) != a_nameUser.end()) {
			u->addOnlineFriend(a_nameUser[*it]);
			a_nameUser[*it]->addOnlineFriend(u);
		}
}

std::string MatrixServer::getUsername(int socket) {
	return a_socketUser[socket]->getUsername();
}

void MatrixServer::updateUserPresence(int socket, int presence) {
	a_socketUser[socket]->setPresence(presence);
	sendUserPresence(socket);
}

void MatrixServer::sendUserPresence(int socket) {
	UserServer* u = a_socketUser[socket];
	std::vector<std::string> tokens;
	tokens.push_back(u->getUsername());
	int i = u->getPresence();
	if (i == INVISIBLE)
		i = OFFLINE;
	tokens.push_back(std::to_string(i));
	MsgWriter mw(tokens, PRESENCE_UPDATE);

	std::vector<UserServer*> userFriends;
	u->getOnlineFriends(userFriends);
	for (std::vector<UserServer*>::iterator it = userFriends.begin(); it != userFriends.end(); it++)
		(*it)->sendMsg(mw.getMsg(), mw.getSz());
}

void MatrixServer::offlineUser(int socket, const std::vector<std::string>& friends) {
//user is not really offline, it's just a lie to her ex friends
	std::vector<std::string> tokens;
	tokens.push_back(getUsername(socket));
	tokens.push_back(std::to_string(OFFLINE));
	MsgWriter mw(tokens, PRESENCE_UPDATE);

	for (std::vector<std::string>::const_iterator it = friends.begin(); it != friends.end(); it++) {
		UserServer* f;
		try {
			f = a_nameUser.at(*it);
			f->sendMsg(mw.getMsg(), mw.getSz());
		}
		catch (const std::out_of_range) {}
	}

	tokens.clear();
	for (std::vector<std::string>::const_iterator it = friends.begin(); it != friends.end(); it++) {
		tokens.push_back(*it);
		tokens.push_back(std::to_string(OFFLINE));
	}
	MsgWriter mwu(tokens, PRESENCE_UPDATE);
	a_socketUser[socket]->sendMsg(mwu.getMsg(), mwu.getSz());
}

void MatrixServer::recFriendsPresence(int socket) {
	UserServer* u = a_socketUser[socket];
	std::vector<UserServer*> userFriends;
	u->getOnlineFriends(userFriends);

	std::vector<std::string> tokens;
	for (std::vector<UserServer*>::iterator it = userFriends.begin(); it != userFriends.end(); it++) {
		tokens.push_back((*it)->getUsername());
		tokens.push_back(std::to_string((*it)->getPresence()));
	}
	MsgWriter mw(tokens, PRESENCE_UPDATE);
	u->sendMsg(mw.getMsg(), mw.getSz());
}

void MatrixServer::newFriendsRequests(int socket, std::vector<std::string>& friends) {
	friends.insert(friends.begin(), std::to_string(NEW_REQUEST));
	MsgWriter mw(friends, BE_MY_FRIEND);
	a_socketUser[socket]->sendMsg(mw.getMsg(), mw.getSz());
}

void MatrixServer::newFriendsRequests(std::string username, std::vector<std::string>& friends) {
	try {
		a_nameUser.at(username);
	}
	catch (const std::out_of_range) {
		return;
	}
	friends.insert(friends.begin(), std::to_string(NEW_REQUEST));
	MsgWriter mw(friends, BE_MY_FRIEND);
	a_nameUser[username]->sendMsg(mw.getMsg(), mw.getSz());
}

void MatrixServer::unknownFriendName(int socket, const std::string& token) {
	std::vector<std::string> request;
	request.push_back(std::to_string(UNKNOWN_USER));
	request.push_back(token);
	MsgWriter mw(request, BE_MY_FRIEND);
	a_socketUser[socket]->sendMsg(mw.getMsg(), mw.getSz());
}

void MatrixServer::createRoom(int socket, std::vector<std::string>& tokens) {
	pthread_mutex_lock(&mtxMatrix);
	int roomNo = a_nextAvailableRoom;
	a_rooms.insert(std::make_pair(roomNo, new RoomServer(roomNo)));
	++a_nextAvailableRoom;
	pthread_mutex_unlock(&mtxMatrix);

	tokens.push_back(a_socketUser[socket]->getUsername());
	addUsersToRoom(roomNo, tokens, true);
}

void MatrixServer::addUsersToRoom(int roomNo, std::vector<std::string>& tokens, bool newRoom) {
	RoomServer* r;
	try {
		r = a_rooms.at(roomNo);
	}
	catch (const std::out_of_range) {
		return;
	}

	UserServer* u;
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		try {
			u = a_nameUser.at(*it);
		}
		catch (const std::out_of_range) {
			u = nullptr;
		}
		if (u != nullptr) {
			r->addUser(u);
			if (!newRoom)
				r->sendNewJoinerName(u);
		}
	}
	if (newRoom)
		r->sendUsersName();
}

void MatrixServer::rmUserFromRoom(int socket, int roomNo) {
	RoomServer* r;
	try {
		r = a_rooms.at(roomNo);
	}
	catch (const std::out_of_range) {
		return;
	}
	int i = r->rmUser(a_socketUser[socket]);
	if (i <= 1) {
		pthread_mutex_lock(&mtxMatrix);
		delete r;
		a_rooms.erase(roomNo);
		pthread_mutex_unlock(&mtxMatrix);
	}
}

void MatrixServer::msgToRoom(int socket, std::vector<std::string>& tokens) {
	RoomServer* r;
	try {
		r = a_rooms[stoi(tokens[0])];
	}
	catch (const std::out_of_range) {
		return;
	}
	switch (stoi(tokens[1])) {
		case (NEW_MSG):
			tokens.insert(tokens.begin()+2, a_socketUser[socket]->getUsername());
			r->sendMsg(tokens);
			break;
		case (ROOM_USERS):
			r->getUsersName(tokens);
			MsgWriter mw(tokens, ROOM_MSG);
			a_socketUser[socket]->sendMsg(mw.getMsg(), mw.getSz());
	}
}

void MatrixServer::sendConfirmedFriends(int socket, const std::vector<std::string>& tokens) {
	UserServer* u = a_socketUser[socket];
	MsgWriter mw(tokens, FRIENDS_LIST);
	u->sendMsg(mw.getMsg(), mw.getSz());
}
