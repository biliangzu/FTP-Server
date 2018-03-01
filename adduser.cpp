#include "adduser.h"
#include "ui_adduser.h"
#include <QMessageBox>
#include <QDebug>
addUser::addUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addUser)
{
    ui->setupUi(this);
}

addUser::~addUser()
{
    delete ui;
}

void addUser::on_addUserButton_clicked()
{
    QString username = ui->userName->text();
    QString password = ui->password->text();
    QString cPassword = ui->cPassword->text();

    if(username.isEmpty() || password.isEmpty() || cPassword.isEmpty()){
        ui->message->setText("Please enter all fields!");
    }else {
        if(password != cPassword){
            ui->message->setText("Passwords do not match!");
        } else {
            add(username, password);
        }
    }
}

void addUser::load(){
    QFile file("users.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Error";
    } else {
        QDataStream in(&file);
        users.clear();
        in.setVersion(QDataStream::Qt_5_9);
        in >> users;
    }

    file.flush();
    file.close();

}

void addUser::add(QString &username, QString &password){

    QFile file("users.us");

    if(!file.exists()){
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "Error";
            return;
        } else {
            QDataStream in(&file);
            users.clear();
            in.setVersion(QDataStream::Qt_5_9);
            in >> users;
        }

        file.flush();
        file.close();
    }

    if(!file.open(QIODevice::WriteOnly)){
        ui->message->setText("Something went wrong...");
        return;
    } else {
        if(!users.contains(username)){
            users.insert(username, password);
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_9);
            out << users;
            ui->message->setText("New user added!");
        } else {
            ui->message->setText("User already exists...");
        }
    }

    file.flush();
    file.close();
}

void addUser::on_cancel_clicked()
{
    this->close();
}
