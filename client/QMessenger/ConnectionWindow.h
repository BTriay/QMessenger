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
#include <QKeyEvent>

#include <QString>
#include <QTime>
#include <string>
#include <vector>
#include <QIntValidator>

#ifdef __linux
#include <arpa/inet.h>
#elif _WIN32
#include <Winsock2.h>
#endif

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
    QLabel* a_lblMessage;
    QPushButton* a_pbConnect;
    QPushButton* a_pbExit;
    QCheckBox* a_cbNewUser;

    void readProfile(std::string& username, std::string& domain, std::string& port);
    void serverConnectionAttempt();
    void serverHello(bool isNewUser);

    Socket* a_socket;
    QObject* a_mw;

public:
    ConnectionWindow(Socket* socket, QWidget* parent = 0);
    void setMW(QObject* mw);
    void keyPressEvent(QKeyEvent* event);

signals:
    void sig_cw_socketConnection(QString host, quint16 port);
    void sig_cw_socketSendMsg(const MsgWriter& mw);
    void sig_cw_identityCheck(bool id);

public slots:
    void slot_cw_newUserCheckBox();
    void slot_cw_connection();
    void slot_cw_helloAnswer(int id);
    void slot_cw_newUserAnswer(int id);
};

//bool configFileReader(const std::string& filename, std::vector<std::string>& tokens);

#endif // CONNECTIONWINDOW_H
