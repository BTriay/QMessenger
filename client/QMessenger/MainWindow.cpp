#include "MainWindow.h"

/* ************************************ PUBLIC ************************************ */
MainWindow::MainWindow() : QWidget () {

    a_vLayoutLeft = new QVBoxLayout();
        a_lwConnectedFriends = new QListWidget();
        a_vLayoutLeft->addWidget(a_lwConnectedFriends);

    a_vLayoutRight = new QVBoxLayout();
        a_pbLogin = new QPushButton("&Login / new user");
        a_pbNewRoom = new QPushButton("&Open a new chat");
        a_pbNewRoom->setEnabled(false);
        a_pbFriendsFoes = new QPushButton("&Friends and foes");
        a_pbFriendsFoes->setEnabled(false);
        a_cbStatus = new QComboBox();
        a_cbStatus->addItem("Online");
        a_cbStatus->addItem("Away");
        a_cbStatus->addItem("Do not disturb");
        a_cbStatus->addItem("Invisible");
        a_cbStatus->setInsertPolicy(QComboBox::NoInsert);
        a_cbStatus->setFixedHeight(28);
        a_pbExit = new QPushButton("E&xit");
        a_vLayoutRight->addWidget(a_pbLogin);
        a_vLayoutRight->addWidget(a_pbNewRoom);
        a_vLayoutRight->addWidget(a_pbFriendsFoes);
        a_vLayoutRight->addWidget(a_cbStatus);
        a_vLayoutRight->addWidget(a_pbExit);
        a_vLayoutRight->insertStretch(4);

    a_hLayoutUpper = new QHBoxLayout();
        a_hLayoutUpper->addLayout(a_vLayoutLeft);
        a_hLayoutUpper->addLayout(a_vLayoutRight);

    a_leMessage = new QLabel();
    a_leMessage->setStyleSheet("QLineEdit{font:italic}");

    a_layoutMain = new QVBoxLayout();
        a_lblConnectedFriends = new QLabel("Connected friends");
        a_lblConnectedFriends->setStyleSheet("QLabel{font:italic}");
        a_layoutMain->addWidget(a_lblConnectedFriends);
        a_layoutMain->addLayout(a_hLayoutUpper);
        a_layoutMain->addWidget(a_leMessage);
    this->setLayout(a_layoutMain);

    a_socket = new Socket();
    a_matrix = new Matrix(a_socket);
    a_socket->setMW(reinterpret_cast<QObject*>(this), a_matrix);
    a_matrix->setMW(reinterpret_cast<QObject*>(this));

    a_connected = false;
    a_identified = false;

    connect(a_pbExit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(a_pbLogin, SIGNAL(clicked()), this, SLOT(slot_mw_connectionWdwInit()));
    connect(a_pbNewRoom, SIGNAL(clicked()), this, SLOT(slot_mw_launchRoomRequest()));
    connect(a_pbFriendsFoes, SIGNAL(clicked()), this, SLOT(slot_mw_friendsFoesWdwInit()));
    connect(a_cbStatus, SIGNAL(activated(int)), this, SLOT(slot_mw_statusChange(int)));
    connect(this, SIGNAL(sig_mw_statusChange(int)), this, SLOT(slot_mw_statusChange(int)));
    connect(this, SIGNAL(sig_mw_socketSendMsg(const MsgWriter&)), a_socket, SLOT(slot_sock_sendMsg(const MsgWriter&)));
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        if (a_identified)
            slot_mw_launchRoomRequest();
        else
            slot_mw_connectionWdwInit();
    if (event->key() == Qt::Key_Escape)
        qApp->quit();
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ PRIVATE ************************************ */
void MainWindow::enableUI() {
    if (!a_connected)
        a_identified = false;
    else
        a_leMessage->setText("Connected to the server");
    if (a_identified) {
        a_leMessage->clear();
        emit sig_mw_statusChange(a_cbStatus->currentIndex());
    }
    a_pendingFriendRequests.clear();
    a_confirmedFriends.clear();
    a_pbNewRoom->setEnabled(a_identified);
    a_pbFriendsFoes->setEnabled(a_identified);
}
/* ************************************ END OF PRIVATE ************************************ */

/* ************************************ SLOTS ************************************ */
void MainWindow::slot_mw_connectionWdwInit() {
    a_cw = new ConnectionWindow(a_socket);
    a_cw->setMW(reinterpret_cast<QObject*>(this));
    connect(this, SIGNAL(sig_mw_helloAnswer(int)), a_cw, SLOT(slot_cw_helloAnswer(int)));
    connect(this, SIGNAL(sig_mw_newUserAnswer(int)), a_cw, SLOT(slot_cw_newUserAnswer(int)));
    a_cw->show();
}

void MainWindow::slot_mw_connectionWdwDestroyed() {
    delete a_cw;
    a_cw = nullptr;
}

void MainWindow::slot_mw_setFriendRequest(const std::vector<std::string>& tokens) {
    if (stoi(tokens[0]) == NEW_REQUEST) {
        std::copy(tokens.begin()+1, tokens.end(), std::back_insert_iterator< std::vector<std::string> > (a_pendingFriendRequests));
        a_leMessage->setText("You have new friend request(s)!");
    }
    else if (stoi(tokens[0]) == UNKNOWN_USER)
        if (a_fw != nullptr)
            emit sig_mw_unknownUser(tokens[1]);
}

void MainWindow::slot_mw_friendsFoesWdwInit() {
    a_leMessage->clear();
    a_fw = new FriendshipWindow(a_pendingFriendRequests, a_socket);
    a_fw->setMW(reinterpret_cast<QObject*>(this));
    a_fw->show();
    connect(this, SIGNAL(sig_mw_friendsList(const std::vector<std::string>&)), a_fw, SLOT(slot_fw_confirmedFriends(const std::vector<std::string>&)));
    connect(this, SIGNAL(sig_mw_unknownUser(const std::string&)), a_fw, SLOT(slot_fw_unknownUser(const std::string&)));
    MsgWriter mw(FRIENDS_LIST);
    emit sig_mw_socketSendMsg(mw);
}

void MainWindow::slot_mw_friendsFoesWdwDestroyed() {
    delete a_fw;
    a_fw = nullptr;
}

void MainWindow::slot_mw_friendsList(const std::vector<std::string>& tokens) {
    a_confirmedFriends.clear();
    std::copy(tokens.begin(), tokens.end(), std::back_insert_iterator< std::vector<std::string> > (a_confirmedFriends));
    if (a_fw != nullptr)
        emit sig_mw_friendsList(a_confirmedFriends);
}

void MainWindow::slot_mw_infoDisplay(const std::string& msg) {
    a_leMessage->setText(QString{msg.c_str()});
}

void MainWindow::slot_mw_userListUpdate(const std::map<std::string, User*>& users) {
    a_lwConnectedFriends->clear();
    int i = 0;
    for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); it++) {
        a_lwConnectedFriends->addItem(QString{(*it).second->getUsername().c_str()});
        switch ((*it).second->getPresence()) {
//http://www.rapidtables.com/web/color/RGB_Color.htm
            case ONLINE:
                a_lwConnectedFriends->item(i)->setTextColor(QColor(0,128,0));
                break;
            case AWAY:
                a_lwConnectedFriends->item(i)->setTextColor(QColor(255,140,0));
                break;
            case DO_NOT_DISTURB:
                a_lwConnectedFriends->item(i)->setTextColor(QColor(255,0,0));
        }
        ++i;
    }
}

