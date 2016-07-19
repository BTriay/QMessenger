#ifndef MATRIXSERVER_H
#define MATRIXSERVER_H

#include <string>
#include <vector>
#include <map>
#include <pthread.h>

#include "UserServer.h"
#include "RoomServer.h"

#include <iostream>

class MatrixServer {
	std::map<int, UserServer*> a_socketUser; //int is the socket of the user
	std::map<std::string, int> a_nameUser; //string is username, int is socket
	std::map<int, RoomServer*> a_rooms;
	int a_nextAvailableRoom;

	pthread_mutex_t mtxMatrix;

public:
	MatrixServer();
	~MatrixServer();

	void createUser(int const socket, std::string const name);
	void addOnlineFriends(int const socket, std::vector<std::string>& friends);
	void rmUser(int socket);

	int createRoom(std::vector<int>& users); //returns the room no
	void addUserToRoom(int roomNo, int socket);
	void rmUserFromRoom(int roomNo, int socket); //if last user leaves room, delete the room
	void sendMsg(int roomNo, std::string& msg);

};

#endif
