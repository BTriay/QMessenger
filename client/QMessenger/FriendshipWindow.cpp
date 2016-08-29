#include "FriendshipWindow.h"

/* ************************************ PUBLIC ************************************ */
FriendshipWindow::FriendshipWindow(const std::vector<std::string>& usernames, Socket* socket, QWidget* parent) : QWidget(parent), a_socket{socket} {
    setWindowTitle("QMessenger - friends and foes");


    a_layoutTop = new QHBoxLayout();
        a_lwPendingRequests = new QListWidget();
        for (std::vector<std::string>::const_iterator it = usernames.begin(); it != usernames.end(); it++)
            a_lwPendingRequests->addItem(QString{it->c_str()});
        a_lwPendingRequests->setSelectionMode(QAbstractItemView::ExtendedSelection);
        a_lwPendingRequests->setFixedHeight(100);
        a_layoutTop->addWidget(a_lwPendingRequests);
    a_layoutTopBtns = new QVBoxLayout();
        a_pbConfirm = new QPushButton("&Confirm");
        a_pbConfirm->setFixedWidth(110);
        a_pbDeny = new QPushButton("&Deny");
        a_pbDeny->setFixedWidth(110);
        a_layoutTopBtns->addWidget(a_pbConfirm);
        a_layoutTopBtns->addWidget(a_pbDeny);
        a_layoutTopBtns->insertStretch(2);
    a_layoutTop->addLayout(a_layoutTopBtns);

    a_layoutUpper = new QHBoxLayout();
        a_lwConfirmedFriends = new QListWidget();
        a_lwConfirmedFriends->setSelectionMode(QAbstractItemView::ExtendedSelection);
        a_lwConfirmedFriends->setFixedHeight(100);
        a_layoutUpper->addWidget(a_lwConfirmedFriends);
    a_layoutUpperBtns = new QVBoxLayout();
        a_pbNoLongerFriend = new QPushButton("No l&onger friends");
        a_pbNoLongerFriend->setFixedWidth(110);
        a_layoutUpperBtns->addWidget(a_pbNoLongerFriend);
        a_layoutUpperBtns->insertStretch(1);
    a_layoutUpper->addLayout(a_layoutUpperBtns);

    a_layoutLower = new QHBoxLayout();
        a_leFindAFriend = new QLineEdit();
        a_pbSendRequest = new QPushButton("Send &request");
        a_pbSendRequest->setFixedWidth(110);
        a_layoutLower->addWidget(a_leFindAFriend);
        a_layoutLower->addWidget(a_pbSendRequest);

    a_layoutBottom = new QHBoxLayout();
        a_lblMessage = new QLabel();
        a_lblMessage->setStyleSheet("QLabel{font:italic}");
        a_pbExit = new QPushButton("E&xit");
        a_pbExit->setFixedWidth(110);
        a_layoutBottom->addWidget(a_lblMessage);
        a_layoutBottom->addWidget(a_pbExit);

    a_layoutMain = new QVBoxLayout();
        a_lblPendingRequests = new QLabel("<i>Pending friend request(s)</i>");
        a_layoutMain->addWidget(a_lblPendingRequests);
        a_layoutMain->addLayout(a_layoutTop);
        a_lblConfirmedFriends = new QLabel("<i>Confirmed friends</i>");
        a_layoutMain->addWidget(a_lblConfirmedFriends);
        a_layoutMain->addLayout(a_layoutUpper);
        a_lblFindAFriend = new QLabel("<i>Be my &friend</i>");
        a_lblFindAFriend->setBuddy(a_leFindAFriend);
        a_layoutMain->addWidget(a_lblFindAFriend);
        a_layoutMain->addLayout(a_layoutLower);
        a_layoutMain->addLayout(a_layoutBottom);
    this->setLayout(a_layoutMain);

    connect(a_pbConfirm, SIGNAL(clicked()), this, SLOT(slot_fw_confirmFriendship()));
    connect(a_pbDeny, SIGNAL(clicked()), this, SLOT(slot_fw_denyFriendship()));
    connect(a_pbNoLongerFriend, SIGNAL(clicked()), this, SLOT(slot_fw_noLongerFriends()));
    connect(a_pbSendRequest, SIGNAL(clicked()), this, SLOT(slot_fw_findAFriend()));
    connect(a_pbExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(sig_fw_sendConfirmation(const MsgWriter&)), a_socket, SLOT(slot_sock_sendMsg(const MsgWriter&)));
}

void FriendshipWindow::setMW(QObject* mw) {
    a_mw = mw;
    connect(a_pbExit, SIGNAL(clicked()), a_mw, SLOT(slot_mw_friendsFoesWdwDestroyed()));
}

void FriendshipWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape)
        this->close();
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ SLOTS ************************************ */
void FriendshipWindow::slot_fw_denyFriendship() {
    std::vector<std::string> tokens;
    QList<QListWidgetItem*> pl = a_lwPendingRequests->selectedItems();
    for (int i=0; i < pl.count(); i++) {
        tokens.push_back(pl.at(i)->text().toStdString());
        QList<QListWidgetItem*> rm = a_lwPendingRequests->findItems(pl.at(i)->text(), Qt::MatchExactly);
        a_lwPendingRequests->takeItem(a_lwPendingRequests->row(rm[0]));
    }

    MsgWriter mw(tokens, WE_ARE_NOT_FRIENDS);
    emit sig_fw_sendConfirmation(mw);
    a_lblMessage->setText("You did not make a friend today");
}

void FriendshipWindow::slot_fw_confirmFriendship() {
    std::vector<std::string> tokens;
    QList<QListWidgetItem*> l = a_lwPendingRequests->selectedItems();
    for (int i=0; i < l.count(); i++)
        tokens.push_back(l.at(i)->text().toStdString());
    MsgWriter mw(tokens, I_AM_YOUR_FRIEND);
    emit sig_fw_sendConfirmation(mw);
    a_lblMessage->setText("You have a new friend");
}

void FriendshipWindow::slot_fw_noLongerFriends() {
    std::vector<std::string> tokens;
    QList<QListWidgetItem*> pl = a_lwConfirmedFriends->selectedItems();
    for (int i=0; i < pl.count(); i++) {
        tokens.push_back(pl.at(i)->text().toStdString());
        QList<QListWidgetItem*> rm = a_lwConfirmedFriends->findItems(pl.at(i)->text(), Qt::MatchExactly);
        a_lwConfirmedFriends->takeItem(a_lwConfirmedFriends->row(rm[0]));
    }

    MsgWriter mw(tokens, WE_ARE_NOT_FRIENDS);
    emit sig_fw_sendConfirmation(mw);
    a_lblMessage->setText("You lost a friend today");
}

void FriendshipWindow::slot_fw_findAFriend() {
    MsgWriter mw(a_leFindAFriend->text().toStdString(), BE_MY_FRIEND);
    emit sig_fw_sendConfirmation(mw);
    a_lblMessage->setText("Friend request sent");
}

void FriendshipWindow::slot_fw_confirmedFriends(const std::vector<std::string>& users) {
    a_lwConfirmedFriends->clear();
    for (std::vector<std::string>::const_iterator it = users.begin(); it != users.end(); it++)
        a_lwConfirmedFriends->addItem(QString{it->c_str()});
}

void FriendshipWindow::slot_fw_unknownUser(const std::string& user) {
    a_lblMessage->setText("User \"" + QString{user.c_str()} + "\" unknown");
}
/* ************************************ END OF SLOTS ************************************ */
