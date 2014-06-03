TARGET   = QtCloudServices
QT      += network websockets qml core-private qml-private
QT      -= gui
DEFINES += QTCLOUDSERVICES_LIBRARY
MODULE   = cloudservices

QMAKE_DOCS = $$PWD/doc/qtcloudservices.qdocconf
OTHER_FILES += \
    doc/qtcloudservices.qdocconf \
    doc/qtcloudservices_client.qdoc \
    doc/Notes.txt

include(../src.pri)

load(qt_module)

SOURCES += \
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
    qmanagedwebsocket.cpp \
    qtcloudservices.cpp \
    qenginiodatastorageshared.cpp \
    qenginiodatastorageobject.cpp \
    qenginioconnectionshared.cpp \
    qenginioconnectionobject.cpp \
    qrestendpoint.cpp \
    qrestendpointshared.cpp \
    qrestendpointobject.cpp \
    qrestconnection.cpp \
    qrestconnectionshared.cpp \
    qrestconnectionobject.cpp \
    qrestrequest.cpp \
    qrestrequestshared.cpp \
    qrestrequestobject.cpp \
    qrestoperation.cpp \
    qrestoperationobject.cpp \
    qrestoperationshared.cpp \
    qenginiorequestobject.cpp \
    qenginiorequestshared.cpp \
    qenginiocollectionshared.cpp \
    qenginiocollectionobject.cpp \
    qenginiooperationobject.cpp \
    qenginiooperationshared.cpp \
    qenginioobjectobject.cpp \
    qenginioobjectshared.cpp \
    qenginioqueryobject.cpp \
    qenginioqueryshared.cpp \
    qenginiouserobject.cpp \
    qenginiousershared.cpp \
    qmanagedwebsocketobject.cpp \
    qmanagedwebsocketshared.cpp \
    qmanagedwebsocketconnectionshared.cpp \
    qmanagedwebsocketconnectionobject.cpp \
    qcloudservicessharedobject.cpp

HEADERS += \
    stdafx.h \
    qcloudservicesconstants_p.h \
    qenginiomodel_p.h \
    qenginiomodelnode_p.h \
    \
    qtcloudservices.h\
    qtcloudservices_global.h \
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
    qmanagedwebsocket.h \
    qenginiodatastorageshared_p.h \
    qenginiodatastorageobject.h \
    qenginiodatastorageobject_p.h \
    qenginioconnectionobject.h \
    qrestendpoint.h \
    qrestendpointshared_p.h \
    qrestendpointobject.h \
    qrestendpointobject_p.h \
    qrestconnection.h \
    qrestconnectionobject.h \
    qrestconnectionshared_p.h \
    qrestrequest.h \
    qrestrequestshared_p.h \
    qrestrequestobject.h \
    qrestrequestobject_p.h \
    qrestoperation.h \
    qrestoperationobject.h \
    qrestoperationobject_p.h \
    qrestoperationshared_p.h \
    qenginiorequestobject_p.h \
    qenginiorequestobject.h \
    qenginiorequestshared_p.h \
    qenginiocollectionshared_p.h \
    qenginiocollectionobject.h \
    qenginiocollectionobject_p.h \
    qenginiooperationobject.h \
    qenginiooperationobject_p.h \
    qenginiooperationshared_p.h \
    qenginioconnectionshared_p.h \
    qenginioconnectionobject_p.h \
    qrestconnectionobject_p.h \
    qenginioobjectobject.h \
    qenginioobjectshared_p.h \
    qenginioobjectobject_p.h \
    qenginioqueryobject.h \
    qenginioqueryobject_p.h \
    qenginioqueryshared_p.h \
    qenginiouserobject.h \
    qenginiousershared_p.h \
    qenginiouserobject_p.h \
    qmanagedwebsocketobject.h \
    qmanagedwebsocketobject_p.h \
    qmanagedwebsocketshared_p.h \
    qmanagedwebsocketconnectionshared_p.h \
    qmanagedwebsocketconnectionobject.h \
    qmanagedwebsocketconnectionobject_p.h \
    qcloudservicessharedobject.h
