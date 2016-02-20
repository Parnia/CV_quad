#include "capture.h"


Capture::Capture(QObject *parent) : QObject(parent)
{

    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(80);


    writer2.setFormat("JPEG");
    writer2.setQuality(80);

    cap.open(_CAM_NUM);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, _FRAME_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, _FRAME_HEIGTH);
    cap.set(CV_CAP_PROP_FPS, _VIDEO_FPS);

    cap >> frame;

    img_timer = new QTimer(this);
    connect(img_timer,SIGNAL(timeout()),this,SLOT(timerEvent_cam()));
    img_timer->start(100);


}

Capture::~Capture()
{

}

void Capture::timerEvent_cam()
{

//    double exec_time = (double)getTickCount();
//      qDebug() << "aaaaaaaaaa";
//    exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
//    qDebug() << "tick" << exec_time;
    cap >> frame;
    double exec_time3 = (double)getTickCount();
    cvtColor(frame,gray_frame,COLOR_BGR2GRAY);
    exec_time3 = ((double)getTickCount() - exec_time3)*1000./getTickFrequency();
    qDebug() << "tick cvtcolor" << exec_time3;


    cvtColor(gray_frame,frame2,COLOR_GRAY2BGR);

    cvtColor(frame,yuv,COLOR_BGR2YUV);
    split(yuv,channel);


  //  qDebug() << mat_test2.size() << "good";

    mat_test=Mat2QImage2(frame);
    QBuffer buff;
    QImageWriter writer(&buff, "JPEG");
    writer.setQuality(80);
    writer.write(mat_test);
    data = buff.data();

    double exec_time = (double)getTickCount();
    mat_test2=Mat2QImage(frame);
    QBuffer buff2;
//    QImageWriter writer2(&buff2, "JPEG");
//    writer2.setQuality(80);
    writer2.setDevice(&buff2);

    writer2.write(mat_test2);
    data2 = buff2.data();
    exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
    qDebug() << "tick rgb" << exec_time;

    mat_test3=Mat2QImage(frame2);
    QBuffer buff3;
    QImageWriter writer3(&buff3, "JPEG");
    writer3.setQuality(80);
    writer3.write(mat_test3);
    data3 = buff3.data();

    mat_test4=Mat2QImage3(frame);
    QBuffer buff4;
    QImageWriter writer4(&buff4, "JPEG");
    writer4.setQuality(80);
    writer4.write(mat_test4);
    data4 = buff4.data();

    //imshow("y",channel[0]);
    mat_test5=Mat2QImage4(channel[0]);
    QBuffer buff5;
    QImageWriter writer5(&buff5, "JPEG");
    writer5.setQuality(80);
    writer5.write(mat_test5);
    data5 = buff5.data();

    double exec_time2 = (double)getTickCount();
    imencode(".jpg",gray_frame,enc_buff,compression_params);
    unsigned int size=enc_buff.size();
    data6.resize(size);
    for(unsigned int i=0; i<size; i++)
    data6[i]=(unsigned char)enc_buff[i];
    exec_time2 = ((double)getTickCount() - exec_time2)*1000./getTickFrequency();
    qDebug() << "tick encode" << exec_time2;

    double exec_time4 = (double)getTickCount();
    std::vector<uchar> bufferToCompress(data6.begin(), data6.end());
    imdecode(bufferToCompress,CV_LOAD_IMAGE_GRAYSCALE,&dec_mat);
    exec_time4 = ((double)getTickCount() - exec_time4)*1000./getTickFrequency();
    qDebug() << "tick decode*" << exec_time4;
    imshow("decoded",dec_mat);

    double exec_time5 = (double)getTickCount();
    image.loadFromData(data2);
    rgb_dec=QImage2Mat(image);
    exec_time5 = ((double)getTickCount() - exec_time5)*1000./getTickFrequency();
    qDebug() << "tick rgb dec*" << exec_time5;
    imshow("rgb dec",rgb_dec);



    qDebug() << data.size() << "index";
    qDebug() << data2.size() << "rgb";
    qDebug() << data3.size() << "gray>>bgr";
    qDebug() << data4.size() << "gray";
    qDebug() << data5.size() << "channel[0]";
    qDebug() << data6.size() << "encode";
    qDebug() << " ";

    //imshow("main",gray_frame);




//   imdecode(enc_buff,CV_LOAD_IMAGE_GRAYSCALE,&dec_mat);
//    imshow("decoded",dec_mat);

//    qDebug()<<image.isGrayscale();
//    qDebug()<<image.byteCount();
    img_ready=1;

}

int Capture::get_frame_status()
{
    return img_ready;
}

void Capture::set_frame_status(int status)
{
    img_ready = status;
}
