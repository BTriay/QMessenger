#include "Db.h"

/***********************PRIVATE METHODS ***********************/
void Db::insertTables() {
	char *errMsg;

														 //salt text not null,
														 //hash text not null,
	const char* clientStmt = "create table CLIENT ( \
														 id integer primary key, \
														 name text not null);";
	if (sqlite3_exec(a_db, clientStmt, NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;

	const char* friendStmt = "create table FRIEND ( \
														 user1 integer not null, \
														 user2 integer not null, \
														 friendConf integer not null, \
														 notFriendConf integer, \
														 foreign key (user1) references client(id) on delete cascade, \
														 foreign key (user2) references client(id) on delete cascade);";
	if (sqlite3_exec(a_db, friendStmt, NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;
}

bool Db::userExists(std::string username) {
	char *errMsg;
	bool t;
	sqlite3_stmt* outStmt;
	std::string stmt = "select name from client where name = \"" + username + "\";";

	if (sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL) != SQLITE_OK)
		std::cout << errMsg;

	if (sqlite3_step(outStmt) == SQLITE_ROW)
		t = true;
	else
		t = false;

	sqlite3_finalize(outStmt);
	return t;
}

int Db::getUserID(std::string username) {
	int i = 0;
	char *errMsg;
	sqlite3_stmt* outStmt;
	std::string stmt = "select id from client where name = \"" + username + "\";";

	if (sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL) != SQLITE_OK)
		std::cout << errMsg;

	if (sqlite3_step(outStmt) == SQLITE_ROW)
		i = sqlite3_column_int(outStmt, 0);
	else
		std::cout << "error getUserID\n";

	sqlite3_finalize(outStmt);
	return i;
}

void Db::getFriendsID(int UID, std::vector<int>& f, bool confirmed) {
	char* errMsg;
	std::string id = std::to_string(UID);
	std::string stmt;
	sqlite3_stmt* outStmt;
	stmt = "select user1, user2 from friend where (user1 = " + id + " or user2 = " + id + ") and friendConf = ";
	if (confirmed)
		stmt += "1;";
	else
		stmt += "0;";

	if (sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL) != SQLITE_OK)
		std::cout << errMsg;

	int u1, u2;
	while (sqlite3_step(outStmt) == SQLITE_ROW) {
		u1 = sqlite3_column_int(outStmt, 0);
		u2 = sqlite3_column_int(outStmt, 1);
		if (u1 != UID)
			f.push_back(u1);
		else
			f.push_back(u2);
	}

	sqlite3_finalize(outStmt);
}

/***********************PUBLIC METHODS ***********************/

Db::Db(std::string dbName, bool createDB) {

	int s = pthread_mutex_init(&mtxDB, NULL);
	if (s != 0)
		std::cout << "db mutex init problem\n";

	std::cout << "db constructor\n";
	if (sqlite3_open(dbName.c_str(), &a_db) != SQLITE_OK)
		std::cout << "error while opening the db\n";
	if (createDB)
		insertTables();

	char *errMsg;
	const char* fKey = "PRAGMA foreign_keys = ON;";
	if (sqlite3_exec(a_db, fKey, NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;
}

Db::~Db() {
	std::cout << "db destructor\n";
	int s = pthread_mutex_destroy(&mtxDB);
	if (s != 0)
		std::cout << "db mutex destroy problem\n";
	sqlite3_close(a_db);
}

void Db::createUser(std::string username) {
	char *errMsg;
	int s;
	std::string stmt = "insert into client (name) values (\"" + username + "\");";
	if (userExists(username)) {
		std::cout << "user already exists, cannot insert\n";
		return;
	}
	s = pthread_mutex_lock(&mtxDB);
	if (s != 0)
		std::cout << "mutex lock pb\n";
	if (sqlite3_exec(a_db, stmt.c_str(), NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;
	s = pthread_mutex_unlock(&mtxDB);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}

void Db::rmUser(std::string username) {
	char *errMsg;
	int s;
	std::string stmt;
	stmt = "delete from client where name=\"" + username + "\";";
	s = pthread_mutex_lock(&mtxDB);
	if (s != 0)
		std::cout << "mutex lock pb\n";
	if (sqlite3_exec(a_db, stmt.c_str(), NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;
	s = pthread_mutex_unlock(&mtxDB);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}

void Db::addFriends(std::string username1, std::string username2, bool confirmed) {
	int u1 = getUserID(username1);
	int u2 = getUserID(username2);
	if (!(u1 && u2))
		return; //users do not exist. Error to handle properly

	char *errMsg;
	int s;
	std::string idU1 = std::to_string(u1);
	std::string idU2 = std::to_string(u2);

	std::string stmt;
	if (!confirmed)
		stmt = "insert into friend (user1, user2, friendConf) values (" + idU1 + "," + idU2 + "," + "0);";
	else
		stmt = "update friend set friendConf = 1 where user1 = " + idU1 + " and user2 = " + idU2 + ";";

	s = pthread_mutex_lock(&mtxDB);
	if (s != 0)
		std::cout << "mutex lock pb\n";
	if (sqlite3_exec(a_db, stmt.c_str(), NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;
	s = pthread_mutex_unlock(&mtxDB);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}

void Db::getFriendsName(std::string username, std::vector<std::string>& fList, bool confirmed) {
	int u = getUserID(username);
	if (!u)
		return; //user does not exist. Error to handle properly

	std::vector<int> friendsUID;
	std::vector<int>::iterator it;
	getFriendsID(u, friendsUID, confirmed);

	char* errMsg;
	int s;
	std::string stmt;
	sqlite3_stmt* outStmt;
	stmt = "select name from client where id = ?;";
	if (sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL) != SQLITE_OK)
		std::cout << errMsg;

	for (it = friendsUID.begin(); it != friendsUID.end(); it++) {
		sqlite3_bind_int(outStmt, 1, *it);
		if (sqlite3_step(outStmt) == SQLITE_ROW)
			fList.push_back(std::string(std::string(reinterpret_cast<const char*>(sqlite3_column_text(outStmt, 0)))));
		sqlite3_reset(outStmt);
	}

	sqlite3_finalize(outStmt);
}

void Db::rmFriends(std::string username1, std::string username2) {
	int u1 = getUserID(username1);
	int u2 = getUserID(username2);
	if (!(u1 && u2))
		return; //users do not exist. Error to handle properly

	char *errMsg;
	int s;
	std::string idU1 = std::to_string(u1);
	std::string idU2 = std::to_string(u2);

	std::string stmt1;
	stmt1 = "delete from friend where user1=" + idU1 + " and user2= " + idU2 + ";";
	std::string stmt2;
	stmt2 = "delete from friend where user1=" + idU2 + " and user2= " + idU1 + ";";

	s = pthread_mutex_lock(&mtxDB);
	if (s != 0)
		std::cout << "mutex lock pb\n";
//one of these two statements will throw an error
	if (sqlite3_exec(a_db, stmt1.c_str(), NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;
	if (sqlite3_exec(a_db, stmt2.c_str(), NULL, NULL, &errMsg) != SQLITE_OK)
		std::cout << errMsg;
	s = pthread_mutex_unlock(&mtxDB);
	if (s != 0)
		std::cout << "mutex unlock pb\n";
}
