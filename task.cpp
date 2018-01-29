#include "task.h"

Task::Task()
{

}

void Task::run(){
    if(command == "MKDIR"){
        qDebug() << "Werkt, task";
    } else if (command=="LIST"){
        qDebug() << "List";
    }
}

void Task::setCommand(QString command){
    this->command = command;
}
