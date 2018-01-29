#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new Server(this);
    ui->connectedUsersTable->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->logTab->removeTab(1);
    ui->usersTab->removeTab(1);

    connect(server, SIGNAL(sendToLog(QString)), this, SLOT(appendLog(QString)));
    connect(server, SIGNAL(sendToTable(QString, QString)), this, SLOT(appendTable(QString, QString)));
    connect(server, SIGNAL(removeFromTableSignal(int)), this, SLOT(removeFromTable(int)));
    connect(this, SIGNAL(deleteUser(int)), server, SLOT(deleteUserServer(int)));

    connect(ui->connectedUsersTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(deleteUserMenu(const QPoint &)));

    connect(this, SIGNAL(deleteUser(int)), server, SLOT(deleteUserServer(int)));

    ui->actionClose_connection->setEnabled(false);
    ui->connectedUsersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}

void MainWindow::on_actionOpen_connection_triggered()
{
    ui->actionOpen_connection->setEnabled(false);
    ui->actionClose_connection->setEnabled(true);
    ui->log->appendPlainText(log("Starting server!"));

    server->startServer();
}

void MainWindow::on_actionClose_connection_triggered()
{
    ui->actionClose_connection->setEnabled(false);
    ui->actionOpen_connection->setEnabled(true);

    ui->log->appendPlainText(log("Shutting down..."));

    server->stopServer();
    ui->connectedUsersTable->setRowCount(0);
}

void MainWindow::appendLog(QString msg){
    ui->log->appendPlainText(log(msg));
}

void MainWindow::appendTable(QString id, QString ip){
    ui->connectedUsersTable->insertRow(ui->connectedUsersTable->rowCount());
    ui->connectedUsersTable->setItem(ui->connectedUsersTable->rowCount()-1, 0, new QTableWidgetItem(id));
    ui->connectedUsersTable->setItem(ui->connectedUsersTable->rowCount()-1, 2, new QTableWidgetItem(ip));
}

void MainWindow::deleteUserMenu(const QPoint &click){
    QMenu menu(this);
    QAction *u = menu.addAction(tr("Kick user"));
    QAction *a = menu.exec(ui->connectedUsersTable->viewport()->mapToGlobal(click));

    if(a == u){
        int selectedRow = ui->connectedUsersTable->selectionModel()->currentIndex().row();
        int selectedId = ui->connectedUsersTable->item(selectedRow, 0)->text().toInt();
        emit deleteUser(selectedId);
        ui->connectedUsersTable->removeRow(selectedRow);
    }
}

void MainWindow::removeFromTable(int id){
    for(int i = 0; i < ui->connectedUsersTable->rowCount(); i++){
        if(ui->connectedUsersTable->item(i, 0)->text().toInt() == id){
            ui->connectedUsersTable->removeRow(i);
        }
    }
}

void MainWindow::on_actionClear_current_log_triggered()
{
    ui->log->clear();
}

QString MainWindow::log(QString message){
    QDateTime current = QDateTime::currentDateTime();
    return current.toString() + " - [SERVER] > " + message;
}
