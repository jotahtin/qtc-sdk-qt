#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include <QtCloudServices/QEnginioDataStorage>
#include <QtCloudServices/QManagedWebSocket>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void doSwitchToEds();
    void doEdsExecute();

    void doSwitchToMws();
    void doMwsConnect();
    void doMwsExecute();
public:
    void handleOperationReply(QEnginioOperation op);
private:
    Ui::MainWindowClass ui;
private:
    QEnginioDataStorage iEnginioDataStorage;
    QManagedWebSocket iManagedWebSocket;
};

#endif // MainWindow_H
