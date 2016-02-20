#ifndef OPTICALFLOW_H
#define OPTICALFLOW_H

#include <QObject>
#include "capture.h"
#include "quad_board.h"

#define MaxCornerNum 70
#define featureNum_thresh 20

#define subPix_winSize Size( 10, 10 )
#define zeroZone Size( -1, -1 )

#define win_size 15
#define optFlow_winSize Size( win_size, win_size )
#define criteria TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03)

#define _Serial
//#define _Show


using namespace cv;

class opticalflow : public QObject
{
    Q_OBJECT
public:
    explicit opticalflow(QObject *parent = 0);

    Quad_Board *transmiter;
    QTimer *timer;
    capture cap;
    cv::Mat colorframe;
    cv::Mat prvframe;
    size_t numOfFeatures;

    cv::Mat grayframe;
    std::vector<unsigned char> match_mask;

    Mat H;
    std::vector<double> plot_datas;


    float delta_x,delta_y;

    void trackFeature();

private:
    std::vector<cv::Point2f> Features[2],old_points,new_points;
    std::vector<int> test[2];
    std::vector<uchar> status;
    std::vector<float> err;
    uint8_t initFlag;

signals:

public slots:
    void timerEvent2();
};

#endif // OPTICALFLOW_H
