#include "mainwindow.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    QThread thr1,thr2;

    w.capture.moveToThread(&thr1);
    thr1.start();


    return a.exec();
}
