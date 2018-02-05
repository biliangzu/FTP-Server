#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include <QFile>
#include <QMap>

namespace Ui {
class addUser;
}

class addUser : public QDialog
{
    Q_OBJECT

public:
    explicit addUser(QWidget *parent = 0);
    ~addUser();

private slots:
    void on_addUserButton_clicked();
    void on_cancel_clicked();

private:
    Ui::addUser *ui;
    void add(QString &username, QString &password);
    void load();
    QMap <QString, QString> users;
};

#endif // ADDUSER_H
