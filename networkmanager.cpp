#include "NetworkManager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), server(nullptr), socket(nullptr), clientConnection(nullptr) {}

void NetworkManager::startServer(quint16 port) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);

    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start on port" << port;
    } else {
        qDebug() << "Server started on port" << port;
        emit serverStarted();
    }
}

void NetworkManager::connectToServer(const QString &host, quint16 port) {
    socket = new QTcpSocket(this);
    if (clientConnection) {
        connect(clientConnection, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    }

    socket->connectToHost(host, port);

    if (!socket->waitForConnected(3000)) {
        qDebug() << "Connection failed";
    } else {
        qDebug() << "Connected to server at" << host << ":" << port;
        socket->write("Hello from client\n");
        emit clientConnected();
    }
}

void NetworkManager::onNewConnection() {
    clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    qDebug() << "New connection established";
}

void NetworkManager::onReadyRead() {
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if (senderSocket) {
        QByteArray data = senderSocket->readAll();
        qDebug() << "Received data:" << data;
        QString msg = QString(data).trimmed();
        qDebug() << "Received command:" << msg;

        if (msg == "START") {
            emit commandReceived("START");
        } else if (msg == "STOP") {
            emit commandReceived("STOP");
        } else {
            emit messageReceived(msg);  // logujemy inne
        }
    }
}
void NetworkManager::sendMessage(const QString &msg) {
    if (socket && socket->isOpen()) {
        socket->write(msg.toUtf8());
    }
}
