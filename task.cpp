#include "task.h"

Task::Task()
{

}

void Task::run(){
    if(command == "MKDIR"){
        qDebug() << "Werkt, task";
    }
}

void Task::setCommand(QString command){
    this->command = command;
}
