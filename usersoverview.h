#ifndef USERSOVERVIEW_H
#define USERSOVERVIEW_H

#include <QMainWindow>
#include <QFile>
#include <QMap>
#include <QDebug>

namespace Ui {
class UsersOverview;
}

class UsersOverview : public QMainWindow
{
    Q_OBJECT

public:
    explicit UsersOverview(QWidget *parent = 0);
    ~UsersOverview();

private:
    Ui::UsersOverview *ui;

private slots:
    void getUsers();
};

#endif // USERSOVERVIEW_H
