#ifndef MATRIXSERVER_H
#define MATRIXSERVER_H

#include <string>
#include <vector>
#include <map>
#include <pthread.h>

#include "../parser.h"
#include "UserServer.h"
#include "RoomServer.h"
#include "Locker.h"

#ifdef TEST_COUT
#include <iostream>
#endif

class MatrixServer {
	std::map<int, UserServer*> a_socketUser; //int is the socket of the user
	std::map<std::string, UserServer*> a_nameUser; //string is username
	std::map<int, RoomServer*> a_rooms;
	int a_nextAvailableRoom;

	pthread_mutex_t mtxMatrix;

public:
	MatrixServer();
	~MatrixServer();

	void createUser(int socket, const std::string& name);
	void rmUser(int socket);
	bool userCreated(int socket) const;
	bool userLogged(const std::string& username) const;
	void addOnlineFriends(int socket, std::vector<std::string>& friends);
	std::string getUsername(int socket);

	void updateUserPresence(int socket, int presence);
	void sendUserPresence(int socket); //to all her friends
	void offlineUser(int socket, const std::vector<std::string>& friends); //when people are not friends anymore, tells them the socket user is offline
	void recFriendsPresence(int socket); //of all her friends to her
	void newFriendsRequests(int socket, std::vector<std::string>& friends);
	void newFriendsRequests(std::string username, std::vector<std::string>& friends);
	void unknownFriendName(int socket, const std::string& token);

	void createRoom(int socket, std::vector<std::string>& tokens);
	void addUsersToRoom(int roomNo, std::vector<std::string>& tokens, bool newRoom);
	void rmUserFromRoom(int socket, int roomNo); //if last user leaves room, delete the room
	void msgToRoom(int socket, std::vector<std::string>& tokens);
	void sendConfirmedFriends(int socket, const std::vector<std::string>& tokens);
};

#endif
