#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDateTime>

#include "server.h"
#include "adduser.h"
#include "usersoverview.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_connection_triggered();
    void on_actionClose_connection_triggered();
    void deleteUserMenu(const QPoint &);
    void appendLogServer(QString);
    void appendLogClient(QString, QString);
    void appendTable(QString, QString, QString);
    void removeFromTable(int);
    void on_actionClear_current_log_triggered();

    void on_actionAdd_user_triggered();
    void on_actionUser_list_triggered();

    void on_actionConfigure_server_triggered();

signals:
    void deleteUser(int);
    void getUsers();

private:
    Ui::MainWindow *ui;
    Server *server;
    QString log(QString message);
    UsersOverview *overview;
    void setStandardSettings();

};

#endif // MAINWINDOW_H
