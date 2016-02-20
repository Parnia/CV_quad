#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(80);

    transmit = new Quad_Board();

    capture.set_frame_status(0);

    GS_IP = new QHostAddress("192.168.1.109");
    send_img_socket = new QUdpSocket(this);
    GS_img_port=1234;

    data_port=4321;
    rec_data_socket = new QUdpSocket(this);
    rec_data_socket->bind(data_port);
    connect(rec_data_socket,SIGNAL(readyRead()),this,SLOT(receive_data()));


    data_timer = new QTimer(this);
    connect(data_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    data_timer->start(2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent()
{

    if (capture.get_frame_status()==1)
    {
      

       capture.set_frame_status(0);

        /*image = Mat2QImage(capture.frame);
        QBuffer buff;
        QImageWriter writer(&buff, "JPEG");
        writer.setQuality(80);        
        writer.write(image);
        data = buff.data();*/

        //cvtColor(capture.frame,gray_frame,COLOR_BGR2GRAY);
        imencode(".jpg",capture.gray_frame,enc_buff,compression_params);
        unsigned int size=enc_buff.size();
        data.resize(size);
        for(unsigned int i=0; i<size; i++)
        data[i]=(unsigned char)enc_buff[i];


        
        send_img_socket->writeDatagram(data, *GS_IP, GS_img_port);
        send_img_socket->flush();
        data.clear();
    }   

    if (data_ready==1)
    {
        data_ready=0;
        
        int x=100*x_;
        int y=100*y_;
        int h=100*height_;

        transmit->Fill_Data(3,(int)x,(int)y,(int)h);
        qDebug() << y;
    }    

}



void MainWindow::receive_data()
{
    //qDebug() << "rec";  //chera vaymiste?
    QByteArray datagram;
    datagram.resize(rec_data_socket->pendingDatagramSize());
    rec_data_socket->readDatagram(datagram.data(), datagram.size());
    rec_data_socket->flush();
    ReadRecPacket(datagram);
    datagram.clear();
}

void MainWindow::ReadRecPacket(QByteArray data)
{

    QByteArray datagram,StartPacket;

    datagram=data;
    QDataStream RecDataStream(&datagram , QIODevice::ReadOnly);

    RecDataStream >> StartPacket;

    if( QString::compare(StartPacket,"St") == 0 )
    {
        data_ready=1;

        RecDataStream >> x_;
        RecDataStream >> y_;
        RecDataStream >> height_;
    }


    datagram.clear();

}
