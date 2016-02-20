#ifndef CAPTURE_H
#define CAPTURE_H

#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <QTimer>

#define _CAM_NUM                0
#define _VIDEO_FPS              30

#define _FRAME_WIDTH           640
#define _FRAME_HEIGTH          480

using namespace cv;

class Capture : public QObject
{
    Q_OBJECT
public:
    explicit Capture(QObject *parent = 0);
    ~Capture();

    int get_frame_status();
    void set_frame_status(int status);

    Mat frame;
    Mat gray_frame;
//    QImage image;

    int img_ready;

 /*   QImage Mat2QImage(Mat const& src)
    {
        Mat temp; // make the same cv::Mat
        cvtColor(src, temp, COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
        QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        QImage dest2(dest);
        dest2.detach(); // enforce deep copy
        return dest2;
    }
*/


public slots:
    void timerEvent_cam();

    
private:
    VideoCapture cap;
    QTimer *img_timer;


};




#endif // CAPTURE_H
