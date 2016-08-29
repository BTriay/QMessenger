#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QKeyEvent>
#include <algorithm>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <vector>
#include <string>

#include "../../parser.h"
#include "ConnectionWindow.h"
#include "Matrix.h"
#include "FriendshipWindow.h"

class MainWindow : public QWidget {
Q_OBJECT

private:
    QVBoxLayout* a_layoutMain;
    QHBoxLayout* a_hLayoutUpper;
    QVBoxLayout* a_vLayoutLeft;
    QVBoxLayout* a_vLayoutRight;

    QLabel* a_lblConnectedFriends;
    QListWidget* a_lwConnectedFriends;
    QPushButton* a_pbLogin;
    QPushButton* a_pbNewRoom;
    QPushButton* a_pbFriendsFoes;
    QComboBox* a_cbStatus;
    QPushButton* a_pbExit;
    QLabel* a_leMessage;

    Socket* a_socket;
    ConnectionWindow* a_cw;
    Matrix* a_matrix;
    FriendshipWindow* a_fw;

    std::vector<std::string> a_pendingFriendRequests;
    std::vector<std::string> a_confirmedFriends;
    bool a_connected;
    bool a_identified;

    void enableUI();

public:
    MainWindow();
    void keyPressEvent(QKeyEvent* event);

signals:
    void sig_mw_socketSendMsg(const MsgWriter& mw);
    void sig_mw_helloAnswer(int id);
    void sig_mw_newUserAnswer(int id);
    void sig_mw_friendsList(const std::vector<std::string>& tokens);
    void sig_mw_unknownUser(const std::string& user);
    void sig_mw_statusChange(int index);

public slots:
    void slot_mw_connectionWdwInit();
    void slot_mw_connectionWdwDestroyed();
    void slot_mw_setFriendRequest(const std::vector<std::string>& tokens);
    void slot_mw_friendsFoesWdwInit();
    void slot_mw_friendsFoesWdwDestroyed();
    void slot_mw_friendsList(const std::vector<std::string>& tokens);
    void slot_mw_infoDisplay(const std::string& msg);
    void slot_mw_socketConnected(bool connected);
    void slot_mw_userIdentified(bool id);
    void slot_mw_userListUpdate(const std::map<std::string, User*>& users);
    void slot_mw_launchRoomRequest();
    void slot_mw_helloAnswer(int id);
    void slot_mw_newUserAnswer(int id);
    void slot_mw_statusChange(int index);
};

#endif // MAINWINDOW_H
