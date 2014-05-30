#include <QDebug>

#include <QApplication>
// #include <QQmlApplicationEngine>

#include <QQuickView>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView view;
    view.setSource(QUrl("qrc:///main.qml"));

    view.show();

    return app.exec();
}
