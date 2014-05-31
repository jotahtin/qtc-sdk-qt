#include <QDebug>

#include <QGuiApplication>
// #include <QQmlApplicationEngine>

#include <QQuickView>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;
// view.setSource(QUrl("qrc:///main.qml"));
    view.setSource(QUrl::fromLocalFile("/home/jta/Programming/QtC-SDK-Qt/examples/cloudservices/quick/edsops/main.qml"));

    view.show();

    return app.exec();
}
