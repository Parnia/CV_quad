#include "capture.h"


Capture::Capture(QObject *parent) : QObject(parent)
{

    set_frame_status(0);    

    cap.open(_CAM_NUM);
if (cap.isOpened())
{
    cap.set(CV_CAP_PROP_FPS, _VIDEO_FPS);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, _FRAME_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, _FRAME_HEIGTH);
    cap >> frame;

    img_timer = new QTimer(this);
    connect(img_timer,SIGNAL(timeout()),this,SLOT(timerEvent_cam()));
    img_timer->start(1);
}

}

Capture::~Capture()
{
   delete img_timer;
}

void Capture::timerEvent_cam()
{

    cap >> frame;
    cvtColor(frame,gray_frame,COLOR_BGR2GRAY);
  //  imshow("test",frame);
 //   image = Mat2QImage(frame);
    
 //   set_frame_status(1);
    img_ready=1;
//    qDebug() << img_ready;

}

int Capture::get_frame_status()
{
    return img_ready;
}

void Capture::set_frame_status(int status)
{
    img_ready = status;
}
