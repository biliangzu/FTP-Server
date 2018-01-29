#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDateTime>

#include "server.h"


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
    void appendLog(QString);
    void appendTable(QString, QString);
    void removeFromTable(int);
    void on_actionClear_current_log_triggered();

signals:
    void deleteUser(int);

private:
    Ui::MainWindow *ui;
    Server *server;
    QString log(QString message);

};

#endif // MAINWINDOW_H
