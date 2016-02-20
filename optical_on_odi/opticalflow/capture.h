#ifndef CAPTURE_H
#define CAPTURE_H
#include <QObject>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QThread>


//using namespace std;
//using namespace cv;

#define CamNum 0
#define _VIDEO_FPS              60

#define _FRAME_WIDTH            320//640
#define _FRAME_HEIGTH           240//480

class capture: public QObject
{
    Q_OBJECT
public:
     capture(QObject *parent = 0);
    ~capture();

    int get_frame_status();
    void set_frame_status(int status);

    //VideoCapture cap;
    cv::Mat frame;
    cv::Mat gray_frame;
    bool frame_status;
    //int count=0;
    //QTimer *main_timer;
    cv::VideoCapture cap;


private:

    QTimer *main_timer;

public slots:
    void timerEvent();
};

#endif // CAPTURE_H
