#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include<QThreadPool>
#include<QDebug>
#include<QHostAddress>

#include <QFileInfo>
#include <QDir>
#include <QDateTime>

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
    void doList(QString & dir);
    QTcpSocket *socket;
    int descriptor;
    QString generateList(const QFileInfo &entry) const;
    void sendData(const QByteArray &bytes);
};

#endif // CLIENT_H
