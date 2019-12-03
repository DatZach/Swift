#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
