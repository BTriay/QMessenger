#ifndef DB_H
#define DB_H

#include <string>
#include <vector>
#include <iterator>
#include "../sqlite-amalgamation/sqlite3.h"
//#include <pthread.h>
/*
NOTE: sqlite is supposed to be thread-safe:
https://www.sqlite.org/threadsafe.html
so the mutexes are disabled and the programme compiled using the -DSQLITE_THREADSAFE option
*/

class Db {
	sqlite3* a_db;
//	pthread_mutex_t mtxDB;

	void insertTables();
	int getUserID(std::string const & username);
	void getFriendsID(int UID, std::vector<int>& f, bool confirmed);
	bool friendsPairInDB(const std::string& username1, const std::string& username2);

public:
	Db(std::string dbName, bool createDB);
	~Db();

	void getCredentials(const std::string& username, std::string& hash, std::string& salt);
	bool createUser(const std::string& username, const std::string& hash, const std::string & salt);
	bool rmUser(const std::string& username);
	void getFriendsName(const std::string& username, std::vector<std::string>& fList, bool confirmed);
	bool addFriendsPair(const std::string& username1, bool confirmedUser1, const std::string& username2, bool confirmedUser2);
	bool rmFriendsPair(const std::string& username1, const std::string& username2);
};

#endif


/*
		void getCredentials(std::string const & username, std::string& hash, std::string& salt);
Function user to provide username. Function fills in hash and salt. If user does not exist, hash remains empty.

		bool createUser(std::string const & username, std::string const & hash, std::string const & salt);
Function user to provide username, hash and salt. Returns false if the username already exists, true otherwise.

		bool rmUser(std::string const & username);
Function user to provide username. Returns false on error, true otherwise.

		void getFriendsName(std::string const & username, std::vector<std::string>& fList, bool confirmed);
Function user to provide username and the confirmed bool. Function fills in the fList. The confirmed bool indicates whether friendship has been confirmed or not. If false, then the requestee has not yet responded.

		bool addFriendsPair(std::string const & username1, std::string const & username2, bool confirmed);

		bool rmFriendsPair(std::string const & username1, std::string const & username2);
*/
