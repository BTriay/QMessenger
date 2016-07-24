#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QTcpSocket>

#include "../../parser.h"
#include "ConnectionWindow.h"

class MainWindow : public QWidget {
Q_OBJECT

private:
    QVBoxLayout* a_layoutMain;
    QHBoxLayout* a_hLayoutUpper;
    QVBoxLayout* a_vLayoutLeft;
    QVBoxLayout* a_vLayoutRight;

    QLabel* a_lblConnectedFriends;
    QListWidget* a_lwConnectedFriends;
    QPushButton* a_pbLogin;
    QPushButton* a_pbNewRoom;
    QPushButton* a_pbFindAFriend;
    QLineEdit* a_leFindAFriend;
    QPushButton* a_pbRmAFriend;
    QLineEdit* a_leRmAFriend;
    QPushButton* a_pbExit;

    QLabel* a_leMessage;

    QTcpSocket* a_socket;
public:
    MainWindow();

signals:

public slots:
    void socketErr(QAbstractSocket::SocketError err);
    void connection();
};

#endif // MAINWINDOW_H
