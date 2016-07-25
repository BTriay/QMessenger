#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QString>
#include <QTcpSocket>
#include <arpa/inet.h>

#include "../../parser.h"

class ConnectionWindow : public QWidget {
Q_OBJECT

private:
    QGridLayout* a_layoutMain;
    QLabel* a_lblUsername;
    QLabel* a_lblServer;
    QLabel* a_lblPort;
    QLabel* a_lblPwd;
    QLabel* a_lblPwdConf;
    QLineEdit* a_leUsername;
    QLineEdit* a_leServer;
    QLineEdit* a_lePort;
    QLineEdit* a_lePwd;
    QLineEdit* a_lePwdConf;
    QLabel* a_leMessage;
    QPushButton* a_pbConnect;
    QPushButton* a_pbExit;
    QCheckBox* a_cbNewUser;

    void readProfile(std::string& username, std::string& domain, std::string& port);
    void serverConnectionAttempt();

    QTcpSocket* a_socket;

public:
    ConnectionWindow(QTcpSocket* socket);

signals:

public slots:
    void serverConnectionSuccess();
    void socketErr(QAbstractSocket::SocketError err);
    void newUser();
    void connection();
};

#endif // CONNECTIONWINDOW_H
