/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtEnginio module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "stdafx.h"

#include <QJsonDocument>

#include <QtCloudServices/qenginiocollection.h>
#include <QtCloudServices/qenginiodatastorage.h>

#include <QtCloudServices/private/qcloudservicesconstants_p.h>
#include <QtCloudServices/private/qenginiocollectionshared_p.h>
#include <QtCloudServices/private/qenginiodatastorageshared_p.h>
#include <QtCloudServices/private/qenginiorequestshared_p.h>
#include <QtCloudServices/private/qenginioobjectshared_p.h>

QT_BEGIN_NAMESPACE

QEnginioCollectionShared::QEnginioCollectionShared(QSharedPointer<QEnginioDataStorageShared> aEnginioDataStorage,
                                                   QString aCollectionName)
    : iEnginioDataStorage(aEnginioDataStorage),
      iCollectionName(aCollectionName)
{

}

bool QEnginioCollectionShared::isValid() const
{
    if (!iEnginioDataStorage) {
        return false;
    }

    if (iCollectionName.isEmpty()) {
        return false;
    }

    return true;
}

QString QEnginioCollectionShared::collectionName() const
{
    return iCollectionName;
}

QSharedPointer<QEnginioOperationShared>
QEnginioCollectionShared::find(QSharedPointer<QEnginioCollectionShared> aSelf,
                               QSharedPointer<QEnginioQueryShared> aQuery,
                               QEnginioOperationShared::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionShared> connection;

    connection = iEnginioDataStorage->reserveConnection(iEnginioDataStorage);

    path = resolveBasePath();

    QSharedPointer<QEnginioRequestShared> request(new QEnginioRequestShared(QtCloudServices::RESTOperationGet, path));

    QUrlQuery urlQuery;
    urlQuery.addQueryItem("limit", QString("%1").arg(aQuery.limit()));
    urlQuery.addQueryItem("offset", QString("%1").arg(aQuery.offset()));

    if (!aQuery.query().isEmpty()) {
        urlQuery.addQueryItem("q",
                              QString::fromLatin1(QJsonDocument(aQuery.query()).toJson(QJsonDocument::Compact)));
    }

    request->setEnginioCollection(aSelf);
    request->setUrlQuery(urlQuery);
    request->setCallback([=](QSharedPointer<QRestOperationObject> aOperation) {
        aSelf->handleCompletedOperation(aOperation,connection,aCallback);
    });

    return connection->customRequest(connection,request);
}

QSharedPointer<QEnginioOperationShared>
QEnginioCollectionShared::findOne(QSharedPointer<QEnginioCollectionShared> aSelf,
                                  const QString &aObjectId,
                                  QEnginioOperationShared::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionShared> connection;

    connection = iEnginioDataStorage->reserveConnection(iEnginioDataStorage);

    path = resolveBasePath();
    path.append("/");
    path.append(aObjectId);

    QSharedPointer<QEnginioRequestShared> request(new QEnginioRequestShared(QtCloudServices::RESTOperationGet, path));
    request->setEnginioCollection(aSelf);
    request->setCallback([=](QSharedPointer<QRestOperationObject> aOperation) {
        aSelf->handleCompletedOperation(aOperation,connection,aCallback);
    });

    return connection->customRequest(connection,request);
}

QSharedPointer<QEnginioOperationShared>
QEnginioCollectionShared::insert(QSharedPointer<QEnginioCollectionShared> aSelf,
                                 QSharedPointer<QEnginioObjectShared> aObject,
                                 QEnginioOperationShared::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionShared> connection;

    connection = iEnginioDataStorage->reserveConnection(iEnginioDataStorage);

    path = resolveBasePath();

    QSharedPointer<QEnginioRequestShared> request(new QEnginioRequestShared(QtCloudServices::RESTOperationPost, path));
    request->setEnginioCollection(aSelf);
    request->setPayload(aObject->jsonObject()());
    request->setCallback([=](QSharedPointer<QRestOperationObject> aOperation) {
        aSelf->handleCompletedOperation(aOperation,connection,aCallback);
    });

    return connection->customRequest(connection,request);
}

QSharedPointer<QEnginioOperationShared>
QEnginioCollectionShared::update(QSharedPointer<QEnginioCollectionShared> aSelf,
                                 const QString &aObjectId,
                                 const QJsonObject &aObject,
                                 QEnginioOperationShared::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionShared> connection;

    connection = iEnginioDataStorage->reserveConnection(iEnginioDataStorage);

    path = resolveBasePath();
    path.append("/");
    path.append(aObjectId);

    QSharedPointer<QEnginioRequestShared> request(new QEnginioRequestShared(QtCloudServices::RESTOperationPut, path));
    request->setEnginioCollection(aSelf);
    request->setPayload(aObject);
    request->setCallback([=](QSharedPointer<QRestOperationObject> aOperation) {
        aSelf->handleCompletedOperation(aOperation,connection,aCallback);
    });

    return connection->customRequest(connection,request);
}

QSharedPointer<QEnginioOperationShared>
QEnginioCollectionShared::remove(QSharedPointer<QEnginioCollectionShared> aSelf,
                                 const QString &aObjectId,
                                 QEnginioOperationShared::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionShared> connection;

    connection = iEnginioDataStorage->reserveConnection(iEnginioDataStorage);

    path = resolveBasePath();
    path.append("/");
    path.append(aObjectId);

    QSharedPointer<QEnginioRequestShared> request(new QEnginioRequestShared(QtCloudServices::RESTOperationDelete, path));
    request->setEnginioCollection(aSelf);
    request->setCallback([=](QSharedPointer<QRestOperationObject> aOperation) {
        aSelf->handleCompletedOperation(aOperation,connection,aCallback);
    });

    return connection->customRequest(connection,request);
}

QSharedPointer<QEnginioObjectShared>
QEnginioCollectionShared::fromJsonObject(QSharedPointer<QEnginioCollectionShared> aSelf,
                                         const QJsonObject &aJsonObject)
{
    QSharedPointer<QEnginioObjectShared> obj(new QEnginioObjectShared(aJsonObject));
    obj->setEnginioCollection(aSelf);
    return obj;
}

QString QEnginioCollectionShared::resolveBasePath() {
    QString path;

    // Object<Path>
    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);

    return path;
}

void QEnginioCollectionShared::handleCompletedOperation(QSharedPointer<QRestOperationObject> aOperation,
                                                        QSharedPointer<QEnginioConnectionShared> aConnection,
                                                        QEnginioOperationShared::Callback aCallback)
{
    if (!aOperation) {
        return;
    }

    if (aOperation->isValid() && !aOperation->isError()) {
        iEnginioDataStorage->releaseConnection(aConnection);
    }

    aCallback(qSharedPointerCast<QEnginioOperationShared>(aOperation));
}

QT_END_NAMESPACE
