QT += core
QT -= gui
CONFIG += serialport

TARGET = opticalflow
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    capture.cpp \
    opticalflow.cpp \
    quad_board.cpp

LIBS += `pkg-config opencv --libs`

HEADERS += \
    capture.h \
    opticalflow.h \
    quad_board.h
