#include "RoomServer.h"

void RoomServer::getUsersName(std::vector<std::string>& users) const {
	for (std::vector<UserServer*>::const_iterator it = a_users.begin(); it != a_users.end(); it++)
		users.push_back((*it)->getUsername());
}

void RoomServer::addUser(UserServer* u) {
	pthread_mutex_lock(&mtxRoom);
	a_users.push_back(u);
	pthread_mutex_unlock(&mtxRoom);

	std::vector<std::string> tokens;
	tokens.push_back(std::to_string(a_roomNo));
	MsgWriter mw(tokens, JOIN_THE_ROOM);
	u->sendMsg(mw.getMsg(), mw.getSz());
}

void RoomServer::addUser(std::vector<UserServer*> users) {
	for (std::vector<UserServer*>::iterator it = users.begin(); it != users.end(); it++)
		addUser(*it);
}

int RoomServer::rmUser(UserServer* u) {
	pthread_mutex_lock(&mtxRoom);
	std::vector<UserServer*>::iterator it = std::find(a_users.begin(), a_users.end(), u);
	if ( it != a_users.end() )
		a_users.erase(it);
	pthread_mutex_unlock(&mtxRoom);

	std::vector<std::string> tokens;
	tokens.push_back(std::to_string(a_roomNo));
	tokens.push_back(std::to_string(LEAVER));
	tokens.push_back(u->getUsername());
	sendMsg(tokens);

	return a_users.size();
}

bool RoomServer::userInRoom(const UserServer* u) {
	if (std::find(a_users.begin(), a_users.end(), u) == a_users.end())
		return false;
	return true;
/*
	std::string name = u->getUsername();
	for (std::vector<UserServer*>::iterator it = a_users.begin(), it != a_users.end(), it++)
		if (name == it->getUsername())
			return true;
	return false;
*/
}

void RoomServer::sendMsg(const std::vector<std::string>& tokens) {
	MsgWriter mw(tokens, ROOM_MSG);
	for (std::vector<UserServer*>::iterator it = a_users.begin(); it != a_users.end(); it++)
		(*it)->sendMsg(mw.getMsg(), mw.getSz());
}

void RoomServer::roomUsers(std::vector<std::string>& tokens) {
	tokens.push_back(std::to_string(a_roomNo));
	tokens.push_back(std::to_string(ROOM_USERS));
	std::vector<std::string> users;
	getUsersName(users);
	std::copy(users.begin(), users.end(), std::back_insert_iterator< std::vector<std::string> > (tokens));
}

void RoomServer::sendUsersName(UserServer* u) {
	std::vector<std::string> tokens;
	roomUsers(tokens);
	MsgWriter mw(tokens, ROOM_MSG);
	u->sendMsg(mw.getMsg(), mw.getSz());
}

void RoomServer::sendUsersName() {
	std::vector<std::string> tokens;
	roomUsers(tokens);
	this->sendMsg(tokens);
}

void RoomServer::sendNewJoinerName(UserServer* u) {
	std::vector<std::string> tokens;
	tokens.push_back(std::to_string(a_roomNo));
	tokens.push_back(std::to_string(NEW_JOINER));
	tokens.push_back(u->getUsername());
	this->sendMsg(tokens);
	this->sendUsersName(u);
}
