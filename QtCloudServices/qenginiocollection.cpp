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

#include <QtCloudServices/qenginiocollection.h>

#include <QtCloudServices/private/qenginiocollection_p.h>
#include <QtCloudServices/private/qenginiodatastorage_p.h>
#include <QtCloudServices/private/qenginioobject_p.h>

QT_BEGIN_NAMESPACE

/*
** QEnginioCollectionPrivate
*/
QEnginioCollectionPrivate::QEnginioCollectionPrivate()
{

}
QEnginioCollectionPrivate::QEnginioCollectionPrivate(const QEnginioDataStorage &aEnginioDataStorage,
        const QString &aCollectionName)
    : iEnginioDataStorage(aEnginioDataStorage), iCollectionName(aCollectionName)
{
}

bool QEnginioCollectionPrivate::isValid() const
{
    if (!iEnginioDataStorage) {
        return false;
    }

    if (iCollectionName.isEmpty()) {
        return false;
    }

    return true;
}

QString QEnginioCollectionPrivate::collectionName() const
{
    return iCollectionName;
}

QEnginioOperation QEnginioCollectionPrivate::find(const QEnginioQuery &aQuery,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QEnginioConnection connection;
    QEnginioCollection::dvar self = getThis<QEnginioCollection>();

    connection = iEnginioDataStorage.reserveConnection();

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);

    QEnginioRequest request(QtCloudServices::RESTOperationGet, path);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("limit", QString("%1").arg(aQuery.limit()));
    urlQuery.addQueryItem("offset", QString("%1").arg(aQuery.offset()));

    if (!aQuery.query().isEmpty()) {
        urlQuery.addQueryItem("q",
                              QString::fromLatin1(QJsonDocument(aQuery.query()).toJson(QJsonDocument::Compact)));
    }

    request.setEnginioCollection(*q<QEnginioCollection>());
    request.setUrlQuery(urlQuery);
    request.then([ = ](QEnginioOperation & op) {
        self->handleCompletedOperation(op, connection, aCallback);
    });

    return connection.customRequest(request);
}

QEnginioOperation QEnginioCollectionPrivate::findOne(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QEnginioConnection connection;
    QEnginioCollection::dvar self = getThis<QEnginioCollection>();

    connection = iEnginioDataStorage.reserveConnection();

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);
    path.append("/");
    path.append(aObjectId);

    QEnginioRequest request(QtCloudServices::RESTOperationGet, path);
    request.setEnginioCollection(*q<QEnginioCollection>());
    request.then([ = ](QEnginioOperation & op) {
        self->handleCompletedOperation(op, connection, aCallback);
    });
    return connection.customRequest(request);
}

QEnginioOperation QEnginioCollectionPrivate::insert(const QEnginioObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QEnginioConnection connection;
    QEnginioCollection::dvar self = getThis<QEnginioCollection>();

    connection = iEnginioDataStorage.reserveConnection();

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);

    QEnginioRequest request(QtCloudServices::RESTOperationPost, path);
    request.setEnginioCollection(*q<QEnginioCollection>());
    request.setPayload(aObject.jsonObject());
    request.then([ = ](QEnginioOperation & op) {
        self->handleCompletedOperation(op, connection, aCallback);
    });

    return connection.customRequest(request);
}

QEnginioOperation QEnginioCollectionPrivate::update(const QString &aObjectId,
        const QJsonObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QEnginioConnection connection;
    QEnginioCollection::dvar self = getThis<QEnginioCollection>();

    connection = iEnginioDataStorage.reserveConnection();

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);
    path.append("/");
    path.append(aObjectId);

    QEnginioRequest request(QtCloudServices::RESTOperationPut, path);
    request.setEnginioCollection(*q<QEnginioCollection>());
    request.setPayload(aObject);
    request.then([ = ](QEnginioOperation & op) {
        self->handleCompletedOperation(op, connection, aCallback);
    });

    return connection.customRequest(request);
}

QEnginioOperation QEnginioCollectionPrivate::remove(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QEnginioConnection connection;
    QEnginioCollection::dvar self = getThis<QEnginioCollection>();

    connection = iEnginioDataStorage.reserveConnection();

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);
    path.append("/");
    path.append(aObjectId);

    QEnginioRequest request(QtCloudServices::RESTOperationDelete, path);
    request.setEnginioCollection(*q<QEnginioCollection>());
    request.then([ = ](QEnginioOperation & op) {
        self->handleCompletedOperation(op, connection, aCallback);
    });

    return connection.customRequest(request);
}

QEnginioObject QEnginioCollectionPrivate::fromJsonObject(const QJsonObject &aJsonObject)
{
    QEnginioObject obj(aJsonObject);

    obj.d<QEnginioObject>()->setEnginioCollection(*q<QEnginioCollection>());

    return obj;
}

void QEnginioCollectionPrivate::handleCompletedOperation(QEnginioOperation & op,
        QEnginioConnection aConnection,
        QEnginioOperation::Callback aCallback)
{
    if (op.isValid() && !op.isError()) {
        iEnginioDataStorage.releaseConnection(aConnection);
    }

    aCallback(op);
}

/*
** QEnginioCollection
*/
QEnginioCollection::QEnginioCollection(const QEnginioDataStorage &aEnginioDataStorage,
                                       const QString &aCollectionName)
    : QCloudServicesObject(QEnginioCollection::dvar(new QEnginioCollectionPrivate(aEnginioDataStorage,
                           aCollectionName)))
{
}

QEnginioCollection::QEnginioCollection(QObject *aParent)
    : QCloudServicesObject(QEnginioCollection::dvar(new QEnginioCollectionPrivate), aParent)
{
}

QEnginioCollection::QEnginioCollection(const QEnginioCollection &aOther, QObject *aParent)
    : QCloudServicesObject(aOther.d<QEnginioCollection>(), aParent)
{
}
QEnginioCollection & QEnginioCollection::operator=(const QEnginioCollection &aOther)
{
    setPIMPL(aOther.d<QEnginioCollection>());
    return *this;
}

bool QEnginioCollection::operator!() const
{
    return !isValid();
}

bool QEnginioCollection::isValid() const
{
    return d<const QEnginioCollection>()->isValid();
}

QString QEnginioCollection::collectionName() const
{
    return d<const QEnginioCollection>()->collectionName();
}

QEnginioOperation QEnginioCollection::find(const QEnginioQuery &aQuery,
        QEnginioOperation::Callback aCallback)
{
    return d<QEnginioCollection>()->find(aQuery, aCallback);
}

QEnginioOperation QEnginioCollection::findOne(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    return d<QEnginioCollection>()->findOne(aObjectId, aCallback);
}

QEnginioOperation QEnginioCollection::insert(const QEnginioObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    return d<QEnginioCollection>()->insert(aObject, aCallback);
}
QEnginioOperation QEnginioCollection::update(const QString &aObjectId,
        const QJsonObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    return d<QEnginioCollection>()->update(aObjectId, aObject, aCallback);
}
QEnginioOperation QEnginioCollection::remove(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    return d<QEnginioCollection>()->remove(aObjectId, aCallback);
}

QEnginioObject QEnginioCollection::fromJsonObject(const QJsonObject &aJsonObject)
{
    return d<QEnginioCollection>()->fromJsonObject(aJsonObject);
}

QT_END_NAMESPACE
