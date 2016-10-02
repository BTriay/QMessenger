#include "Db.h"

/***********************PRIVATE METHODS ***********************/
void Db::insertTables() {
	std::string clientStmt{"create table CLIENT ( \
														 id integer primary key, \
														 name text not null, \
														 hash text not null, \
														 salt text);"};
//salt will be null until cryptographic functions are in... and "hash" will actually be the pwd in clear text
	sqlite3_exec(a_db, clientStmt.c_str(), NULL, NULL, NULL);

	std::string friendStmt{"create table FRIEND ( \
														 user1 integer not null, \
														 friendConfUser1 integer not null, \
														 user2 integer not null, \
														 friendConfUser2 integer not null, \
														 foreign key (user1) references client(id) on delete cascade, \
														 foreign key (user2) references client(id) on delete cascade);"};
	sqlite3_exec(a_db, friendStmt.c_str(), NULL, NULL, NULL);
}

int Db::getUserID(const std::string& username) {
	int i;
	sqlite3_stmt* outStmt;
	std::string stmt = "select id from client where name = ?;";

	sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL);
	sqlite3_bind_text(outStmt, 1, username.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(outStmt) == SQLITE_ROW)
		i = sqlite3_column_int(outStmt, 0);
	else
		i = 0;

	sqlite3_finalize(outStmt);
	return i;
}

void Db::getFriendsID(int UID, std::vector<int>& f, bool confirmed) {
	int u1, u2;
	std::string id = std::to_string(UID);
	sqlite3_stmt* outStmt;
	std::string stmt;

	if (confirmed) //get users where both sides confirmed
		stmt = "select user1, user2 from friend where ((user1=" + id + " or user2=" + id + ") and (friendConfUser1=1 and friendConfUser2=1));";
	else { //get users where this user did not confirm
		stmt = "select user1, user2 from friend where (user1=" + id + " and friendConfUser1=0) or (user2=" + id + " and friendConfUser2=0);";
	}

	sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL);
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

bool Db::friendsPairInDB(const std::string& username1, const std::string& username2) {
//to check whether a this pair is already in the list (confirmed status notwithstanding)
	int u1 = getUserID(username1);
	int u2 = getUserID(username2);
	if (!(u1 && u2))
		return false; //one (the) user(s) do(es) not exist

	std::string idU1 = std::to_string(u1);
	std::string idU2 = std::to_string(u2);

	bool exist = false;
	sqlite3_stmt* outStmt;
	std::string stmt = "select user1 from friend where (user1=" + idU1 + " and user2=" + idU2 + ") or (user1=" + idU2 + " and user2=" + idU1 + ");";

	sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL);
	if (sqlite3_step(outStmt) == SQLITE_ROW)
		exist = true;

	sqlite3_finalize(outStmt);
	return exist;
}
/***********************PUBLIC METHODS ***********************/
Db::Db(std::string dbName, bool createDB) {
//	pthread_mutex_init(&mtxDB, NULL);
	sqlite3_open(dbName.c_str(), &a_db);
	if (createDB)
		insertTables();
	std::string fKey{"PRAGMA foreign_keys = ON;"};
	sqlite3_exec(a_db, fKey.c_str(), NULL, NULL, NULL);
}

Db::~Db() {
//	pthread_mutex_destroy(&mtxDB);
	sqlite3_close(a_db);
}

void Db::getCredentials(const std::string& username, std::string& hash, std::string& salt) {
	int i;
	sqlite3_stmt* outStmt;
	std::string stmt = "select hash, salt from client where name = ?;";

	sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL);
	sqlite3_bind_text(outStmt, 1, username.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(outStmt) == SQLITE_ROW) {
//to be investigated for UTF-8. sqlite3_column_text returns a const unsigned char*
		hash = std::string{reinterpret_cast<const char*>(sqlite3_column_text(outStmt, 0))};
		salt = std::string{reinterpret_cast<const char*>(sqlite3_column_text(outStmt, 1))};
	}
	else
		hash = "";
	sqlite3_finalize(outStmt);
}

