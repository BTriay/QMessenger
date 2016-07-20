#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QString>

class StartWindow : public QWidget {
Q_OBJECT

private:
    QHBoxLayout* a_mainLayout;
    QVBoxLayout* a_vLayoutLabels;
    QVBoxLayout* a_vLayoutLineEdits;
    QVBoxLayout* a_vLayoutBtns;

    QLabel* a_lblUsername;
    QLabel* a_lblServer;

    QLineEdit* a_leUsername;
    QLineEdit* a_leServer;

    QPushButton* a_pbConnect;
    QPushButton* a_pbExit;

public:
    StartWindow(QString const & username, QString const & server);

signals:

public slots:

};

#endif // STARTWINDOW_H
