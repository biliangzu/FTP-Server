#include "filesocket.h"

FileSocket::FileSocket(QObject *parent) : QTcpServer(parent)
{

}

FileSocket::~FileSocket(){
    delete socket;
}

void FileSocket::listenFor(){
    listen(QHostAddress::Any);
}

void FileSocket::incomingConnection(int socketDescriptor){
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()), Qt::DirectConnection);

    socket->setSocketDescriptor(socketDescriptor);

    this->close();
}

void FileSocket::sendList(const QByteArray &bytes){
    socket->write(bytes);
}

void FileSocket::sendFile(QString fileName){
    QFile file(fileName);
    QByteArray buffer;

    if(file.open(QIODevice::ReadOnly)){
        qDebug () << "File opened";
    }

    buffer = file.readAll();

    socket->write(buffer);

    file.close();
}

void FileSocket::receiveFile(QString fileName){
    qDebug() << fileNameSet;
    this->fileName = fileName;
    fileNameSet = true;
}

void FileSocket::readyRead(){
    qDebug() << "ready";
    if(fileNameSet){
        QFile file(fileName);

        if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
            qDebug() << file.errorString();
            return;
        } else {
            qDebug() << "File opened";
        }

        QByteArray buffer = socket->readAll();

        file.write(buffer);
        file.close();

        if(!socket->isReadable()){
            fileNameSet = false;
        }
    }
}

