#ifndef QTCLOUDCONSOLE_H
#define QTCLOUDCONSOLE_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
};

#endif // QTCLOUDCONSOLE_H
