#include "ConnectionWindow.h"

/* ************************************ PUBLIC FUNCTIONS ************************************ */
ConnectionWindow::ConnectionWindow(QTcpSocket* socket) : QWidget () {

    std::string username;
    std::string domain;
    std::string port;
    readProfile(username, domain, port);

    setWindowTitle("QMessenger - Login");

    a_lblUsername = new QLabel("&Username");
    a_lblServer = new QLabel("&Server");
    a_lblPort = new QLabel("&Port");
    a_lblPwd =  new QLabel("Pass&word");
    a_lblPwdConf =  new QLabel("Con&firm password");
    a_leUsername = new QLineEdit(QString(username.c_str()));
    a_lblUsername->setBuddy(a_leUsername);
    a_leServer = new QLineEdit(QString(domain.c_str()));
    a_lblServer->setBuddy(a_leServer);
    a_lePort = new QLineEdit(QString(port.c_str()));
    a_lblPort->setBuddy(a_lePort);
    a_lePwd = new QLineEdit();
    a_lblPwd->setBuddy(a_lePwd);
    a_lePwd->setEchoMode(QLineEdit::Password);
    a_lePwdConf = new QLineEdit();
    a_lblPwdConf->setBuddy(a_lePwdConf);
    a_lePwdConf->setEchoMode(QLineEdit::Password);
    a_lePwdConf->setDisabled(true);
    a_cbNewUser = new QCheckBox("&New user");
    a_cbNewUser->setCheckState(Qt::Unchecked);
    a_pbConnect = new QPushButton("&Connect");
    a_pbExit = new QPushButton("E&xit");
    a_leMessage = new QLabel();

    a_layoutMain = new QGridLayout();
    a_layoutMain->addWidget(a_lblUsername,0,0);
    a_layoutMain->addWidget(a_lblServer,1,0);
    a_layoutMain->addWidget(a_lblPort,2,0);
    a_layoutMain->addWidget(a_lblPwd,3,0);
    a_layoutMain->addWidget(a_lblPwdConf,4,0);
    a_layoutMain->addWidget(a_leUsername,0,1);
    a_layoutMain->addWidget(a_leServer,1,1);
    a_layoutMain->addWidget(a_lePort,2,1);
    a_layoutMain->addWidget(a_lePwd,3,1);
    a_layoutMain->addWidget(a_lePwdConf,4,1);
    a_layoutMain->addWidget(a_pbConnect,0,2);
    a_layoutMain->addWidget(a_cbNewUser,1,2);
    a_layoutMain->addWidget(a_pbExit,2,2);
    a_layoutMain->addWidget(a_leMessage,5,0,1,3);

    a_layoutMain->setAlignment(Qt::AlignCenter);

    this->setLayout(a_layoutMain);

    a_socket = socket;
    this->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(a_pbExit, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(a_pbConnect, SIGNAL(clicked()), this, SLOT(connection()));
    QObject::connect(a_cbNewUser, SIGNAL(clicked()), this, SLOT(newUser()));
    QObject::connect(a_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketErr(QAbstractSocket::SocketError)));
    QObject::connect(a_socket, SIGNAL(connected()), this, SLOT(serverConnectionSuccess()));
}

/* ************************************ END OF PUBLIC FUNCTIONS ************************************ */

/* ************************************ PRIVATE FUNCTIONS ************************************ */
void ConnectionWindow::readProfile(std::string& username, std::string& domain, std::string& port) {
    std::string filename = "profile";
    std::vector<std::string> tokens;
    std::vector<std::string>::iterator it;

    if (configFileReader(filename, tokens)) {
        for (it = tokens.begin(); it != tokens.end(); it++) {
            if (*it == "username") {
                it++;
                username = *it;
            }
            else if (*it == "domain") {
                it++;
                domain = *it;
            }
            else if (*it == "port") {
                it++;
                port = *it;
            }
        }
    }
}

void ConnectionWindow::serverConnectionAttempt() {
    a_socket->abort();
    bool ok;
    quint16 port = this->a_lePort->text().toUShort(&ok);
    if (ok) {
        a_leMessage->setText("Connecting to the server");
        a_socket->connectToHost(this->a_leServer->text(), port);
    }
    else
        a_leMessage->setText("Invalid port number");
}
/* ************************************ END OF PRIVATE FUNCTIONS ************************************ */

/* ************************************ SLOTS ************************************ */
void ConnectionWindow::serverConnectionSuccess() {
    a_leMessage->setText("Connected");
}

void ConnectionWindow::socketErr(QAbstractSocket::SocketError err) {
    QString errMsg;
    errMsg = a_socket->errorString();
    a_leMessage->setText(errMsg);
}

void ConnectionWindow::newUser() {
    if (a_cbNewUser->isChecked ())
        a_lePwdConf->setDisabled(false);
    else
        a_lePwdConf->setDisabled(true);
}

void ConnectionWindow::connection() {
    if (a_cbNewUser->isChecked())
        if (a_lePwd->text() != a_lePwdConf->text()) {
            a_leMessage->setText("The passwords do not match");
            return;
        }
    serverConnectionAttempt();
}
/* ************************************ END OF SLOTS ************************************ */
