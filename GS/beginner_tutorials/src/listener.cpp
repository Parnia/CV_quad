#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "QtExtSerialPort/qextserialport.h"
#include "quad_board.h"
#include "get_img.h"
#include <QApplication>
#include <QTimer>
#include "send_pos.h"
#include <QThread>
//#include <X11/Xlib.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    XInitThreads();

    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    ros::NodeHandle n_;

    Send_Pos sendPos;
    ros::Subscriber sub = n.subscribe("/ORB_SLAM/Par", 1000, &Send_Pos::chatterCallback, &sendPos);

//    ros::NodeHandle nh_;


    Get_Img getImg;
//    image_transport::ImageTransport it_(n_);
//    image_transport::Subscriber sub_ = it_.subscribe("camera/image_raw", 1, &Get_Img::imageCallback,&getImg);

    QThread thr1,thr2;

//    getImg.moveToThread(&thr1);
//    sendPos.moveToThread(&thr2);

//    thr1.start();
//    thr2.start();

    //qDebug("thread");





    return a.exec();
}
