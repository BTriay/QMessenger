#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <string>

class Room {
protected:
	int a_roomNo;
	
public:
	virtual void sendMsg(std::string& msg) = 0;
};

#endif
