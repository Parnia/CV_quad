#include <QCoreApplication>
#include "opticalflow.h"
#include "capture.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    opticalflow w;

    QThread mtr;
    w.cap.moveToThread(&mtr);
    mtr.start(QThread::HighestPriority);

    return a.exec();
}

