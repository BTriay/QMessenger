#ifndef SOCKET_H
#define SOCKET_H

#include <QApplication>
#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTimer>

#include "Matrix.h"
#include "../../parser.h"
class MsgWriter;
class Matrix;

class Socket : public QObject {
    Q_OBJECT

private:
    bool connected;
    QTcpSocket* a_socket;
    QTimer* a_timer;
    Matrix* a_matrix;
    QObject* a_mw;

public:
    explicit Socket();
    void setMW(QObject* mw, Matrix* matrix);
    qint64 getMsgSz(uint16_t* len);
    qint64 getMsg(char* c_msg, size_t sz);

signals:
    void sig_sock_infoDisplay(const std::string& msg);
    void sig_sock_recMsg(int identifier, const std::vector<std::string>& tokens);
    void sig_sock_status(bool connected);

public slots:
    void slot_sock_connection(QString host, quint16 port);
    void slot_sock_sendMsg(const MsgWriter& mw);
    void slot_sock_keepAlive();
    void slot_sock_recMsg();
    void slot_sock_status();
    void slot_sock_socketError(QAbstractSocket::SocketError);
};

#endif // SOCKET_H
