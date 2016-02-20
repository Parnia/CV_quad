#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QDebug>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QImage>
#include <QBuffer>
#include <QImageWriter>
#include <stdio.h>
#include "quad_board.h"
#include "capture.h"
#include "plot_opencv.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ReadRecPacket(QByteArray data);

    Plot_opencv plot1;
    std::vector<double> plot_datas;

    QUdpSocket *send_img_socket;
    QHostAddress *GS_IP;
    int GS_img_port;

    QByteArray data;

    Quad_Board *transmit;
    Capture capture;

    int data_ready;

    QUdpSocket *rec_data_socket;
    int data_port;

    float x_;
    float height_;
    float y_;

    QImage image;

    Mat mat_test;


    QImage Mat2QImage(Mat const& src)  //GRAY
    {
        Mat temp; // make the same cv::Mat
        cvtColor(src, temp, COLOR_BGR2GRAY); // cvtColor Makes a copt, that what i need
        //src.copyTo(temp);
        QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Grayscale8);
        QImage dest2(dest);
        dest2.detach(); // enforce deep copy
        return dest2;
    }




public slots:
    void timerEvent();
    void receive_data(void);

private:
    Ui::MainWindow *ui;
    QTimer *data_timer;
};

#endif // MAINWINDOW_H