void MainWindow::slot_mw_launchRoomRequest() {
    QList<QListWidgetItem*> l = a_lwConnectedFriends->selectedItems();
    if (l.count()) {
        std::vector<std::string> tokens;
        for (int i=0; i < l.count(); i++)
            tokens.push_back(l.at(i)->text().toStdString());
        MsgWriter mw(tokens, NEW_ROOM);
        emit sig_mw_socketSendMsg(mw);
    }
    else
        a_leMessage->setText("No friend selected to start chatting");
}

void MainWindow::slot_mw_socketConnected(bool connected) {
    a_connected = connected;
    enableUI();
}

void MainWindow::slot_mw_userIdentified(bool identified) {
    a_identified = identified;
    enableUI();
}

void MainWindow::slot_mw_helloAnswer(int id) {
    emit sig_mw_helloAnswer(id);
}

void MainWindow::slot_mw_newUserAnswer(int id) {
    emit sig_mw_newUserAnswer(id);
}

void MainWindow::slot_mw_statusChange(int index) {
    std::string status;
    switch (index) {
        case 0:
            status = std::to_string(ONLINE);
            break;
        case 1:
            status = std::to_string(AWAY);
            break;
        case 2:
            status = std::to_string(DO_NOT_DISTURB);
            break;
        case 3:
            status = std::to_string(INVISIBLE);
    }
    MsgWriter mw(status, PRESENCE_UPDATE);
    emit sig_mw_socketSendMsg(mw);
}
/* ************************************ END OF SLOTS ************************************ */
