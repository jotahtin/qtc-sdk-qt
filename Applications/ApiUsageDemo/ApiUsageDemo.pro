#-------------------------------------------------
#
# Project created by QtCreator 2014-05-28T12:05:49
#
#-------------------------------------------------

include(../../QtCloudServices.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ApiUsageDemo
TEMPLATE = app


SOURCES += main.cpp apiusagedemo.cpp

HEADERS  += apiusagedemo.h

FORMS    += apiusagedemo.ui

CONFIG += mobility
MOBILITY = 


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../QtCloudServices/release/ -lQtCloudServices
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../QtCloudServices/debug/ -lQtCloudServices
else:unix: LIBS += -L$$OUT_PWD/../../QtCloudServices/ -lQtCloudServices

INCLUDEPATH += $$PWD/../../QtCloudServices
DEPENDPATH += $$PWD/../../QtCloudServices
