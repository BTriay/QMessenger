#ifndef ROOMWINDOW_H
#define ROOMWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QListWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSizePolicy>

class RoomWindow : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout* a_layoutMain;
    QHBoxLayout* a_hLayoutUpper;
    QVBoxLayout* a_vLayoutUpperRight;
    QHBoxLayout* a_hLayoutLower;
    QHBoxLayout* a_hLayoutBtns;

    QTextEdit* a_tePrevMsg;
    QLineEdit* a_leMsg;
    QListWidget* a_lwUsers;
    QLabel* a_lblUsers;
    QPushButton* a_pbSend;
    QPushButton* a_pbExit;
    QPushButton* a_pbKickOut;
    QPushButton* a_pbInviteUser;

    int roomNo;

public:
    RoomWindow();

signals:

public slots:
    void sendMsg();

};

#endif // ROOMWINDOW_H
