#include "StartWindow.h"

StartWindow::StartWindow(QString const & username, QString const & server) : QWidget () {

    setWindowTitle("Welcome to QMessenger!");

    a_lblUsername = new QLabel("&Username");
    a_lblServer = new QLabel("&Server");
    a_vLayoutLabels = new QVBoxLayout();
    a_vLayoutLabels->addWidget(a_lblUsername);
    a_vLayoutLabels->addWidget(a_lblServer);

    a_leUsername = new QLineEdit(username);
    a_lblUsername->setBuddy(a_leUsername);
    a_leServer = new QLineEdit(server);
    a_lblServer->setBuddy(a_leServer);
    a_vLayoutLineEdits = new QVBoxLayout();
    a_vLayoutLineEdits->addWidget(a_leUsername);
    a_vLayoutLineEdits->addWidget(a_leServer);

    a_pbConnect = new QPushButton("&Connect");
    a_pbExit = new QPushButton("E&xit");
    a_vLayoutBtns = new QVBoxLayout();
    a_vLayoutBtns->addWidget(a_pbConnect);
    a_vLayoutBtns->addWidget(a_pbExit);

    a_mainLayout = new QHBoxLayout();
    a_mainLayout->addLayout(a_vLayoutLabels);
    a_mainLayout->addLayout(a_vLayoutLineEdits);
    a_mainLayout->addLayout(a_vLayoutBtns);

    this->setLayout(a_mainLayout);

    QObject::connect(a_pbExit, SIGNAL(clicked()), qApp, SLOT(quit()));

}
