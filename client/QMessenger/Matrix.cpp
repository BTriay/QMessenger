#include "Matrix.h"

/* ************************************ PUBLIC ************************************ */
Matrix::Matrix(Socket* socket, QObject* parent) : QObject(parent), a_socket{socket} {}

void Matrix::setMW(QObject* mw) {
    a_mw = mw;
    connect(this, SIGNAL(sig_mx_sendMsg(const MsgWriter&)), a_socket,
SLOT(slot_sock_sendMsg(const MsgWriter&)));
    connect(this, SIGNAL(sig_mx_friendshipRequest(const std::vector<std::string>&)), a_mw,
SLOT(slot_mw_setFriendRequest(const std::vector<std::string>&)));
    connect(this, SIGNAL(sig_mx_infoDisplay(const std::string&)), a_mw,
SLOT(slot_mw_infoDisplay(const std::string&)));
    connect(this, SIGNAL(sig_mx_helloAnswer(int)), a_mw, SLOT(slot_mw_helloAnswer(int)));
    connect(this, SIGNAL(sig_mx_newUserAnswer(int)), a_mw, SLOT(slot_mw_newUserAnswer(int)));
    connect(this, SIGNAL(sig_mx_userListUpdate(const std::map<std::string, User*>&)), a_mw,
SLOT(slot_mw_userListUpdate(const std::map<std::string, User*>&)));
    connect(this, SIGNAL(sig_mx_confirmedFriends(const std::vector<std::string>&)), a_mw,
SLOT(slot_mw_friendsList(const std::vector<std::string>&)));
}

void Matrix::getUsernames(std::vector<std::string>& usernames) const {
    for (std::map<std::string, User*>::const_iterator it = a_users.begin(); it != a_users.end(); it++)
        usernames.push_back(it->first);
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ PRIVATE ************************************ */
void Matrix::createRoom(int roomNo) {
    a_rooms.insert(std::make_pair(roomNo, new RoomWindow(roomNo, a_socket, this)));
}

void Matrix::addUser(std::string& username) {
    a_users.insert(std::make_pair(username, new User(username)));
}

void Matrix::rmUser(const std::string& username) {
    std::map<std::string, User*>::iterator it;
    if ( (it = a_users.find(username)) != a_users.end() ) {
        delete a_users[username];
        a_users.erase(it);
        emit sig_mx_userListUpdate(a_users);
    }
}

void Matrix::roomMsg(const std::vector<std::string>& tokens) {
    int roomNo = stoi(tokens[0]);
    switch (stoi(tokens[1])) {
        case NEW_MSG:
            a_rooms[roomNo]->publishMsg(tokens);
            break;
        case ROOM_USERS:
            for (std::vector<std::string>::const_iterator it = tokens.begin()+2; it != tokens.end(); it++)
                a_rooms[roomNo]->addUser(*it, false);
            break;
        case NEW_JOINER:
            for (std::vector<std::string>::const_iterator it = tokens.begin()+2; it != tokens.end(); it++)
                a_rooms[roomNo]->addUser(*it, true);
            break;
        case LEAVER:
            for (std::vector<std::string>::const_iterator it = tokens.begin()+2; it != tokens.end(); it++)
                a_rooms[roomNo]->rmUser(*it);
    }
}

void Matrix::presenceUpdate(const std::vector<std::string>& tokens) {
    for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++) {
        User* u;
        try {
            u = a_users.at(*it);
        }
        catch (const std::out_of_range) {
            a_users.insert(std::make_pair(*it, new User(*it)));
            u = a_users[*it];
        }
       ++it;
        u->setPresence(stoi(*it));
        if (u->getPresence() == OFFLINE)
            a_users.erase(a_users.find(u->getUsername()));
    }
    emit sig_mx_userListUpdate(a_users);
}
/* ************************************ END OF PRIVATE ************************************ */

/* ************************************ SLOTS ************************************ */
void Matrix::slot_mx_recMsg(int identifier, const std::vector<std::string>& tokens) {
    switch (identifier) {
        case ROOM_MSG:
            roomMsg(tokens);
            break;
        case PRESENCE_UPDATE:
            presenceUpdate(tokens);
            break;
        case JOIN_THE_ROOM:
            createRoom(stoi(tokens[0]));
            break;
        case BE_MY_FRIEND:
            emit sig_mx_friendshipRequest(tokens);
            break;
        case FRIENDS_LIST:
            emit sig_mx_confirmedFriends(tokens);
            break;
        case HELLO:
            emit sig_mx_helloAnswer(stoi(tokens[0]));
            break;
        case NEW_USER:
            emit sig_mx_newUserAnswer(stoi(tokens[0]));
            break;
        case INVALID_MSG:
            ;
    }
}

void Matrix::slot_mx_sendRoomMsg(int roomNo, const std::string& msg) {
    std::vector<std::string> tokens;
    tokens.push_back(std::to_string(roomNo));;
    tokens.push_back(std::to_string(NEW_MSG));;
    tokens.push_back(msg);
    MsgWriter mw(tokens, ROOM_MSG);
    emit sig_mx_sendMsg(mw);
}

void Matrix::slot_mx_quitRoom(int roomNo) {
    MsgWriter mw(std::to_string(roomNo), LEAVE_THE_ROOM);
    emit sig_mx_sendMsg(mw);
    for (std::map<int, RoomWindow*>::iterator it = a_rooms.begin(); it != a_rooms.end(); it++) {
        if (it->first == roomNo) {
            a_rooms.erase(it);
            return;
        }
    }
}

void Matrix::slot_mx_inviteUser(int roomNo, const std::vector<std::string>& invitees) {
    std::vector<std::string> tokens;
    tokens.push_back(std::to_string(roomNo));;
    std::copy(invitees.begin(), invitees.end(), std::back_insert_iterator< std::vector<std::string> > (tokens));
    MsgWriter mw(tokens, JOIN_THE_ROOM);
    emit sig_mx_sendMsg(mw);
}
/* ************************************ END OF SLOTS ************************************ */
