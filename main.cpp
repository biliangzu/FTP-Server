#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/icons/icons/servericon.png"));
    w.show();

    QCoreApplication::setOrganizationName("Moor");
    QCoreApplication::setOrganizationDomain("jmoor.com");
    QCoreApplication::setApplicationName("JussiesFTPServer");

    return a.exec();
}
