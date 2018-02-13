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
    currentDir = settings.value("rootPath").toString();

    emit message( "Client connected at " + QString::number(descriptor));
    sendResponse(settings.value("welcomeMessage").toString().toLocal8Bit());
    socket->flush();
}

void Client::readyRead(){
    QString c = socket->readLine();

    if (c.startsWith("LOGIN")){
        doLogin(c);
    } else if(c.startsWith("LIST")){
        sendResponse("333");
        doList(c);
        emit clientMessage(this->username, c);
    } else if(c.startsWith("PUT")){
        doPut(c);
        emit clientMessage(this->username, c);
    }

//    Task *task = new Task();
//    task->setCommand(c);

//    QThreadPool::globalInstance()->start(task);
}

void Client::doPut(QString fileName){
    fileName = currentDir + "/" + fileName.trimmed().split(" ").at(1);
    qDebug() << "Do put" << fileName;
    fileSocket->receiveFile(fileName);
}

void Client::doLogin(QString creds){
    QMap<QString, QString> users = getUsers();

    QString name = creds.split(" ").at(1);
    QString password = creds.split(" ").at(2);

    name = name.trimmed();
    password = password.trimmed();

    qDebug() << name << password;

    if(!users.contains(name)){
        sendResponse("430 Invalid username or password");
        socket->disconnectFromHost();
    } else if(users[name] != password){
        sendResponse("430 Invalid username or password");
        socket->disconnectFromHost();
    } else {
        openFileSocket();
        sendResponse(("230 Login OK! " + QString::number(filePort)).toLocal8Bit());
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

    QDir dir(currentDir);
    qDebug() << "Command: " << path;
    QString dirPath = (path.trimmed().split(" ").at(1) != "/") ? currentDir+"/"+path.trimmed().split(" ").at(1) : settings.value("rootPath").toString();
    if(!dir.cd(dirPath)){
        qDebug()<< "Mislukt!" << dirPath;
    }
    currentDir = dir.absolutePath();

    // TODO: FIX DIRECTORY NAME WITH MORE THAN 1 STRING
    QFileInfo info(currentDir);

    if(info.isDir()){
        QFileInfoList entryList = dir.entryInfoList();
        QStringList outlines;

        foreach(QFileInfo entry, entryList){
            if(entry.isHidden()) qDebug() << "Hidden";
            outlines.append(generateList(entry));
        }

        fileSocket->sendList(outlines.join(QString()).toLocal8Bit());
    } else {
        qDebug() << "1file";
        fileSocket->sendList(generateList(info).toLocal8Bit());
    }
}

void Client::sendResponse(const QByteArray &bytes){
    socket->write(bytes +"\r\n");
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

void Client::openFileSocket(){
    fileSocket = new FileSocket();

    fileSocket->listenFor();
    filePort = fileSocket->serverPort();

   // sendResponse("225 Data connection open");
}


