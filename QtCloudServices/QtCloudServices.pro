#-------------------------------------------------
#
# Project created by QtCreator 2014-05-26T18:28:30
#
#-------------------------------------------------

include(../QtCloudServices.pri)

QT       += network
QT       -= gui

TARGET = QtCloudServices
TEMPLATE = lib
MODULE = qtcloudservices
DEFINES += QTCLOUDSERVICES_LIBRARY

SOURCES += \
    enginiobackendconnection.cpp \
    enginiodummyreply.cpp \
    enginiofakereply.cpp \
    qcloudservicesobject.cpp \
    qenginiocollection.cpp \
    qenginioconnection.cpp \
    qenginiodatastorage.cpp \
    qenginiooperation.cpp \
    qenginioquery.cpp \
    qenginiorequest.cpp \
    qcloudservicesconstants.cpp \
    qenginiomodel.cpp \
    qenginioobject.cpp \
    qenginiouser.cpp

HEADERS += qtcloudservices.h\
        qtcloudservices_global.h \
    enginiobasemodel.h \
    enginiomodel.h \
    qcloudservicesobject.h \
    qcollectionobject.h \
    qenginiocollection.h \
    qenginioconnection.h \
    qenginiodatastorage.h \
    qenginiooperation.h \
    qenginioquery.h \
    qenginiorequest.h \
    private/chunkdevice_p.h \
    private/enginiobackendconnection_p.h \
    private/enginiobasemodel_p.h \
    private/enginiodummyreply_p.h \
    private/enginiofakereply_p.h \
    private/enginioobjectadaptor_p.h \
    private/qcloudservicesconstants_p.h \
    private/qcloudservicesobject_p.h \
    private/qenginiocollection_p.h \
    private/qenginioconnection_p.h \
    private/qenginiodatastorage_p.h \
    private/qenginiooperation_p.h \
    private/qenginioquery_p.h \
    private/qenginiorequest_p.h \
    stdafx.h \
    private/qenginiomodel_p.h \
    private/qenginioobject_p.h \
    private/qenginiouser_p.h \
    qenginiomodel.h \
    qenginioobject.h \
    qenginiouser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=