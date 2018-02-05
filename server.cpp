#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent){

}

Server::~Server(){
 //   stopServer();
}

void Server::startServer(){

    QSettings settings;
    quint16 port;

    port = (settings.value("serverPort").isNull() ? 1234 : settings.value("serverPort").toInt());

    if(!this->listen(QHostAddress::Any, port)){
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
      connect(client, SIGNAL(info(QString,QString, QString)), this, SLOT(getInfo(QString,QString, QString)));
      connect(client, SIGNAL(removeFromTable(int)), this, SLOT(removeFromTable(int)));
      connect(client, SIGNAL(clientMessage(QString,QString)), this, SLOT(clientMessage(QString, QString)));

      connect(this, SIGNAL(deleteUser(int)), client, SLOT(kickClient(int)));
      connect(this, SIGNAL(kickAll()), client, SLOT(kickAll()));

      client->setSocket(socketDescriptor);
}

void Server::message(QString msg){
    emit sendToLogServer(msg);
}

void Server::clientMessage(QString username, QString message){
    emit sendToLogClient(username, message);
}

void Server::getInfo(QString id,QString ip, QString username){
    emit sendToTable(id, ip, username);
}

void Server::deleteUserServer(int id){
    emit deleteUser(id);
}

void Server::removeFromTable(int id){
    emit removeFromTableSignal(id);
}
