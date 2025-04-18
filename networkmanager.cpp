#include "NetworkManager.h"
#include <QDebug>
#include <QMessageBox>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), server(nullptr), socket(nullptr), clientConnection(nullptr) {}

void NetworkManager::startServer(quint16 port) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);

    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start on port" << port;
        QMessageBox::critical(nullptr, "Błąd", "Nie udało się uruchomić serwera na porcie " + QString::number(port));
    } else {
        QString msg = "Serwer uruchomiony na porcie " + QString::number(port);
        qDebug() << msg;
        QMessageBox::information(nullptr, "Serwer", msg);
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
        QString failMsg = "Nie udało się połączyć z serwerem pod adresem:\n" + host + "\nPort: " + QString::number(port);
        qDebug() << failMsg;
        QMessageBox::critical(nullptr, "Błąd połączenia", failMsg);
    } else {
        QString successMsg = "Połączono z serwerem!\nAdres: " + host + "\nPort: " + QString::number(port);
        qDebug() << successMsg;
        QMessageBox::information(nullptr, "Sukces", successMsg);
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
