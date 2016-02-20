#include "send_pos.h"

Send_Pos::Send_Pos(QObject *parent) : QObject(parent)
{

    Quad_port = 4321;
    Quad_IP = new QHostAddress("192.168.1.118");
    send_pos_socket = new QUdpSocket(this);

    dataTimer = new QTimer(this);
    dataTimer->start(20);

    connect(dataTimer,SIGNAL(timeout()),this,SLOT(dataTimerEvent()));
}

Send_Pos::~Send_Pos()
{

}



void Send_Pos::dataTimerEvent()
{
//    qDebug() << "data";

//    data_=quadStatusPacketGen(5,6,7);
//    send_pos_socket->writeDatagram(data_, *Quad_IP, Quad_port);
//    send_pos_socket->flush();
//    data_.clear();

    if (data_ready==1)
    {
//        qDebug() << x_ << y_ << height_;
        data_ready=0;
        data_=quadStatusPacketGen(x_,y_,height_);
        send_pos_socket->writeDatagram(data_, *Quad_IP, Quad_port);
        send_pos_socket->flush();
        data_.clear();
    }

    //cv::imshow("view", frame_);

}

QByteArray Send_Pos::quadStatusPacketGen(float x,float y,float height)
{
    QByteArray datagram;
    QDataStream SendDataPacket(&datagram,QIODevice::ReadWrite);
    QByteArray StartPacket = "St";
    //QByteArray EndPacket = "En";


    SendDataPacket << StartPacket;
    SendDataPacket << x;
    SendDataPacket << y;
    SendDataPacket << height;
    //SendDataPacket << IP;
    //SendDataPacket << EndPacket;
    //    qDebug() << "quadpacket: " << SendDataPacket << ", " << datagram;
    //    return datagram;

    return datagram;

}

void Send_Pos::chatterCallback(const geometry_msgs::Point::ConstPtr& msg)
{
    data_ready=1;

    x_=100*msg->x;
    height_=-100*msg->y;
    y_=100*msg->z;


}



