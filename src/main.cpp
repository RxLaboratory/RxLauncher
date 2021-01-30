#include "mainwindow.h"

#include <QApplication>
#include <QtSvg>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Duduf");
    QCoreApplication::setOrganizationDomain("duduf.com");
    QCoreApplication::setApplicationName("DuLauncher");

    MainWindow w;
    w.show();
    return a.exec();
}
