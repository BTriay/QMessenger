#ifndef FRIENDSHIPWINDOW_H
#define FRIENDSHIPWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QKeyEvent>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <vector>
#include <string>

#include "../../parser.h"

class FriendshipWindow : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* a_layoutMain;
        QLabel* a_lblPendingRequests;
        QLabel* a_lblConfirmedFriends;
        QLabel* a_lblFindAFriend;

    QHBoxLayout* a_layoutTop;
        QListWidget* a_lwPendingRequests;
        QVBoxLayout* a_layoutTopBtns;
            QPushButton* a_pbConfirm;
            QPushButton* a_pbDeny;

    QHBoxLayout* a_layoutUpper;
        QListWidget* a_lwConfirmedFriends;
        QVBoxLayout* a_layoutUpperBtns;
            QPushButton* a_pbNoLongerFriend;

    QHBoxLayout* a_layoutLower;
        QLineEdit* a_leFindAFriend;
        QPushButton* a_pbSendRequest;

    QHBoxLayout* a_layoutBottom;
        QLabel* a_lblMessage;
        QPushButton* a_pbExit;

    Socket* a_socket;
    QObject* a_mw;

    std::vector<std::string> a_confirmedFriends;

public:
    explicit FriendshipWindow(const std::vector<std::string>& usernames, Socket* socket, QWidget* parent = 0);
    void setMW(QObject* mw);
    void keyPressEvent(QKeyEvent* event);

signals:
    void sig_fw_sendConfirmation(const MsgWriter& mw);

public slots:
    void slot_fw_denyFriendship();
    void slot_fw_confirmFriendship();
    void slot_fw_noLongerFriends();
    void slot_fw_findAFriend();
    void slot_fw_confirmedFriends(const std::vector<std::string>& tokens);
    void slot_fw_unknownUser(const std::string& user);
};

#endif // FRIENDSHIPWINDOW_H
