#include "capture.h"

capture::capture(QObject *parent) : QObject(parent)
{
    cap.open(CamNum);
    if( cap.isOpened() )
    {
        cap.set(CV_CAP_PROP_FPS, _VIDEO_FPS);
        cap.set(CV_CAP_PROP_FRAME_WIDTH, _FRAME_WIDTH);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, _FRAME_HEIGTH);
        cap >> frame;


        main_timer = new QTimer(this);
        connect(main_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
        main_timer->start(2); //zaman? //chera delay?
    }
    else
    {
        fprintf(stderr,"could not open cam\n");
        exit(0);
    }
}

void capture::timerEvent()
{
    cap >> frame;
    frame_status = 1;
    cv::cvtColor(frame,gray_frame,cv::COLOR_BGR2GRAY);
    //imshow("frame",frame);
    //    count++;


}

int capture::get_frame_status()
{
    return frame_status;
}

void capture::set_frame_status(int status)
{
    frame_status = status;
}

capture::~capture()
{
    delete main_timer;
}

