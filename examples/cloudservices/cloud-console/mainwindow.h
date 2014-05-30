#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include <QtCloudServices/QEnginioDataStorage>
#include <QtCloudServices/QManagedWebSocket>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void doSwitchToEds();
    void doExecute();

    void doSwitchToMws();
public:
    void handleOperationReply(QEnginioOperation & op);
private:
    Ui::MainWindowClass ui;
private:
    QEnginioDataStorage iEnginioDataStorage;
    QManagedWebSocket iManagedWebSocket;
};

#endif // MainWindow_H
