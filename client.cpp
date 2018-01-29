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

    if(c == "LIST"){
        qDebug() << "Werkt";
        QString dir = QDir::homePath();
        doList(dir);
    }

//    Task *task = new Task();
//    task->setCommand(c);

//    QThreadPool::globalInstance()->start(task);

}

void Client::doList(QString & dir){
    QFileInfo info;
    info.setFile(dir);

    if(info.isDir()){
        QFileInfoList entryList = info.dir().entryInfoList();
        QStringList outlines;

        foreach(QFileInfo entry, entryList){
            if(entry.isHidden()) qDebug() << "Hidden";
            outlines.append(generateList(entry));
        }
        qDebug() << outlines;
        socket->write(outlines.join(QString()).toLocal8Bit());
    } else {
        qDebug() << "Else" <<  generateList(info);
    }
}

void Client::sendData(const QByteArray &bytes){

}

QString Client::generateList(const QFileInfo &entry) const {
    QString line;
    line += QLocale(QLocale::English).toString(entry.lastModified(), QLatin1String("MM-dd-yy  hh:mmAP"));
    if (entry.isDir()){
        line += QLatin1String("       <DIR>         ");
    } else {
        line += QString("%1").arg(entry.size(), 20);
    }

    line += QLatin1Char(' ');
    line += entry.fileName();

    line += QLatin1String("\r\n");
    return line;
}

void Client::connected(){
    emit message("Client " + QString::number(socket->socketDescriptor()) + " connected");
}

void Client::disconnected(){
    emit message("Client " + QString::number(this->descriptor) +" disconnected");
    emit removeFromTable(this->descriptor);
    delete this;
}

void Client::kickAll(){
    socket->disconnectFromHost();
}

void Client::kickClient(int descriptor){
    if(this->descriptor == descriptor){
        socket->disconnectFromHost();
    }
}


