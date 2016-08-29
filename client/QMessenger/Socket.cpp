#include "Socket.h"

/* ************************************ PUBLIC ************************************ */
Socket::Socket() {
    a_socket = new QTcpSocket(qApp);

}

void Socket::setMW(QObject* mw, Matrix* matrix) {
    a_mw = mw;
    a_matrix = matrix;
    connect(a_socket, SIGNAL(connected()), this, SLOT(slot_sock_status()));
    connect(a_socket, SIGNAL(disconnected()), this, SLOT(slot_sock_status()));
    connect(this, SIGNAL(sig_sock_status(bool)), a_mw, SLOT(slot_mw_socketConnected(bool)));
    connect(a_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_sock_socketError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(sig_sock_infoDisplay(const std::string&)), mw, SLOT(slot_mw_infoDisplay(const std::string&)));
}

qint64 Socket::getMsgSz(uint16_t* len) {
    return a_socket->read(reinterpret_cast<char*>(len), sizeof(uint16_t));
}

qint64 Socket::getMsg(char* c_msg, size_t sz) {
    return a_socket->read(c_msg, sz);
}
/* ************************************ END OF PUBLIC ************************************ */

/* ************************************ SLOTS ************************************ */
void Socket::slot_sock_connection(QString host, quint16 port) {
    a_socket->abort();
    a_socket->connectToHost(host, port);

    a_timer = new QTimer(this);
    a_timer->start(60000);
    connect(a_timer, SIGNAL(timeout()), this, SLOT(slot_sock_keepAlive()));
    connect(a_socket, SIGNAL(readyRead()), this, SLOT(slot_sock_recMsg()));
    connect(this, SIGNAL(sig_sock_recMsg(int, const std::vector<std::string>&)), a_matrix, SLOT(slot_mx_recMsg(int,const std::vector<std::string>&)));
}

void Socket::slot_sock_sendMsg(const MsgWriter& mw) {
    a_socket->write(mw.getMsg(), mw.getSz());
}

void Socket::slot_sock_keepAlive() {
    MsgWriter mw(KEEPALIVE);
    a_socket->write(mw.getMsg(), mw.getSz());
}

void Socket::slot_sock_recMsg() {
    std::string msg;
    std::vector<std::string> tokens;
    getSocketMsg(msg, this);
    if (!msg.empty()) {
        int i = msgParser(msg, tokens);

qDebug() << "new message. i = " << i;
for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    qDebug() << (*it).c_str();

        emit sig_sock_recMsg(i, tokens);
        this->slot_sock_recMsg(); //because readyRead() signal is sent only once
    }
}

void Socket::slot_sock_socketError(QAbstractSocket::SocketError) {
    std::string errMsg{a_socket->errorString().toStdString()};
    emit sig_sock_infoDisplay(errMsg);
}

void Socket::slot_sock_status() {
    switch (a_socket->state()) {
        case (QAbstractSocket::UnconnectedState):
            emit sig_sock_status(false);
            break;
        case (QAbstractSocket::ConnectedState):
            emit sig_sock_status(true);
    }
}
/* ************************************ END OF SLOTS ************************************ */
