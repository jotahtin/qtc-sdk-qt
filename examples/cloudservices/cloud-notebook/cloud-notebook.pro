#-------------------------------------------------
#
# Project created by QtCreator 2014-05-30T11:05:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCloudNotebook
TEMPLATE = app

gcc*: {
    QMAKE_CXXFLAGS += -std=c++11
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
