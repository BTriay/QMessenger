#ifndef JOINROOM_H
#define JOINROOM_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

#include <vector>
#include <string>

#include "RoomWindow.h"
class RoomWindow;

class JoinRoom : public QWidget {
    Q_OBJECT

private:
    QLabel* a_lblOnlineFriends;
    QListWidget* a_lwFriends;
    QPushButton* a_pbExit;
    QPushButton* a_pbInvite;
    QHBoxLayout* a_layoutBtns;
    QVBoxLayout* a_layoutMain;

    RoomWindow* a_rw;

public:
    explicit JoinRoom(RoomWindow* roomWindow, const std::vector<std::string>& usernames, QWidget *parent = 0);
    void keyPressEvent(QKeyEvent* event);

signals:
    void sig_jr_inviteThem(const std::vector<std::string>& usernames);

public slots:
    void slot_jr_invite();
};

#endif // JOINROOM_H
