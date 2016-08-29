#include "JoinRoom.h"

/* ************************************ PUBLIC ************************************ */
JoinRoom::JoinRoom(RoomWindow* roomWindow, const std::vector<std::string>& usernames, QWidget *parent) :
    a_rw{roomWindow}, QWidget(parent) {

    a_lblOnlineFriends = new QLabel("<i>Online friends</i>");
    a_lwFriends = new QListWidget();
    for (std::vector<std::string>::const_iterator it = usernames.begin(); it != usernames.end(); it++)
        a_lwFriends->addItem(QString{it->c_str()});
    a_pbInvite = new QPushButton("&Invite");
    a_pbExit = new QPushButton("E&xit");

    a_layoutBtns = new QHBoxLayout();
    a_layoutBtns->addWidget(a_pbInvite);
    a_layoutBtns->addWidget(a_pbExit);
    a_layoutMain = new QVBoxLayout();
    a_layoutMain->addWidget(a_lblOnlineFriends);
    a_layoutMain->addWidget(a_lwFriends);
    a_layoutMain->addLayout(a_layoutBtns);
    this->setLayout(a_layoutMain);

    connect(a_pbExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(a_pbInvite, SIGNAL(clicked()), this, SLOT(slot_jr_invite()));
    connect(this, SIGNAL(sig_jr_inviteThem(const std::vector<std::string>&)), a_rw, SLOT(slot_rw_inviteUsers(const std::vector<std::string>&)));
}

void JoinRoom::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        slot_jr_invite();
    if (event->key() == Qt::Key_Escape)
        this->close();
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ SLOTS ************************************ */
void JoinRoom::slot_jr_invite() {
    QList<QListWidgetItem*> l = a_lwFriends->selectedItems();
    if (l.count()) {
        std::vector<std::string> invitees;
        for (int i=0; i < l.count(); i++)
            invitees.push_back(l.at(i)->text().toStdString());
        emit sig_jr_inviteThem(invitees);
    }
}
/* ************************************ END OF SLOTS ************************************ */
