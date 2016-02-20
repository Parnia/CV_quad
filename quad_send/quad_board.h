#ifndef QUAD_BOARD_H
#define QUAD_BOARD_H


#include <QObject>
#include "QtSerialPort/QSerialPort"
#include "QtSerialPort/QSerialPortInfo"
#include "qdebug.h"

#define _DEV_SERIAL            "ttyUSB0"
//"/dev/ttyUSB0"
#define MPC_BUFF_AMOUNT  11


class Quad_Board : public QObject
{
    Q_OBJECT
public:
    explicit Quad_Board(QObject *parent = 0);
    ~Quad_Board();

    //void decode(uchar data);

    union _ch2int
        {
            int16_t real;
            char byte[2];
        };


    typedef union _ch2int  ch2int;



    void Mpc_decode(u_int8_t _data);
    void Empty_Data();
    void Fill_Data(uchar num , ... );
    void Send_Data();

public:

    bool ready;
    char decode_stage;
    char sum;
    char received_packet_length;
    int integer_received_counter;
    int data_num;
    int data_recieved[32];
    int16_t data[32];
    int j;
    int Len;
    int pack_started;
    QByteArray data_send;
    uchar Num;
    uchar check_Sum;
    QSerialPort *serial_port;
    ch2int conv;

public slots:
    void readData();




};




#endif // QUAD_BOARD_H
