#-------------------------------------------------
#
# Project created by QtCreator 2016-02-06T17:01:44
#
#-------------------------------------------------

QT       += core gui network
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = quad_send
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    quad_board.cpp \
    capture.cpp \
    plot_opencv.cpp

HEADERS  += mainwindow.h \
    quad_board.h \
    capture.h \
    plot_opencv.h

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`
