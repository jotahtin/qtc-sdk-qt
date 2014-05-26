QT += network gui widgets

TARGET = Todos
TEMPLATE = app

include(../../../QtCloudServices.pri)
include(../../Common/backendhelper/backendhelper.pri)

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    todosmodel.cpp \

HEADERS += \
    mainwindow.h \
    todosmodel.h \

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../QtCloudServices/release/ -lQtCloudServices
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../QtCloudServices/debug/ -lQtCloudServices
else:unix: LIBS += -L$$OUT_PWD/../../../QtCloudServices/ -lQtCloudServices

INCLUDEPATH += $$PWD/../../../QtCloudServices
DEPENDPATH += $$PWD/../../../QtCloudServices
