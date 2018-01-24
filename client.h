#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include<QThreadPool>
#include<QDebug>
#include<QHostAddress>

#include "task.h"

class Client : public QObject
{
    Q_OBJECT

signals:
    void message(QString);
    void removeFromTable(int);
    void info(QString, QString);

public:
    explicit Client(QObject *parent = nullptr);
    void setSocket(qintptr descriptor);

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void kickClient(int);
    void kickAll();

 //   void result();

private:
    QTcpSocket *socket;
    int descriptor;
};

#endif // CLIENT_H
