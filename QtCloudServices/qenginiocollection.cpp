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
** QEnginioCollectionObject
*/
QEnginioCollectionObject::QEnginioCollectionObject(QSharedPointer<QEnginioDataStorageObject> aEDS,
        const QString &aCollectionName)
    : iEDS(aEDS), iCollectionName(aCollectionName)
{
}

bool QEnginioCollectionObject::isValid() const
{
    if (!iEDS) {
        return false;
    }

    if (iCollectionName.isEmpty()) {
        return false;
    }


    return true;
}

QString QEnginioCollectionObject::collectionName() const
{
    return iCollectionName;
}

QEnginioOperation QEnginioCollectionObject::find(QSharedPointer<QEnginioCollectionObject> aSelf,
        const QEnginioQuery &aQuery,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionObject> connection;
    connection = iEDS->reserveConnection(iEDS);

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

    request.urlQuery(urlQuery);
    request.then([ = ](QEnginioOperation & op) {
        if (op.isValid() && !op.isError()) {
            iEDS->releaseConnection(connection);
        }

        aCallback(op);
    });
    return connection->customRequest(connection, request, aSelf);

}

QEnginioOperation QEnginioCollectionObject::findOne(QSharedPointer<QEnginioCollectionObject> aSelf,
        const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionObject> connection;
    connection = iEDS->reserveConnection(iEDS);

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);
    path.append("/");
    path.append(aObjectId);

    QEnginioRequest request(QtCloudServices::RESTOperationGet, path);
    request.then([ = ](QEnginioOperation & op) {
        if (op.isValid() && !op.isError()) {
            iEDS->releaseConnection(connection);
        }

        aCallback(op);
    });
    return connection->customRequest(connection, request, aSelf);
}


QEnginioOperation QEnginioCollectionObject::insert(QSharedPointer<QEnginioCollectionObject> aSelf,
        const QEnginioObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionObject> connection;
    connection = iEDS->reserveConnection(iEDS);

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);

    QEnginioRequest request(QtCloudServices::RESTOperationPost, path);
    request.payload(aObject.jsonObject());
    request.then([ = ](QEnginioOperation & op) {
        if (op.isValid() && !op.isError()) {
            iEDS->releaseConnection(connection);
        }

        // TODO: update collection & collect weak ref to object object

        aCallback(op);
    });
    return connection->customRequest(connection, request, aSelf);
}

QEnginioOperation QEnginioCollectionObject::update(QSharedPointer<QEnginioCollectionObject> aSelf,
        const QString &aObjectId,
        const QJsonObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionObject> connection;
    connection = iEDS->reserveConnection(iEDS);

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);
    path.append("/");
    path.append(aObjectId);

    QEnginioRequest request(QtCloudServices::RESTOperationPut, path);
    request.payload(aObject);
    request.then([ = ](QEnginioOperation & op) {
        if (op.isValid() && !op.isError()) {
            iEDS->releaseConnection(connection);
        }

        aCallback(op);
    });
    return connection->customRequest(connection, request, aSelf);
}

QEnginioOperation QEnginioCollectionObject::remove(QSharedPointer<QEnginioCollectionObject> aSelf,
        const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QString path;
    QSharedPointer<QEnginioConnectionObject> connection;
    connection = iEDS->reserveConnection(iEDS);

    path = QtCloudServicesConstants::service_path_objects;
    path.append(iCollectionName);
    path.append("/");
    path.append(aObjectId);

    QEnginioRequest request(QtCloudServices::RESTOperationDelete, path);
    request.then([ = ](QEnginioOperation & op) {
        if (op.isValid() && !op.isError()) {
            iEDS->releaseConnection(connection);
        }

        aCallback(op);
    });
    return connection->customRequest(connection, request, aSelf);
}

QEnginioObject QEnginioCollectionObject::fromJsonObject(QSharedPointer<QEnginioCollectionObject> aSelf,
        const QJsonObject &aJsonObject)
{
    QEnginioObject obj;
    QEnginioObjectPrivate *obj_d;
    obj_d = reinterpret_cast<QEnginioObjectPrivate *>(QTC_D_PTR(&obj));

    if (obj_d) {
        obj_d->setEnginioObjectObject(QEnginioObjectObject::get(aSelf, aJsonObject));
    }

    return obj;
}

/*
** QEnginioCollectionPrivate
*/
QEnginioCollectionPrivate::QEnginioCollectionPrivate()
{

}

