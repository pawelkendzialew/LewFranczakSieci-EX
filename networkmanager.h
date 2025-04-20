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
    void startServer(quint16 port);
    void connectToServer(const QString &host , quint16 port);
    void sendMessage(const QString &msg);

signals:
    void messageReceived(const QString &msg);
    void serverStarted();
    void clientConnected();
    void commandReceived(const QString &cmd);

    void receivedU(double u);
    void receivedY(double y);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QTcpSocket *clientConnection;
};

#endif // NETWORKMANAGER_H
