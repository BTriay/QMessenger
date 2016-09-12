#include "ThreadFunc.h"

/* ************************************ PUBLIC ************************************ */
ThreadFunc::ThreadFunc(Bundle* bundle) {
	a_db = bundle->db;
	a_matrix = bundle->matrix;
	a_epfd = bundle->epfd;
}

void ThreadFunc::operator() (int parserDef, int socket, std::vector<std::string>& tokens) {
	switch (parserDef) {
		case USER_HANGUP:
			a_matrix->rmUser(socket);
			return;
		case HELLO:
		case NEW_USER:
			newConnect(socket, tokens, parserDef);
			return;
	}

	if (!a_matrix->userCreated(socket)) {
#ifdef TEST_COUT
		std::cout << "user not logged in, exit\n";
#endif
		return;
	}

	switch (parserDef) {
		case ROOM_MSG:
			a_matrix->msgToRoom(socket, tokens);
			return;
		case PRESENCE_UPDATE:
			a_matrix->updateUserPresence(socket, stoi(tokens[0]));
			return;
		case NEW_ROOM:
			a_matrix->createRoom(socket, tokens);
			return;
		case JOIN_THE_ROOM:
			inviteToRoom(tokens);
			return;
		case LEAVE_THE_ROOM:
			a_matrix->rmUserFromRoom(socket, stoi(tokens[0])); //stoi(tokens[0]) == room to leave
			return;
		case BE_MY_FRIEND:
			beMyFriend(socket, tokens[0]);
			return;
		case I_AM_YOUR_FRIEND:
			weAreFriends(socket, tokens);
			return;
		case WE_ARE_NOT_FRIENDS:
			weAreNotFriends(socket, tokens);
			return;
		case FRIENDS_LIST:
			friendsList(socket);
			break;
		case DELETE_ME:
			deleteMe(socket);
//			break;
//		case VERSION_NO: //nothing for now, please come back later
//			;
		}
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ PRIVATE ************************************ */
std::string ThreadFunc::getUsername(int socket) const {
	return a_matrix->getUsername(socket);
}

void ThreadFunc::inviteToRoom(std::vector<std::string>& tokens) {
	int roomNo = stoi(tokens[0]);
	tokens.erase(tokens.begin());
	a_matrix->addUsersToRoom(roomNo, tokens, false);
}

void ThreadFunc::confirmPotentialFriends(int socket) {
	std::vector<std::string> maybeFriendsList;
	a_db->getFriendsName(getUsername(socket), maybeFriendsList, false);
	if (maybeFriendsList.size())
		a_matrix->newFriendsRequests(socket, maybeFriendsList);
}

void ThreadFunc::confirmPotentialFriends(std::string username) {
	std::vector<std::string> maybeFriendsList;
	a_db->getFriendsName(username, maybeFriendsList, false);
	if (maybeFriendsList.size())
		a_matrix->newFriendsRequests(username, maybeFriendsList);
}

void ThreadFunc::beMyFriend(int socket, const std::string& token) {
	if (!a_db->addFriendsPair(getUsername(socket), true, token, false))
		a_matrix->unknownFriendName(socket, token);
	confirmPotentialFriends(token);
}

void ThreadFunc::weAreFriends(int socket, const std::vector<std::string>& tokens) {
	for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++)
		a_db->addFriendsPair(getUsername(socket), true, *it, true);
	friendsList(socket);
	udpateOnlineFriends(socket);
}

void ThreadFunc::weAreNotFriends(int socket, const std::vector<std::string>& tokens) {
	for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++) {
		a_db->rmFriendsPair(getUsername(socket), *it);
		a_matrix->offlineUser(socket, tokens);
	}
}

void ThreadFunc::friendsList(int socket) {
	std::vector<std::string> confirmedFriends;
	a_db->getFriendsName(getUsername(socket), confirmedFriends, true);
	a_matrix->sendConfirmedFriends(socket, confirmedFriends);
}

void ThreadFunc::newConnect(int socket, std::vector<std::string>& tokens, int identifier) {
	std::vector<std::string> replyMsg;
	std::string username;
	std::string password;
	bool valid = false;

	try {
		username = tokens[0];
		password = tokens[1];
	}
	catch (const std::out_of_range err) {
		if (identifier == HELLO)
			replyMsg.push_back("Invalid hello message");
		if (identifier == NEW_USER)
			replyMsg.push_back("Invalid new user message");
		MsgWriter mw(replyMsg, INVALID_MSG);
		write(socket, mw.getMsg(), mw.getSz());
		return;
	}

	if (identifier == HELLO) {
		if (a_matrix->userLogged(username))
			replyMsg.push_back(std::to_string(ALREADY_LOGGED));
		else {
			std::string hash; // == clear password until cryptographic functions are in
			std::string salt; //is not used for now, and should be empty
			a_db->getCredentials(username, hash, salt);

			if (hash.empty())
				replyMsg.push_back(std::to_string(DONT_KNOW_YOU));
			else if (password != hash)
				replyMsg.push_back(std::to_string(WRONG_PWD));
			if (replyMsg.empty()) {
				replyMsg.push_back(std::to_string(WELCOME_BACK));
				valid = true;
			}
		}
	}
	else if (identifier == NEW_USER) {
		if (!a_db->createUser(username, password, ""))
			replyMsg.push_back(std::to_string(USERNAME_TAKEN));
		else {
			replyMsg.push_back(std::to_string(USERNAME_OK));
			valid = true;
		}
	}

	MsgWriter mw(replyMsg, identifier);
	write(socket, mw.getMsg(), mw.getSz());

	if (valid) {
		a_matrix->createUser(socket, username);
		if (identifier == HELLO)
			udpateOnlineFriends(socket);
	}
}

void ThreadFunc::udpateOnlineFriends(int socket) {
	std::string username{getUsername(socket)};
	std::vector<std::string> friendsList;
	a_db->getFriendsName(username, friendsList, true);
	a_matrix->addOnlineFriends(socket, friendsList);
	a_matrix->recFriendsPresence(socket);
	a_matrix->sendUserPresence(socket);
	confirmPotentialFriends(socket);
}

void ThreadFunc::deleteMe(int socket) {
	a_db->rmUser(getUsername(socket));
	a_matrix->rmUser(socket);
}
/* ************************************ END OF PRIVATE ************************************ */
