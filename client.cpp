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
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));

    socket->setSocketDescriptor(descriptor);

    emit message( "Client connected at " + QString::number(descriptor));
    sendData("220 Welcome to Jussie's FTP server!\r\n");
    socket->flush();
}

void Client::readyRead(){
    QString c = socket->readLine();

    if (c.startsWith("LOGIN")){
        doLogin(c);
    } else if(c.startsWith("LIST")){
        doList(c);
        emit clientMessage(this->username, c);
    }

//    Task *task = new Task();
//    task->setCommand(c);

//    QThreadPool::globalInstance()->start(task);
}

void Client::doLogin(QString creds){
    QMap<QString, QString> users = getUsers();

    QString name = creds.split(" ").at(1);
    QString password = creds.split(" ").at(2);

    name = name.trimmed();
    password = password.trimmed();

    qDebug() << name << password;

    if(!users.contains(name)){
        sendData("430 Invalid username or password\r\n");
        socket->disconnectFromHost();
    } else if(users[name] != password){
        sendData("430 Invalid username or password\r\n");
        socket->disconnectFromHost();
    } else {
        sendData("230 Login OK!\r\n");
        this->username = name;
        emit clientMessage(username, "LOGGED IN");
        emit info(QString::number(descriptor), socket->peerAddress().toString(), username);
        authorized = true;
    }
}

QMap<QString, QString> Client::getUsers(){
    QFile file("users.us");
    QMap <QString, QString> users;

    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "Error";
        } else {
            QDataStream in(&file);
            users.clear();
            in.setVersion(QDataStream::Qt_5_9);
            in >> users;
            file.close();
        }
    }

    return users;
}

void Client::doList(QString & path){

    QString dir = (path.trimmed().split(" ").at(1) != NULL) ? dir : settings.value("rootPath").toString(); // fout
    qDebug() << dir;

    QFileInfo info;
    info.setFile(path);

    if(info.isDir()){
        QFileInfoList entryList = info.dir().entryInfoList();
        QStringList outlines;

        foreach(QFileInfo entry, entryList){
            if(entry.isHidden()) qDebug() << "Hidden";
            outlines.append(generateList(entry));
        }

        sendData(outlines.join(QString()).toLocal8Bit());
    } else {
        qDebug() << "Else" <<  generateList(info);
    }
}

void Client::sendData(const QByteArray &bytes){
    //Todo Datasocket
    socket->write(bytes);
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


