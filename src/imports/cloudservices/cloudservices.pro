requires(qtHaveModule(quick))

TARGETPATH = QtCloudServices

QT += qml declarative quick network cloudservices cloudservices-private

CONFIG += release

QMAKE_DOCS = $$PWD/doc/qtcloudservicesqml.qdocconf
OTHER_FILES += \
    doc/qtcloudservicesqml.qdocconf \
    doc/qtcloudservicesqml_plugin.qdoc \
    qmldir

include(../../src.pri)

TARGET = qtcloudservices
TARGET.module_name = QtCloudServices

HEADERS += \
        qqmlcloudservices_p.h

SOURCES += \
        qqmlcloudservices.cpp

# QML_FILES += \
#    Video.qml

# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0

load(qml_plugin)

QMLDIRFILE = $${_PRO_FILE_PWD_}/qmldir

copy2build.input = QMLDIRFILE
copy2build.output = ../../../qml/$${TARGET.module_name}/qmldir
!contains(TEMPLATE_PREFIX, vc):copy2build.variable_out = PRE_TARGETDEPS
copy2build.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy2build.name = COPY ${QMAKE_FILE_IN}
copy2build.CONFIG += no_link
force_independent: QMAKE_EXTRA_COMPILERS += copy2build
