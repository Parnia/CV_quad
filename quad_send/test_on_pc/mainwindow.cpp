#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    plot_datas.resize(3);
    transmit = new Quad_Board();

    GS_IP = new QHostAddress("127.0.0.1");
    send_img_socket = new QUdpSocket(this);
    GS_img_port=1234;

    data_port=4321;
    rec_data_socket = new QUdpSocket(this);
    rec_data_socket->bind(data_port);
    connect(rec_data_socket,SIGNAL(readyRead()),this,SLOT(receive_data()));


    data_timer = new QTimer(this);
    connect(data_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
    data_timer->start(5);

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
//imshow("test2",capture.frame);

        image = Mat2QImage(capture.frame); //& GRAY

  //      qDebug() << image.size() << "good";

        QBuffer buff;
        QImageWriter writer(&buff, "JPEG");
        writer.setQuality(80);
        writer.write(image);
        data = buff.data();

//        writer.setDevice(&buffer);
//        writer.setFormat("JPEG");
//        writer.setCompression(9); quality?

//        unsigned int size=capture.enc_buff.size();
//        data.resize(size);
//        for(unsigned int i=0; i<size; i++)
//        data[i]=(unsigned char)capture.enc_buff[i];


//        std::vector<uchar> bufferToCompress(data.begin(), data.end());
//        imdecode(bufferToCompress,CV_LOAD_IMAGE_COLOR,&capture.dec_mat);
//        imshow("decoded",capture.dec_mat);



        send_img_socket->writeDatagram(data, *GS_IP, GS_img_port);
        send_img_socket->flush();
        data.clear();
//        qDebug() << "ok";
    }

    if (data_ready==1)
    {
        data_ready=0;
        //transmit->Fill_Data(3,(float)x_,(float)y_,(float)height_);
        //qDebug() << x_ << y_ << height_;
        plot_datas[0] = x_;  //red
        plot_datas[1] = y_;  //blue
        plot_datas[2] = height_;  //green
        plot1.Plot("x y h", -60, 60, plot_datas);
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
