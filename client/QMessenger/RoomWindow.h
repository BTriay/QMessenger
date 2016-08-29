#ifndef ROOMWINDOW_H
#define ROOMWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QListWidget>
#include <QKeyEvent>

#include <QVBoxLayout>
#include <QHBoxLayout>

//#include <QSizePolicy>
#include <vector>
#include <string>
#include "Matrix.h"
#include "JoinRoom.h"

class Socket;
class Matrix;
class JoinRoom;

class RoomWindow : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout* a_layoutMain;
    QHBoxLayout* a_hLayoutUpper;
    QVBoxLayout* a_vLayoutUpperRight;
    QHBoxLayout* a_hLayoutLower;
    QHBoxLayout* a_hLayoutBtns;

    QTextEdit* a_tePrevMsg;
    QLineEdit* a_leMsg;
    QListWidget* a_lwUsers;
    QLabel* a_lblUsers;
    QPushButton* a_pbSend;
    QPushButton* a_pbExit;
    QPushButton* a_pbInviteUser;

    int a_roomNo;
    Socket* a_socket;
    Matrix* a_matrix;
    JoinRoom* a_joinRoom;
    std::vector<std::string> a_users;

public:
    explicit RoomWindow(int roomNo, Socket* socket, Matrix* matrix);

    void addUser(const std::string& username, bool justJoined);
    void rmUser(const std::string& username);
    void publishMsg(const std::vector<std::string>& tokens);
    void keyPressEvent(QKeyEvent* event);

signals:
    void sig_rw_sendMsg(int roomNo, const std::string& msg);
    void sig_rw_quitRoom(int roomNo);
    void sig_rw_inviteUsers(int roomNo, const std::vector<std::string>& invitees);

public slots:
    void slot_rw_sendMsg();
    void slot_rw_quitRoom();
    void slot_rw_inviteUsers();
    void slot_rw_inviteUsers(const std::vector<std::string>& invitees);

};

#endif // ROOMWINDOW_H
