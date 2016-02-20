#ifndef CAPTURE_H
#define CAPTURE_H

#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QBuffer>
#include <QImageWriter>

#define _CAM_NUM                1
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

    Mat frame, frame2,yuv;
    Mat channel[3];
    Mat gray_frame;
    bool img_ready;

    std::vector<uchar> enc_buff;
    std::vector<int> compression_params;
    Mat dec_mat;

    QImage Mat2QImage(Mat const& src)  //GRAY
    {
        Mat temp; // make the same cv::Mat
        cvtColor(src, temp, COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
        //src.copyTo(temp);
        QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        QImage dest2(dest);
        dest2.detach(); // enforce deep copy
        return dest2;
    }

    QImage Mat2QImage2(Mat const& src)  //GRAY
    {
        Mat temp; // make the same cv::Mat
        cvtColor(src, temp, COLOR_BGR2GRAY); // cvtColor Makes a copt, that what i need
        //src.copyTo(temp);
        QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Indexed8);
        QImage dest2(dest);
        dest2.detach(); // enforce deep copy
        return dest2;
    }

    QImage Mat2QImage3(Mat const& src)  //GRAY
    {
        Mat temp; // make the same cv::Mat
        cvtColor(src, temp, COLOR_BGR2GRAY); // cvtColor Makes a copt, that what i need
        //src.copyTo(temp);
        QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Grayscale8);
        QImage dest2(dest);
        dest2.detach(); // enforce deep copy
        return dest2;
    }

    QImage Mat2QImage4(Mat const& src)  //GRAY
    {
        Mat temp; // make the same cv::Mat
        //cvtColor(src, temp, COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
        src.copyTo(temp);
        QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Grayscale8);
        QImage dest2(dest);
        dest2.detach(); // enforce deep copy
        return dest2;
    }

    Mat QImage2Mat(QImage src)
    {
        //QImage myImage=src;
        Mat tmp(src.height(),src.width(),CV_8UC4,src.scanLine(0));
        Mat MatOut(src.height(),src.width(),CV_8UC3);

        //cvtColor(tmp, MatOut, COLOR_RGBA2RGB);
        tmp.copyTo(MatOut);
        return MatOut;
    }


    QImage mat_test;
    QImage mat_test2;
    QImage mat_test3;
    QImage mat_test4,mat_test5;
    QImage image;
    QByteArray data,data2,data3,data4,data5,data6;

    Mat rgb_dec;

    //QBuffer buff2;
    QImageWriter writer2;

public slots:
    void timerEvent_cam();

    
private:
    VideoCapture cap;
    QTimer *img_timer;


};

//640 * 480 / RGB:  13158
//640 * 480 / MONO: 9549
//320 * 240 / RGB:  5386
//320 * 240 / MONO: 4673


#endif // CAPTURE_H
