QT += qml quick network cloudservices-private

QMAKE_DOCS = $$PWD/doc/qtcloudservicesqml.qdocconf
OTHER_FILES += \
    doc/qtcloudservicesqml.qdocconf \
    doc/qtcloudservicesqml_plugin.qdoc \
    qmldir

include(../../src.pri)

# qtmultimediaquicktools-private

HEADERS += \
        qqmlcloudservices_p.h

SOURCES += \
        qqmlcloudservices.cpp

# QML_FILES += \
#    Video.qml

# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

load(qml_plugin)
