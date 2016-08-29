#include "ConnectionWindow.h"

/* ************************************ PUBLIC ************************************ */
ConnectionWindow::ConnectionWindow(Socket* socket, QWidget* parent) : QWidget(parent), a_socket{socket} {
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
    a_pbExit = new QPushButton("Clos&e");
    a_lblMessage = new QLabel();
    a_lblMessage->setStyleSheet("QLabel{font:italic}");

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
        a_layoutMain->addWidget(a_lblMessage,5,0,1,3);
        a_layoutMain->setAlignment(Qt::AlignCenter);
    this->setLayout(a_layoutMain);

    connect(a_pbExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(a_pbConnect, SIGNAL(clicked()), this, SLOT(slot_cw_connection()));
    connect(a_cbNewUser, SIGNAL(clicked()), this, SLOT(slot_cw_newUserCheckBox()));
    connect(this, SIGNAL(sig_cw_socketConnection(QString,quint16)), a_socket, SLOT(slot_sock_connection(QString,quint16)));
    connect(this, SIGNAL(sig_cw_socketSendMsg(const MsgWriter&)), a_socket, SLOT(slot_sock_sendMsg(const MsgWriter&)));
}

void ConnectionWindow::setMW(QObject *mw) {
    a_mw = mw;
    connect(a_pbExit, SIGNAL(clicked()), a_mw, SLOT(slot_mw_connectionWdwDestroyed()));
    connect(this, SIGNAL(sig_cw_identityCheck(bool)), a_mw, SLOT(slot_mw_userIdentified(bool)));
    //connect(this, SIGNAL(sig_cw_connectionAttempt()), a_mw, SLOT(slot_mw_connectionAttempt()));
}

void ConnectionWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        slot_cw_connection();
    if (event->key() == Qt::Key_Escape)
        this->close();
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ PRIVATE ************************************ */
void ConnectionWindow::readProfile(std::string& username, std::string& domain, std::string& port) {
#ifdef __linux
    std::string filename = "profile";
#elif _WIN32
    std::string filename = "profile.txt";
#endif
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
    bool ok;
    quint16 port = this->a_lePort->text().toUShort(&ok);
    if (ok)
        emit sig_cw_socketConnection(this->a_leServer->text(), port);
    else
        a_lblMessage->setText("Invalid port number");
}

void ConnectionWindow::serverHello(bool isNewUser) {
    emit sig_cw_identityCheck(false);
    std::vector<std::string> tokens;
    tokens.push_back(a_leUsername->text().toStdString());
    tokens.push_back(a_lePwd->text().toStdString());
    MsgWriter mw(tokens, isNewUser ? NEW_USER : HELLO);
    emit sig_cw_socketSendMsg(mw);
}

/* ************************************ END OF PRIVATE ************************************ */

/* ************************************ SLOTS ************************************ */
void ConnectionWindow::slot_cw_newUserCheckBox() {
    if (a_cbNewUser->isChecked())
        a_lePwdConf->setDisabled(false);
    else
        a_lePwdConf->setDisabled(true);
}

void ConnectionWindow::slot_cw_connection() {
    if (a_cbNewUser->isChecked())
        if (a_lePwd->text() != a_lePwdConf->text()) {
            a_lblMessage->setText("The passwords do not match");
            return;
        }
    if (a_lePwd->text().isEmpty()) {
            a_lblMessage->setText("Please enter your password");
            return;
        }
    emit sig_cw_identityCheck(false);
    serverConnectionAttempt();
    serverHello(a_cbNewUser->isChecked());
}

void ConnectionWindow::slot_cw_helloAnswer(int id) {
    if (id == DONT_KNOW_YOU) {
        a_lblMessage->setText("Unknown username");
        emit sig_cw_identityCheck(false);
    }
    else if (id == WRONG_PWD) {
        a_lblMessage->setText("Incorrect password");
        emit sig_cw_identityCheck(false);
    }
    else if (id == WELCOME_BACK) {
        a_lblMessage->setText("Welcome back");
        emit sig_cw_identityCheck(true);
    }
}

void ConnectionWindow::slot_cw_newUserAnswer(int id) {
    if (id == USERNAME_OK) {
        a_lblMessage->setText("You are now registered");
        emit sig_cw_identityCheck(true);
    }
    else if (id == USERNAME_TAKEN) {
        a_lblMessage->setText("This username is already taken");
        emit sig_cw_identityCheck(false);
    }
}
/* ************************************ END OF SLOTS ************************************ */
