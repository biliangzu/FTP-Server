#ifndef TASK_H
#define TASK_H

#include<QRunnable>
#include<QObject>
#include<QDebug>

class Task : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Task();

signals:
    void Result();

public:
    void setCommand(QString command);

protected:
    void run();
private:
    QString command;
};

#endif // TASK_H
