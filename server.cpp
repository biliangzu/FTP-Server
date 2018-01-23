#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent){
    threadPool = new QThreadPool(this);

    //TODO Max.amount of connections.
    //threadPool->setMaxThreadCount(connectionAmount);
}

Server::~Server(){
    delete threadPool;
}

void Server::startServer(){
    if(!this->listen(QHostAddress::Any, 1234)){
        message("Couldn't start server... Port already in use?");
    } else {
        message("Listening...");
    }
}

void Server::stopServer(){
    this->close();
    emit kickAll();
}

void Server::incomingConnection(qintptr socketDescriptor){

      Client *client = new Client(this);


      connect(client, SIGNAL(message(QString)), this, SLOT(message(QString)));
      connect(client, SIGNAL(info(QString,QString)), this, SLOT(getInfo(QString,QString)), Qt::DirectConnection);

      connect(this, SIGNAL(deleteUser(int)), client, SLOT(kickClient(int)), Qt::QueuedConnection);
      connect(this, SIGNAL(kickAll()), client, SLOT(kickAll()));

      client->setSocket(socketDescriptor);

//    ConnectionThread *task = new ConnectionThread();

//    task->setAutoDelete(true);
//    task->setSocketDescriptor(socketDescriptor);

//    connect(task, SIGNAL(message(QString)), this, SLOT(message(QString)));
//    connect(task, SIGNAL(info(QString,QString)), this, SLOT(getInfo(QString,QString)),Qt::DirectConnection);

//    connect(this, SIGNAL(deleteUser(int)), task, SLOT(deleteUserInTask(int)), Qt::QueuedConnection);
//    threadPool->start(task);

}

void Server::message(QString msg){
    //Send message to GUI.
    emit sendToLog(msg);
}

void Server::getInfo(QString id,QString ip){
    emit sendToTable(id, ip);
}

void Server::deleteUserServer(int id){
    emit deleteUser(id);
}
