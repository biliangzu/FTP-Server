#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QThreadPool>
#include <QTimer>
#include <vector>
#include <memory>
#include <QSettings>

#include "client.h"

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void startServer();
    void stopServer();

signals:
    void sendToLogServer(QString);
    void sendToLogClient(QString username, QString message);
    void sendToTable(QString, QString, QString);
    void deleteUser(int);
    void kickAll();
    void removeFromTableSignal(int);

public slots:
    void message(QString);
    void clientMessage(QString, QString);
    void getInfo(QString, QString, QString);
    void deleteUserServer(int);
    void removeFromTable(int);

protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    int connectedClients = 0;

};

#endif // SERVER_H
