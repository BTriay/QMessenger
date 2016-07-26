#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

//message type identifier
#define ROOM_MSG						1
#define KEEPALIVE						2
#define PRESENCE_UPDATE			3
	#define ONLINE						30
	#define AWAY							31
	#define DO_NOT_DISTURB		32
	#define OFFLINE						33
#define NEW_ROOM						4
#define INVITE_USER_TO_ROOM	5
#define LEAVE_THE_ROOM			6
#define BE_MY_FRIEND				7
#define I_AM_YOUR_FRIEND		8
#define WE_ARE_NOT_FRIENDS	9
#define HELLO								10
#define NEW_USER						11
	#define USERNAME_OK				111
	#define USERNAME_TAKEN		112
#define DELETE_ME						12

//config file
#define CONFIG_THREADS			"threadspool"
#define CONFIG_DB						"db"
#define CONFIG_PORT					"port"

bool configFileReader(std::string const & filename, std::vector<std::string>& tokens);
int msgParser(std::string const & msg, std::vector<std::string>& tokens);
char* msgWriter(std::string& msg, std::vector<std::string>& tokens, int identifier, size_t* size);

#endif
