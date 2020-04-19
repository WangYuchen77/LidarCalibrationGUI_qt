#-------------------------------------------------
#
# Project created by QtCreator 2020-03-02T16:12:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalibrationLidarofFar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


TEMPLATE = app
#CONFIG += console c++11
CONFIG -= app_bundle
# Mac
INCLUDEPATH += /usr/local/Cellar/opencv/4.1.2/include/opencv4/opencv2/
INCLUDEPATH += /usr/local/Cellar/opencv/4.1.2/include/
INCLUDEPATH += /usr/local/Cellar/opencv/4.1.2/include/opencv4/
LIBS += -L/usr/local/Cellar/opencv/4.1.2/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
# Ubuntu
#INCLUDEPATH += /usr/local/opencv/
#INCLUDEPATH += /usr/local/opencv/include
#INCLUDEPATH += /usr/local/include/fastrtps
#INCLUDEPATH += /usr/local/include/fastcdr

LIBS += -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lfastrtps -lfastcdr


SOURCES += \
        main.cpp \
        mainwindow.cpp\
        imageprocess.cpp \
    operation.cpp \
    common.cpp \
    include/LaserScanPubSubTypes.cxx \
    include/LaserScan.cxx \
    include/Time.cxx


HEADERS += \
        mainwindow.h \
        imageprocess.h \
    operation.h \
    common.h \
    include/ \
    include/rapidjson

FORMS += \
        mainwindow.ui
