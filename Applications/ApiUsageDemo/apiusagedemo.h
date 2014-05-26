#ifndef APIUSAGEDEMO_H
#define APIUSAGEDEMO_H

#include <QtWidgets/QMainWindow>
#include "ui_apiusagedemo.h"

#include <QtCloudServices/QEnginioDataStorage>

#include <QtCloudServices/qenginioconnection.h>

class ApiUsageDemo : public QMainWindow {
    Q_OBJECT

public:
    ApiUsageDemo(QWidget *parent = 0);
    ~ApiUsageDemo();

public slots:
    void doExecute();
public:
    void handleOperationReply(QEnginioOperation & op);
private:
    Ui::ApiUsageDemoClass ui;
private:
    QEnginioDataStorage iEnginioDataStorage;
};

#endif // APIUSAGEDEMO_H
