#include "MainWindow.h"

MainWindow::MainWindow() : QWidget () {

    a_lblConnectedFriends = new QLabel("Connected friends");
    a_lwConnectedFriends = new QListWidget();
    a_vLayoutLeft = new QVBoxLayout();
    a_vLayoutLeft->addWidget(a_lblConnectedFriends);
    a_vLayoutLeft->addWidget(a_lwConnectedFriends);

    a_pbLogin = new QPushButton("&Login / new user");
    a_pbNewRoom = new QPushButton("&Open a new chat");
    a_pbFindAFriend = new QPushButton("Find a &friend");
    a_leFindAFriend = new QLineEdit();
    a_pbRmAFriend = new QPushButton("&Kick a foe");
    a_leRmAFriend = new QLineEdit();
    a_pbExit = new QPushButton("E&xit");

    a_vLayoutRight = new QVBoxLayout();
    a_vLayoutRight->addWidget(a_pbLogin);
    a_vLayoutRight->addWidget(a_pbNewRoom);
    a_vLayoutRight->addWidget(a_pbFindAFriend);
    a_vLayoutRight->addWidget(a_leFindAFriend);
    a_vLayoutRight->addWidget(a_pbRmAFriend);
    a_vLayoutRight->addWidget(a_leRmAFriend);
    a_vLayoutRight->addWidget(a_pbExit);

    a_hLayoutUpper = new QHBoxLayout();
    a_hLayoutUpper->addLayout(a_vLayoutLeft);
    a_hLayoutUpper->addLayout(a_vLayoutRight);

    a_leMessage = new QLabel();

    a_layoutMain = new QVBoxLayout();
    a_layoutMain->addLayout(a_hLayoutUpper);
    a_layoutMain->addWidget(a_leMessage);
    this->setLayout(a_layoutMain);

    a_socket = new QTcpSocket(qApp);

    QObject::connect(a_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketErr(QAbstractSocket::SocketError)));
    QObject::connect(a_pbExit, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(a_pbLogin, SIGNAL(clicked()), this, SLOT(connection()));
}


/* ************************************ SLOTS ************************************ */
void MainWindow::socketErr(QAbstractSocket::SocketError err) {
    QString errMsg;
    errMsg = a_socket->errorString();
    a_leMessage->setText(errMsg);
}

void MainWindow::connection() {
    cw = new ConnectionWindow(a_socket);
    cw->show();
}
/* ************************************ END OF SLOTS ************************************ */
