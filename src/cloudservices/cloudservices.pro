TARGET   = QtCloudServices
QT      += network websockets qml
QT      -= gui
DEFINES += QTCLOUDSERVICES_LIBRARY
MODULE   = qtcloudservices

QMAKE_DOCS = $$PWD/doc/qtcloudservices.qdocconf
OTHER_FILES += \
    doc/qtcloudservices.qdocconf \
    doc/qtcloudservices_client.qdoc

include(../src.pri)

load(qt_module)

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
    qenginiouser.cpp \
    qenginiomodelnode.cpp \
    qmanagedwebsocket.cpp

HEADERS += \
    stdafx.h \
    chunkdevice_p.h \
    enginiobackendconnection_p.h \
    enginiodummyreply_p.h \
    enginiofakereply_p.h \
    enginioobjectadaptor_p.h \
    qcloudservicesconstants_p.h \
    qcloudservicesobject_p.h \
    qenginiocollection_p.h \
    qenginioconnection_p.h \
    qenginiodatastorage_p.h \
    qenginiooperation_p.h \
    qenginioquery_p.h \
    qenginiorequest_p.h \
    qenginiomodel_p.h \
    qenginioobject_p.h \
    qenginiouser_p.h \
    qenginiomodelnode_p.h \
    qmanagedwebsocket_p.h \
    \
    qtcloudservices.h\
    qtcloudservices_global.h \
    qcloudservicesobject.h \
    qenginioconnection.h \
    qenginiocollection.h \
    qenginiodatastorage.h \
    qenginiooperation.h \
    qenginioquery.h \
    qenginiorequest.h \
    qenginiomodel.h \
    qenginioobject.h \
    qenginiouser.h \
    qenginiomodelnode.h \
    qmanagedwebsocket.h
