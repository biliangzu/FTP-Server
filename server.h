#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QThreadPool>
#include <QTimer>
#include <vector>

#include "client.h"

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void startServer();
    void stopServer();

signals:
    void sendToLog(QString);
    void sendToTable(QString, QString);
    void deleteUser(int);
    void kickAll();

public slots:
    void message(QString);
    void getInfo(QString, QString);
    void deleteUserServer(int);

protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    std::vector <Client*> clients;

};

#endif // SERVER_H
