#include <QtCore/qjsondocument.h>
#include <QtCore/qdebug.h>

#include <QJSonParseError>
#include <QJsonArray>
#include <QJsonValue>

#include "apiusagedemo.h"

#include <QtCloudServices/QEnginioOperation.h>

ApiUsageDemo::ApiUsageDemo(QWidget *parent)
    : QMainWindow(parent),
      iEnginioDataStorage("https://api.engin.io", "5379dea0698b3c1dc00cdf57")
{
    ui.setupUi(this);

    ui.lineEditEdsBackendId->setText(iEnginioDataStorage.backendId());
    ui.lineEditEdsBackendAddress->setText(iEnginioDataStorage.backendAddress().toString());

    connect(ui.lineEditEdsBackendId, &QLineEdit::textChanged,
            &iEnginioDataStorage, &QEnginioDataStorage::setBackendId);
    connect(ui.lineEditEdsBackendAddress, &QLineEdit::textChanged,
            &iEnginioDataStorage, &QEnginioDataStorage::setBackendAddressString);
    connect(&iEnginioDataStorage, &QEnginioDataStorage::backendIdChanged,
            ui.lineEditEdsBackendId, &QLineEdit::setText);
    connect(&iEnginioDataStorage, &QEnginioDataStorage::backendAddressChanged,
    [ = ](const QUrl & backendAddress) {
        this->ui.lineEditEdsBackendAddress->setText(backendAddress.toString());
    });
}

ApiUsageDemo::~ApiUsageDemo()
{

}

void ApiUsageDemo::doExecute()
{
    QEnginioCollection collection;
    collection = iEnginioDataStorage.collection(ui.lineEditCollection->text());

    QJsonObject object;
    QJsonDocument document;
    QJsonParseError documentParseError;
    QString objectId;
    QByteArray tmp;

    ui.plainTextEditOutput->clear();

    switch (ui.tabWidgetObjectOperations->currentIndex()) {
    case 0:  // find
        tmp = ui.plainTextEditFindInput->toPlainText().toLatin1();

        if (tmp.size() > 0) {
            document = QJsonDocument::fromJson(tmp, &documentParseError);

            if (documentParseError.error != QJsonParseError::NoError) {
                ui.plainTextEditOutput->insertPlainText(
                    tr("** Input JSON Invalid, Parse Error:\n").append(documentParseError.errorString()));
                return;
            }

            if (document.isObject()) {
                object = document.object();
            }
        }

        // collection.insert(object, [=](QEnginioOperation & op) { this->handleOperationReply(op); });
        collection.find(QEnginioQuery(object)
                        .limit(ui.spinBoxFindLimit->value())
                        .offset(ui.spinBoxFindOffset->value())
                        ,
                        std::bind(std::mem_fn(&ApiUsageDemo::handleOperationReply), this,
                                  std::placeholders::_1));

        break;

    case 1: // findOne
        objectId = ui.lineEditFindOneObjectId->text();
        collection.findOne(objectId, std::bind(std::mem_fn(&ApiUsageDemo::handleOperationReply), this,
                                               std::placeholders::_1));

        break;

    case 2: // insert
        document = QJsonDocument::fromJson(ui.plainTextEditInsertInput->toPlainText().toLatin1(),
                                           &documentParseError);

        if (documentParseError.error != QJsonParseError::NoError) {
            ui.plainTextEditOutput->insertPlainText(
                tr("** Input JSON Invalid, Parse Error:\n").append(documentParseError.errorString()));
            return;
        }

        if (document.isObject()) {
            object = document.object();
        } else {
            ui.plainTextEditOutput->insertPlainText(
                tr("** Input JSON Invalid.\n"));
            return;
        }

        // collection.insert(object, [=](QEnginioOperation & op) { this->handleOperationReply(op); });
        collection.insert(object, std::bind(std::mem_fn(&ApiUsageDemo::handleOperationReply), this,
                                            std::placeholders::_1));

        break;

    case 3: // update
        objectId = ui.lineEditUpdateObjectId->text();
        document = QJsonDocument::fromJson(ui.plainTextEditUpdateInput->toPlainText().toLatin1(),
                                           &documentParseError);

        if (documentParseError.error != QJsonParseError::NoError) {
            ui.plainTextEditOutput->insertPlainText(
                tr("** Input JSON Invalid, Parse Error:\n").append(documentParseError.errorString()));
            return;
        }

        if (document.isObject()) {
            object = document.object();
        } else {
            ui.plainTextEditOutput->insertPlainText(
                tr("** Input JSON Invalid.\n"));
            return;
        }

        collection.update(objectId,
                          object, std::bind(std::mem_fn(&ApiUsageDemo::handleOperationReply), this,
                                            std::placeholders::_1));

        break;

    case 4: // remove
        objectId = ui.lineEditRemoveObjectId->text();
        collection.remove(objectId, std::bind(std::mem_fn(&ApiUsageDemo::handleOperationReply), this,
                                              std::placeholders::_1));

        break;
    }
}

void ApiUsageDemo::handleOperationReply(QEnginioOperation & op)
{
    if (!op) {
        this->ui.plainTextEditOutput->insertPlainText(
            tr("Processing error..").append(op.errorString()).append("\n")
        );
        return;
    }

    ui.plainTextEditOutput->insertPlainText(
        tr("Operation OK\n")
    );

    ui.plainTextEditOutput->insertPlainText(
        tr("RequestId: ").append(op.requestId()).append("\n")
    );
    ui.plainTextEditOutput->insertPlainText(
        tr("ResultObject:\n")
    );

    ui.plainTextEditOutput->insertPlainText(
        QString::fromLatin1(QJsonDocument(op.result()).toJson())
    );


    if (0 == ui.tabWidgetObjectOperations->currentIndex()) { // find
        qDebug() << "ResultObjects: " << op.resultObjectCount();
        QList<QEnginioObject> lst = op.resultObjects();
        QList<QEnginioObject>::iterator i;

        for (i = lst.begin(); i != lst.end(); ++i) {
            qDebug() << (*i);
        }
    }
}
