#include "usersoverview.h"
#include "ui_usersoverview.h"


UsersOverview::UsersOverview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UsersOverview)
{
    ui->setupUi(this);
    ui->usersTab->removeTab(1);
    ui->userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

UsersOverview::~UsersOverview()
{
    delete ui;
}

void UsersOverview::getUsers(){
    ui->userTable->setRowCount(0);

    QFile file("users.us");
    QMap <QString, QString> users;

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
            file.close();
        }
    }

    QMap<QString, QString>::const_iterator i = users.constBegin();
    while( i != users.constEnd()){
        ui->userTable->insertRow(ui->userTable->rowCount());
        ui->userTable->setItem(ui->userTable->rowCount()-1, 0, new QTableWidgetItem(i.key()));
        ui->userTable->setItem(ui->userTable->rowCount()-1, 1, new QTableWidgetItem(i.value()));
        ++i;
    }
}
