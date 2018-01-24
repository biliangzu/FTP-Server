#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent){
}

Server::~Server(){
 //   stopServer();
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
      connect(client, SIGNAL(info(QString,QString)), this, SLOT(getInfo(QString,QString)));
      connect(client, SIGNAL(removeFromTable(int)), this, SLOT(removeFromTable(int)));

      connect(this, SIGNAL(deleteUser(int)), client, SLOT(kickClient(int)));
      connect(this, SIGNAL(kickAll()), client, SLOT(kickAll()));

      client->setSocket(socketDescriptor);
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

void Server::removeFromTable(int id){
    emit removeFromTableSignal(id);
}
