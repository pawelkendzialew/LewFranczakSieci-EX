/*#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

class NetworkManager
{
public:
    NetworkManager();
};

#endif // NETWORKMANAGER_H
*/
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    void startServer(quint16 port = 12345);
    void connectToServer(const QString &host = "127.0.0.1", quint16 port = 12345);
    void sendMessage(const QString &msg);

signals:
    void messageReceived(const QString &msg);
    void serverStarted();
    void clientConnected();
    void commandReceived(const QString &cmd);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QTcpSocket *clientConnection;
};

#endif // NETWORKMANAGER_H
