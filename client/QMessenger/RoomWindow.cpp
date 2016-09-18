#include "RoomWindow.h"

/* ************************************ PUBLIC ************************************ */
RoomWindow::RoomWindow(int roomNo, Socket* socket, Matrix* matrix) : QWidget(), a_roomNo{roomNo},
a_socket{socket}, a_matrix{matrix} {

    setWindowTitle("QMessenger");
//Upper layout
    a_tePrevMsg = new QTextEdit();

    a_lblUsers = new QLabel("<i>Users in the chat</i>");
    a_lwUsers = new QListWidget();

    a_pbExit = new QPushButton("E&xit");
    a_pbInviteUser = new QPushButton("&Invite a user");

    a_vLayoutUpperRight = new QVBoxLayout();

    a_hLayoutBtns = new QHBoxLayout();
    a_hLayoutBtns->addWidget(a_pbInviteUser);
    a_hLayoutBtns->addWidget(a_pbExit);
    a_hLayoutBtns->setAlignment(Qt::AlignHCenter);

    a_vLayoutUpperRight->addWidget(a_lblUsers);
    a_vLayoutUpperRight->addWidget(a_lwUsers);
    a_vLayoutUpperRight->addLayout(a_hLayoutBtns);
    a_vLayoutUpperRight->setSizeConstraint(QLayout::SetFixedSize);

    a_hLayoutUpper = new QHBoxLayout();
    a_hLayoutUpper->addWidget(a_tePrevMsg);
    a_hLayoutUpper->addLayout(a_vLayoutUpperRight);

//Lower layout
    a_leMsg = new QLineEdit();
    a_pbSend = new QPushButton("&Send");
    a_hLayoutLower = new QHBoxLayout();
    a_hLayoutLower->addWidget(a_leMsg);
    a_hLayoutLower->addWidget(a_pbSend);
//Main
    a_layoutMain = new QVBoxLayout();
    a_layoutMain->addLayout(a_hLayoutUpper);
    a_layoutMain->addLayout(a_hLayoutLower);

    a_joinRoom = nullptr;
    this->setLayout(a_layoutMain);
    this->show();

    connect(a_pbExit, SIGNAL(clicked()), this, SLOT(hide()));
    connect(a_pbExit, SIGNAL(clicked()), this, SLOT(slot_rw_quitRoom()));
    connect(this, SIGNAL(sig_rw_quitRoom(int)), a_matrix, SLOT(slot_mx_quitRoom(int)));
    connect(a_pbSend, SIGNAL(clicked()), this, SLOT(slot_rw_sendMsg()));
    connect(this, SIGNAL(sig_rw_sendMsg(int, const std::string&)), a_matrix,
SLOT(slot_mx_sendRoomMsg(int, const std::string&)));
    connect(a_pbInviteUser, SIGNAL(clicked()), this, SLOT(slot_rw_inviteUsers()));
    connect(this, SIGNAL(sig_rw_inviteUsers(int, const std::vector<std::string>&)), a_matrix, SLOT(slot_mx_inviteUser(int, const std::vector<std::string>&)));
}

void RoomWindow::addUser(const std::string& username, bool justJoined) {
    if (justJoined) {
        std::string newJoiner = username + " joined the room";
        a_tePrevMsg->append(QString{newJoiner.c_str()});
    }
    a_users.push_back(username);
    a_lwUsers->addItem(QString{username.c_str()});
    a_lwUsers->sortItems(Qt::AscendingOrder);
}

void RoomWindow::rmUser(const std::string& username) {
    std::string leaver = username + " left the room";
    a_tePrevMsg->append(QString{leaver.c_str()});
    for (std::vector<std::string>::iterator it = a_users.begin(); it != a_users.end(); it++) {
        if (*it == username) {
            a_users.erase(it);
            break;
        }
    }
    if (a_users.size() == 1) {
        a_pbSend->setEnabled(false);
        a_pbInviteUser->setEnabled(false);
    }
    QList<QListWidgetItem*> l = a_lwUsers->findItems(QString{username.c_str()}, Qt::MatchExactly);
    if (l.size())
        a_lwUsers->takeItem(a_lwUsers->row(l[0]));
}

void RoomWindow::publishMsg(const std::vector<std::string>& tokens) {
    QString gmsg = "<b>" + QString{tokens[2].c_str()} + "</b>: ";
    a_tePrevMsg->append(gmsg);
    for (std::vector<std::string>::const_iterator it = tokens.begin() + 3; it != tokens.end(); it++)
        a_tePrevMsg->append(QString{it->c_str()});
}

void RoomWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        slot_rw_sendMsg();
    if (event->key() == Qt::Key_Escape)
        a_pbExit->click();
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ SLOTS ************************************ */
void RoomWindow::slot_rw_sendMsg() {
    emit sig_rw_sendMsg(a_roomNo, a_leMsg->text().toStdString());
    a_leMsg->clear();
}

void RoomWindow::slot_rw_quitRoom() {
    emit sig_rw_quitRoom(a_roomNo);
}

void RoomWindow::slot_rw_inviteUsers() {
    std::vector<std::string> usernames;
    a_matrix->getUsernames(usernames);
    a_joinRoom = new JoinRoom(this, usernames);
    a_joinRoom->show();
}

void RoomWindow::slot_rw_inviteUsers(const std::vector<std::string>& invitees) {
    emit sig_rw_inviteUsers(a_roomNo, invitees);
}
/* ************************************ END OF SLOTS ************************************ */
