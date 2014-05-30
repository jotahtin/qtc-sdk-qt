TEMPLATE = app

QT += qml quick declarative

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../QtCloudServices/release/ -lQtCloudServices
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../QtCloudServices/debug/ -lQtCloudServices
else:unix: LIBS += -L$$OUT_PWD/../../../QtCloudServices/ -lQtCloudServices

INCLUDEPATH += $$PWD/../../../QtCloudServices
DEPENDPATH += $$PWD/../../../QtCloudServices
