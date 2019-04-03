#-------------------------------------------------
#
# Project created by QtCreator 2018-06-20T10:14:04
#
#-------------------------------------------------

QT       += core gui

# for CAN wrapper
CONFIG += c++11
# for the raspicam
INCLUDEPATH += /usr/local/include/raspicam
# for the GPIO
INCLUDEPATH += /home/laris/wiringPi/wiringPi
LIBS += -L /usr/local/lib -lraspicam -lwiringPi -lcrypt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Poodle_demo
TEMPLATE = app


SOURCES += main.cpp\
        poodle_window.cpp \
    canwrapper.cpp \
    clickablelabel.cpp \
    seriallens.cpp \
    motor_canopen_driver.cpp \
    poodlecamera.cpp \
    log_handler.cpp

HEADERS  += poodle_window.h \
    canwrapper.h \
    clickablelabel.h \
    seriallens.h \
    motor_canopen_driver.h \
    poodlecamera.h \
    log_handler.h

FORMS    += poodle_window.ui
