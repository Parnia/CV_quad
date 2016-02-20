#include "quad_board.h"



Quad_Board::Quad_Board(QObject *parent) : QObject(parent)
{

    serial_port = new QSerialPort(this);
    serial_port->close();
    serial_port->setPortName(_DEV_SERIAL);
    serial_port->open(QIODevice::ReadWrite);
    serial_port->setBaudRate(QSerialPort::Baud115200);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->setDataTerminalReady(0);
    serial_port->setRequestToSend(0);
    ready = false;
    decode_stage = 0;
    sum = 0;
    integer_received_counter = 0;
    Num = 0;
    if( serial_port->isOpen())
    {
        qDebug("Serial Port Opened  .");
    }
    else
    {
        qDebug("Serial Port NOT Opened!!!!!!!");

    }

}

Quad_Board::~Quad_Board()
{

}

void Quad_Board::decode(uchar data)
{


}

void Quad_Board::Empty_Data()
{
    Num = 0;
    check_Sum = 0;
    data_send.clear();
    data_send.append(QByteArray::fromHex("FF"));
    data_send.append(QByteArray::fromHex("FF"));

}

void Quad_Board::Fill_Data(uchar num,...)
{
    va_list arguments;
    va_start( arguments, num );

    Empty_Data();
    data_send.append(((num * 2) + 3));
    data_send.append(Num);
    for(int i = 0; i < num; i++)
    {
        conv.real=(uint)va_arg( arguments, int );
       // qDebug()<<conv.real;
        check_Sum = check_Sum + (uchar)conv.byte[0] + (uchar)conv.byte[1];
        data_send.append(conv.byte[0]);
        data_send.append(conv.byte[1]);
    }
    Send_Data();

    va_end ( arguments );

}

void Quad_Board::Send_Data()
{
    data_send.append((~(check_Sum) + 1));
    serial_port->write(data_send);
    //qDebug()<<data_send.toHex();
    serial_port->flush();

}

/*#include "opticalflow.h"
#include "QDebug"

opticalflow::opticalflow(QObject *parent) : QObject(parent)
{
    initFlag=1;
    numOfFeatures=0;
    timer = new QTimer (this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerEvent2()));

#ifdef _Serial
    transmiter = new Quad_Board();
#endif

    timer->start(2);


}

void opticalflow::timerEvent2()
{
    if (cap.get_frame_status()==1)

    {
        cap.set_frame_status(0);
        cap.frame.copyTo(colorframe);

        trackFeature();
//        #ifdef _Show
//            imshow("frame",colorframe);
//        #endif

//            if (cv::waitKey(1)>0) exit(0);

    }
}


void opticalflow::trackFeature()
{
    cv::cvtColor(colorframe,grayframe,cv::COLOR_BGR2GRAY);

    if (initFlag)
    {
        cv::goodFeaturesToTrack(grayframe,Features[1],MaxCornerNum,0.01,10,Mat(), 3, 0, 0.04);
        /// Calculate the refined corner locations
        cv::cornerSubPix(grayframe,Features[1], subPix_winSize, zeroZone, criteria );
        Features[0]=Features[1];

        if(prvframe.empty())
            grayframe.copyTo(prvframe);

        initFlag=0;
        fprintf(stderr,"******init\n");
    }

    if (Features[1].size()>featureNum_thresh)
    {

        cv::calcOpticalFlowPyrLK(prvframe,grayframe,Features[0],Features[1],status,err,optFlow_winSize,
                3, criteria, 0, 0.001);

        //old_points=Features[0];
        //new_points=Features[1];

        delta_x=delta_y=0;

        numOfFeatures=0;
        for (unsigned int i=0;i<status.size();i++)
        {
            if (status[i])
            {

                //Features[0][numOfFeatures]=Features[0][i];
                Features[1][numOfFeatures++]=Features[1][i];
                Point2f setPoint=Features[0][i],point=Features[1][i];
                delta_x+=(setPoint.x-point.x);
                delta_y+=(setPoint.y-point.y);

#ifdef _Show
                cv::arrowedLine(colorframe, Features[0][i], Features[1][i], Scalar(150,255,0),4);
#endif
            }
        }

        delta_x/=numOfFeatures;
        delta_y/=numOfFeatures;

#ifdef _Serial
        transmiter->Fill_Data(3,(int)(delta_x *10),(int)(delta_y *10),255);
        //  transmiter->Fill_Data(2,3,3);
#endif

        //fprintf(stderr,"delta_x:%f delta_y:%f\n",delta_x,delta_y);

        Features[1].resize(numOfFeatures);
        //Features[0].resize(numOfFeatures);


        /// Calculate the refined corner locations
        if (!Features[1].empty())
            cv::cornerSubPix(grayframe,Features[1], subPix_winSize, zeroZone, criteria );

        //        fprintf(stderr,"%ld\n",Features[1].size());

        //********************************************************************************
        //        if (Features[1].size()>2)
        //            H = cv::findHomography(Features[0],Features[1],RANSAC,4,match_mask);

        //        fprintf(stderr,"masknum: %d\n",countNonZero(match_mask));

        //        int num=0; //chera ino ezafe mikonam bad mishe?? daghighan kudum ahazf??
        //        for (i=0;i<match_mask.size();i++)
        //        {
        //            if (match_mask[i])
        //            {
        //                Features[1][num++]=Features[1][i];
        //            }
        //        }
        //        Features[1].resize(num);

        //        if (countNonZero(match_mask) > featureNum_thresh/2)
        //        {
        //            fprintf(stderr,"**mask ok\n");
        //            cv::perspectiveTransform(old_points,new_points,H);
        //            fprintf(stderr,"Features[1]size: %ld\n",Features[1].size());

        //            Features[0].resize(new_points.size());
        //            Features[1].resize(new_points.size());
        //            Features[1] = new_points;

        //            for(unsigned int k = 0;k<new_points.size();k++)
        //                cv::arrowedLine(colorframe, old_points[k], new_points[k], Scalar(150,255,0),2);
        //        }

        //        else
        //            initFlag=1;
        //***************************************************************************

//        grayframe.copyTo(prvframe);
//        Features[0]=Features[1];

    }

    else
    {
        initFlag=1;
        fprintf(stderr,"initflag\n");
    }

    grayframe.copyTo(prvframe);
    Features[0]=Features[1];

#ifdef _Show
    imshow("frame",colorframe);
#endif

    if (cv::waitKey(1)>0) exit(0);



}
*/



