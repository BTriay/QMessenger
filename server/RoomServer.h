#include "../Room.h"

class RoomServer : public Room {
	std::vector<int> a_users; //identified by their sockets

public:
	void addUser(int socket);
	void rmUser(int socket);
	void sendMsg(std::string& msg);

};
