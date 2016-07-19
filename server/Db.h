#ifndef DB_H
#define DB_H

#include <string>
#include <vector>
#include <iterator>
#include "../sqlite-amalgamation-3130000/sqlite3.h"
#include <pthread.h>

#include <iostream>

class Db {
	sqlite3* a_db;
	pthread_mutex_t mtxDB;

	void insertTables();
	bool userExists(std::string username);
	int getUserID(std::string username);
	void getFriendsID(int UID, std::vector<int>& f, bool confirmed);

public:
	Db(std::string dbName, bool createDB);
	~Db();

//	void getUserHash(std::string username, std::string& salt, std::string& hash);
//	void setUserHash(std::string username, std::string& salt, std::string& hash);
	void createUser(std::string username/*, std::string& salt, std::string& hash*/);
	void rmUser(std::string username);
	void getFriendsName(std::string username, std::vector<std::string>& fList, bool confirmed);
	void addFriends(std::string username1, std::string username2, bool confirmed);
	void rmFriends(std::string username1, std::string username2);
};

#endif