bool Db::createUser(const std::string& username, const std::string& hash, const std::string& salt) {
	if (getUserID(username))
		return false;

	bool ok = true;
	sqlite3_stmt* outStmt;
	std::string stmt = "insert into client (name, hash, salt) values (?, ?, ?);";
	std::string stmt_begin = "BEGIN;";
	std::string stmt_rollback = "ROLLBACK;";
	std::string stmt_commit = "COMMIT;";

//	pthread_mutex_lock(&mtxDB);
	sqlite3_exec(a_db, stmt_begin.c_str(), NULL, NULL, NULL);
	try {
		if (sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL) != SQLITE_OK)
			throw 1;
		if (sqlite3_bind_text(outStmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
			throw 1;
		if (sqlite3_bind_text(outStmt, 2, hash.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
			throw 1;
		if (sqlite3_bind_text(outStmt, 3, salt.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
			throw 1;
		if (sqlite3_step(outStmt) != SQLITE_DONE)
			throw 1;
	}
	catch (int e) {
		sqlite3_exec(a_db, stmt_rollback.c_str(), NULL, NULL, NULL);
		return false;
	}

	if (ok)
		sqlite3_exec(a_db, stmt_commit.c_str(), NULL, NULL, NULL);
	sqlite3_finalize(outStmt);
//	pthread_mutex_unlock(&mtxDB);
	return true;
}

bool Db::rmUser(const std::string& username) {
	bool ok = true;
	sqlite3_stmt* outStmt;
	std::string stmt = "delete from client where name=?;";
	std::string stmt_begin = "BEGIN;";
	std::string stmt_rollback = "ROLLBACK;";
	std::string stmt_commit = "COMMIT;";

//	pthread_mutex_lock(&mtxDB);

	sqlite3_exec(a_db, stmt_begin.c_str(), NULL, NULL, NULL);
	try {
		if (sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL) != SQLITE_OK)
			throw 1;
		if (sqlite3_bind_text(outStmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
			throw 1;
		if (sqlite3_step(outStmt) != SQLITE_ROW)
			throw 1;
	}
	catch (int e) {
		ok = false;
		sqlite3_exec(a_db, stmt_rollback.c_str(), NULL, NULL, NULL);
	}

	if (ok)
		sqlite3_exec(a_db, stmt_commit.c_str(), NULL, NULL, NULL);
	sqlite3_finalize(outStmt);
//	pthread_mutex_unlock(&mtxDB);
	return ok;
}

bool Db::addFriendsPair(const std::string& username1, bool confirmedUser1, const std::string& username2, bool confirmedUser2) {
	int u1 = getUserID(username1);
	int u2 = getUserID(username2);
	if (!(u1 && u2))
		return false; //one (the) user(s) do(es) not exist

	bool existingPair = friendsPairInDB(username1, username2);

	std::string idU1 = std::to_string(u1);
	std::string idU2 = std::to_string(u2);
	const std::string c1{confirmedUser1 ? '1' : '0'};
	const std::string c2{confirmedUser2 ? '1' : '0'};

	std::string stmt;
	if (!(confirmedUser1 && confirmedUser2) && !existingPair)
		stmt = "insert into friend (user1, friendConfUser1, user2, friendConfUser2) values (" + idU1 + "," + std::to_string(confirmedUser1) + "," + idU2 + "," + std::to_string(confirmedUser2) + ");";
	else {
		stmt = "update friend set friendConfUser1=" + c1 + ", friendConfUser2=" + c2 + " where user1=" + idU1 + " and user2=" + idU2 + "; update friend set friendConfUser1=" + c2 + ", friendConfUser2=" + c1 + " where user1=" + idU2 + " and user2=" + idU1 + ";";
	}

//	pthread_mutex_lock(&mtxDB);
	sqlite3_exec(a_db, stmt.c_str(), NULL, NULL, NULL);
//	pthread_mutex_unlock(&mtxDB);
	return true;
}

void Db::getFriendsName(const std::string& username, std::vector<std::string>& fList, bool confirmed) {
	std::vector<int> friendsUID;
	std::vector<int>::iterator it;
	int u = getUserID(username);
	getFriendsID(u, friendsUID, confirmed);

	if (!friendsUID.size())
		return; //no friend

	sqlite3_stmt* outStmt;
	std::string stmt = "select name from client where id=?;";

	sqlite3_prepare_v2(a_db, stmt.c_str(), stmt.size(), &outStmt, NULL);
	for (it = friendsUID.begin(); it != friendsUID.end(); it++) {
		sqlite3_bind_int(outStmt, 1, *it);
		if (sqlite3_step(outStmt) == SQLITE_ROW)
			fList.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(outStmt, 0))));
		sqlite3_reset(outStmt);
	}
	sqlite3_finalize(outStmt);
}

bool Db::rmFriendsPair(const std::string& username1, const std::string& username2) {
	int u1 = getUserID(username1);
	int u2 = getUserID(username2);
	if (!(u1 && u2))
		return false; //one (the) user(s) do(es) not exist
	std::string idU1 = std::to_string(u1);
	std::string idU2 = std::to_string(u2);

	bool ok = true;
	std::string stmt1 = "delete from friend where user1=" + idU1 + " and user2=" + idU2 + ";";
	std::string stmt2 = "delete from friend where user1=" + idU2 + " and user2=" + idU1 + ";";
	std::string stmt_begin = "BEGIN;";
	std::string stmt_rollback = "ROLLBACK;";
	std::string stmt_commit = "COMMIT;";

//	pthread_mutex_lock(&mtxDB);

	sqlite3_exec(a_db, stmt_begin.c_str(), NULL, NULL, NULL);
	int i, j;
	try {
		i = sqlite3_exec(a_db, stmt1.c_str(), NULL, NULL, NULL);
		j = sqlite3_exec(a_db, stmt2.c_str(), NULL, NULL, NULL);
		if (i+j)
			throw 1;
	}
	catch (int e) {
		ok = false;
		sqlite3_exec(a_db, stmt_rollback.c_str(), NULL, NULL, NULL);
	}

	if (ok)
		sqlite3_exec(a_db, stmt_commit.c_str(), NULL, NULL, NULL);

//	pthread_mutex_unlock(&mtxDB);
	return ok;
}
