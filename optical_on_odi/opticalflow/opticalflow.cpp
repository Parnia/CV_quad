#include "opticalflow.h"
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
        cap.gray_frame.copyTo(grayframe);


        trackFeature();

    }
}


void opticalflow::trackFeature()
{

    if (initFlag)
    {
        cv::goodFeaturesToTrack(grayframe,Features[1],MaxCornerNum,0.01,10,Mat(), 3, 0, 0.04);
        if (!Features[1].empty())
            cv::cornerSubPix(grayframe,Features[1], subPix_winSize, zeroZone, criteria );
        Features[0]=Features[1];

        if(prvframe.empty())
            grayframe.copyTo(prvframe);

        initFlag=0;
#ifdef _Show
        fprintf(stderr,"*init\n");
#endif
    }

    if (Features[1].size()>featureNum_thresh)
    {

        cv::calcOpticalFlowPyrLK(prvframe,grayframe,Features[0],Features[1],status,err,optFlow_winSize,
                3, criteria, 0, 0.001);

        old_points=Features[0];
        new_points=Features[1];

        delta_x=delta_y=0;

        numOfFeatures=0;
        for (unsigned int i=0;i<status.size();i++)
        {
            if (status[i])
            {

                Features[0][numOfFeatures]=Features[0][i];
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

#ifdef _Show
        fprintf(stderr,"delta_x:%f delta_y:%f\n",delta_x,delta_y);
#endif

        Features[1].resize(numOfFeatures);
        Features[0].resize(numOfFeatures);


        //if (!Features[1].empty())
            //cv::cornerSubPix(grayframe,Features[1], subPix_winSize, zeroZone, criteria );

        //********************************************************************************
        if (Features[1].size()>3)
            H = cv::findHomography(Features[0],Features[1],CV_RANSAC,15,match_mask);

        int num=0;
        for (unsigned int i=0;i<match_mask.size();i++)
        {
            if (match_mask[i])
            {
                Features[0][num]=Features[0][i];
                Features[1][num++]=Features[1][i];
            }
        }
        Features[0].resize(num);
        Features[1].resize(num);

//        if (countNonZero(match_mask) > featureNum_thresh/2)
//        {
//            //fprintf(stderr,"**mask ok\n");
//            cv::perspectiveTransform(old_points,new_points,H);
//            //fprintf(stderr,"Features[1]size: %ld\n",Features[1].size());

//            Features[0].resize(new_points.size());
//            Features[1].resize(new_points.size());
//            Features[1] = new_points;

//            //                    for(unsigned int k = 0;k<new_points.size();k++)
//            //                        cv::arrowedLine(colorframe, old_points[k], new_points[k], Scalar(150,255,0),2);
//        }

//        else
//            initFlag=1;
        //***************************************************************************

    }

    else
    {
        initFlag=1;
    }

    grayframe.copyTo(prvframe);
    Features[0]=Features[1];

#ifdef _Show
    imshow("frame",colorframe);
#endif

    if (cv::waitKey(1)>0) exit(0);



}
