#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(5);
}

void Client::setSocket(qintptr descriptor){
    this->descriptor = descriptor;
    socket = new QTcpSocket();


    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);

    emit message( "Client connected at " + QString::number(descriptor));
    emit info(QString::number(descriptor), socket->peerAddress().toString());




}

void Client::readyRead(){
    emit message( "Client readyRead");

    QString c = socket->readAll();

    Task *task = new Task();
    task->setCommand(c);

    QThreadPool::globalInstance()->start(task);

}

void Client::connected(){
    emit message("Client connected");
}

void Client::disconnected(){
    emit message("Client disconnected");
}

void Client::kickAll(){
    socket->disconnectFromHost();
}

void Client::kickClient(int descriptor){
    if(this->descriptor == descriptor){
        socket->disconnectFromHost();
    }
}


