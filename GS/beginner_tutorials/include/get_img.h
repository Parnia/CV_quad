#ifndef GET_IMG_H
#define GET_IMG_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QDebug>
#include <QUdpSocket>
#include <qimage.h>
#include <ros/ros.h>
#include <QTimer>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer

using namespace cv;

class Get_Img : public QObject
{
    Q_OBJECT
public:
    explicit Get_Img(QObject *parent = 0);
    ~Get_Img();

    void imageCallback(const sensor_msgs::ImageConstPtr& msg);

    QUdpSocket *rec_img_socket;
    int img_port;
    QByteArray datagram;

    Mat main_frame;
    QImage image;

    QTimer *Maintimer;

    int img_ready;

    ros::NodeHandle nh;
    image_transport::Publisher pub;


    sensor_msgs::ImagePtr msg;

    Mat dec_mat;
    cv::Mat frame_;

    double exec_time;



    Mat QImage2Mat(QImage src)
    {
        //QImage myImage=src;
        Mat tmp(src.height(),src.width(),CV_8UC1,src.scanLine(0)); //GRAY
        Mat MatOut(src.height(),src.width(),CV_8UC1); //GRAY

        //cvtColor(tmp, MatOut, COLOR_RGBA2RGB);
        tmp.copyTo(MatOut);
        return MatOut;
    }



public slots:
    void receive_img(void);

protected slots:
    void MaintimerEvent(void);

};


#endif // GET_IMG_H

