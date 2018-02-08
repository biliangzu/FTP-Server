#ifndef FILESOCKET_H
#define FILESOCKET_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>

class FileSocket : public QTcpServer
{
    Q_OBJECT
public:
    explicit FileSocket(QObject *parent = nullptr);
    ~FileSocket();

signals:
    //void fileDownloaded(QFile);
    void fileUploaded();

public slots:
    void readyRead();

public:
    void listenFor();
    void sendFile(QString fileName);
    void receiveFile(QString fileName);

private:
    QTcpSocket *socket;
    QString fileName;
    void incomingConnection(int socketDescriptor);
};

#endif // FILESOCKET_H
