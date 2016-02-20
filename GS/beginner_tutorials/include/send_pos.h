#ifndef SEND_POS_H
#define SEND_POS_H

#include <QMainWindow>
#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QTimer>
#include <stdio.h>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "sensor_msgs/Image.h"
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QUdpSocket>
#include <qimage.h>
#include <ros/ros.h>
#include <QTimer>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer


class Send_Pos : public QObject
{
    Q_OBJECT
public:
    explicit Send_Pos(QObject *parent = 0);
    ~Send_Pos();

    QByteArray quadStatusPacketGen(float x,float y,float height);
    void chatterCallback(const geometry_msgs::Point::ConstPtr& msg);


    QTimer *dataTimer;

    QHostAddress *Quad_IP;
    QUdpSocket *send_pos_socket;
    QByteArray data_;
    int Quad_port;

    float x_;
    float height_;
    float y_;

    int data_ready;



protected slots:
    void dataTimerEvent(void);

};


#endif // SEND_POS_H

