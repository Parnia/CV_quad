#include "get_img.h"

Get_Img::Get_Img(QObject *parent) : QObject(parent)
{

    image_transport::ImageTransport it(nh);
    pub = it.advertise("camera/image_raw", 1);

    img_ready=0;

    img_port=1234;
    rec_img_socket = new QUdpSocket(this);
    rec_img_socket->bind(img_port);
    connect(rec_img_socket,SIGNAL(readyRead()),this,SLOT(receive_img()));

    //qDebug() << "iniiiiiiit";
    //qDebug() << check;

    Maintimer = new QTimer(this);
    Maintimer->start(1);
    connect(Maintimer,SIGNAL(timeout()),this,SLOT(MaintimerEvent()));


}

Get_Img::~Get_Img()
{
    //qDebug() << "destroy";
}

void Get_Img::receive_img()
{

//    qDebug() << "recccccccccc";
    datagram.resize(rec_img_socket->pendingDatagramSize());
    rec_img_socket->readDatagram(datagram.data(), datagram.size());
    rec_img_socket->flush();
    img_ready=1;

    //    waitKey(1);
    //    datagram.clear();
}

void Get_Img::MaintimerEvent()
{
//qDebug() << "timer";
    if (img_ready==1)
    {
//        qDebug() << "rec";
        img_ready=0;

        //        image.loadFromData(datagram);
//        main_frame=QImage2Mat(image);  //GRAY

        std::vector<uchar> bufferToCompress(datagram.begin(), datagram.end());
        dec_mat=imdecode(bufferToCompress,CV_LOAD_IMAGE_GRAYSCALE);
       // imshow("received",dec_mat);

        //imshow("testtttttt",main_frame);
        //exec_time = (double)getTickCount();
        //qDebug() << exec_time << "1";
        msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", dec_mat).toImageMsg();
        pub.publish(msg);


    }
    //qDebug() << "rec";
    ros::spinOnce();

}

void Get_Img::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
//    qDebug() << "subs";

    frame_=cv_bridge::toCvShare(msg, "mono8")->image;

    //    double exec_time = (double)getTickCount();
    //      qDebug() << "aaaaaaaaaa";
        exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
        qDebug() << "tick" << exec_time;


    cv::imshow("view", frame_);
//    cv::waitKey(2);
//    try
//    {
//      cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
//      cv::waitKey(30);
//    }
//    catch (cv_bridge::Exception& e)
//    {
//      ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
//    }
}