QSharedPointer<QEnginioCollectionObject> QEnginioCollectionPrivate::enginioCollectionObject() const
{
    return iObject;
}
void QEnginioCollectionPrivate::setEnginioCollectionObject(QSharedPointer<QEnginioCollectionObject> aObject)
{
    iObject = aObject;
}

bool QEnginioCollectionPrivate::isValid() const
{
    if (iObject) {
        return iObject->isValid();
    }

    return false;
}
QString QEnginioCollectionPrivate::collectionName() const
{
    if (iObject) {
        return iObject->collectionName();
    }

    return QString();
}

QEnginioOperation QEnginioCollectionPrivate::find(const QEnginioQuery &aQuery,
        QEnginioOperation::Callback aCallback)
{
    QEnginioOperation operation;

    if (iObject) {
        operation = iObject->find(iObject, aQuery, aCallback);
    }

    return operation;
}
QEnginioOperation QEnginioCollectionPrivate::findOne(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QEnginioOperation operation;

    if (iObject) {
        operation = iObject->findOne(iObject, aObjectId, aCallback);
    }

    return operation;
}

QEnginioOperation QEnginioCollectionPrivate::insert(const QEnginioObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QEnginioOperation operation;

    if (iObject) {
        operation = iObject->insert(iObject, aObject, aCallback);
    }

    return operation;
}
QEnginioOperation QEnginioCollectionPrivate::update(const QString &aObjectId,
        const QJsonObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QEnginioOperation operation;

    if (iObject) {
        operation = iObject->update(iObject, aObjectId, aObject, aCallback);
    }

    return operation;

}
QEnginioOperation QEnginioCollectionPrivate::remove(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QEnginioOperation operation;

    if (iObject) {
        operation = iObject->remove(iObject, aObjectId, aCallback);
    }

    return operation;
}

QEnginioObject QEnginioCollectionPrivate::fromJsonObject(const QJsonObject &aJsonObject)
{
    if (iObject) {
        return iObject->fromJsonObject(enginioCollectionObject(), aJsonObject);
    }

    return QEnginioObject();
}

/*
** QEnginioCollection
*/
QEnginioCollection::QEnginioCollection(QObject *aParent)
    : QCloudServicesObject(*new QEnginioCollectionPrivate(), aParent)
{
}

QEnginioCollection::QEnginioCollection(const QEnginioCollection &aOther, QObject *aParent)
    : QCloudServicesObject(*new QEnginioCollectionPrivate(), aParent)
{
    *this = aOther;
}
QEnginioCollection& QEnginioCollection::operator=(const QEnginioCollection &aOther)
{
    QTC_D(QEnginioCollection);
    QEnginioCollectionPrivate *other;

    other = reinterpret_cast<QEnginioCollectionPrivate *>(QTC_D_PTR(&aOther));

    if (other) {
        d->setEnginioCollectionObject(other->enginioCollectionObject());
    }

    return *this;
}

bool QEnginioCollection::operator!() const
{
    return !isValid();
}

bool QEnginioCollection::isValid() const
{
    QTC_D(const QEnginioCollection);
    return d->isValid();
}

QString QEnginioCollection::collectionName() const
{
    QTC_D(const QEnginioCollection);
    return d->collectionName();
}

QEnginioOperation QEnginioCollection::find(const QEnginioQuery &aQuery,
        QEnginioOperation::Callback aCallback)
{
    QTC_D(QEnginioCollection);
    return d->find(aQuery, aCallback);
}

QEnginioOperation QEnginioCollection::findOne(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QTC_D(QEnginioCollection);
    return d->findOne(aObjectId, aCallback);
}

QEnginioOperation QEnginioCollection::insert(const QEnginioObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QTC_D(QEnginioCollection);
    return d->insert(aObject, aCallback);
}
/*
QEnginioOperation QEnginioCollection::insert(const QJsonObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QTC_D(QEnginioCollection);
    return d->insert(aObject, aCallback);
}
*/
QEnginioOperation QEnginioCollection::update(const QString &aObjectId,
        const QJsonObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    QTC_D(QEnginioCollection);
    return d->update(aObjectId, aObject, aCallback);
}
QEnginioOperation QEnginioCollection::remove(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    QTC_D(QEnginioCollection);
    return d->remove(aObjectId, aCallback);
}

QEnginioObject QEnginioCollection::fromJsonObject(const QJsonObject &aJsonObject)
{
    QTC_D(QEnginioCollection);
    return d->fromJsonObject(aJsonObject);
}

QT_END_NAMESPACE
