#-------------------------------------------------
#
# Project created by QtCreator 2014-05-30T11:05:12
#
#-------------------------------------------------

QT += core network gui widgets cloudservices

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCloudConsole
TEMPLATE = app

gcc*: {
    QMAKE_CXXFLAGS += -std=c++11
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    cloudconsole.qrc
