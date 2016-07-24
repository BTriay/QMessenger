#include "RoomWindow.h"

RoomWindow::RoomWindow() : QWidget() {

    setWindowTitle("QMessenger");
//Upper layout
    a_tePrevMsg = new QTextEdit();

    a_lblUsers = new QLabel("<i>Users in the chat</i>");
    a_lwUsers = new QListWidget();

    a_pbExit = new QPushButton("E&xit");
    a_pbKickOut = new QPushButton("&Kick out a user");
    a_pbInviteUser = new QPushButton("&Invite a user");

    a_vLayoutUpperRight = new QVBoxLayout();

    a_hLayoutBtns = new QHBoxLayout();
    a_hLayoutBtns->addWidget(a_pbInviteUser);
    a_hLayoutBtns->addWidget(a_pbKickOut);
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

    this->setLayout(a_layoutMain);

    QObject::connect(a_pbExit, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(a_pbSend, SIGNAL(clicked()), this, SLOT(sendMsg()));
}

void RoomWindow::sendMsg() {
/*
    uint16_t sz = a_leMsg->text().toStdString().length();
    uint16_t nsz = htons(sz);
    char buf[200];
    memcpy(buf, &nsz, sizeof(uint16_t));
    memcpy(buf+sizeof(uint16_t), a_leMsg->text().toStdString().c_str(), sz);
    buf[sz + sizeof(uint16_t)] = '\0';
    int s = a_socket->write( (const char*) &buf, sz+sizeof(uint16_t));
    if (s==-1)
        qDebug() << "socket write error";
        */
}
