#ifndef MATRIX_H
#define MATRIX_H

#include <QApplication>
#include <QObject>

#include <map>
#include <string>
#include <algorithm>

#include "RoomWindow.h"
#include "../../User.h"
#include "../../parser.h"

class RoomWindow;
class Socket;
class MsgWriter;

class Matrix : public QObject {
    Q_OBJECT
private:
    std::map<std::string, User*> a_users;
    std::map<int, RoomWindow*> a_rooms;
    Socket* a_socket;
    QObject* a_mw;

    void createRoom(int roomNo);
    void addUser(std::string& username);
    void rmUser(const std::string& username);
    void roomMsg(const std::vector<std::string>& tokens);
    void presenceUpdate(const std::vector<std::string>& tokens);

public:
    Matrix(Socket* socket, QObject* parent = 0);
    void setMW(QObject* mw);
    void getUsernames(std::vector<std::string>& usernames) const;

signals:
    void sig_mx_sendMsg(const MsgWriter& mw);
    void sig_mx_friendshipRequest(const std::vector<std::string>& tokens);
    void sig_mx_infoDisplay(const std::string& msg);
    void sig_mx_helloAnswer(int id);
    void sig_mx_newUserAnswer(int id);
    void sig_mx_userListUpdate(const std::map<std::string, User*>& users);
    void sig_mx_confirmedFriends(const std::vector<std::string>& users);

public slots:
    void slot_mx_recMsg(int identifier, const std::vector<std::string>& tokens);
    void slot_mx_sendRoomMsg(int roomNo, const std::string& msg);
    void slot_mx_quitRoom(int roomNo);
    void slot_mx_inviteUser(int roomNo, const std::vector<std::string>& invitees);
};

#endif // MATRIX_H
