QT += widgets

INCLUDEPATH += $$PWD
HEADERS += $$PWD/backendhelper.h
SOURCES += $$PWD/backendhelper.cpp
FORMS += $$PWD/helperdialog.ui

gcc*: {
    QMAKE_CXXFLAGS += -std=c++11
}
