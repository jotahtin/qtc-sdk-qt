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

#include <QtCloudServices/private/qenginiocollectionobject_p.h>
#include <QtCloudServices/private/qenginiocollectionshared_p.h>
#include <QtCloudServices/private/qenginioobjectobject_p.h>
#include <QtCloudServices/private/qenginiooperationobject_p.h>
#include <QtCloudServices/private/qenginioqueryobject_p.h>

QT_BEGIN_NAMESPACE

/*
** Private
*/

QEnginioCollectionObjectPrivate::QEnginioCollectionObjectPrivate() {

}

bool QEnginioCollectionObjectPrivate::isValid() const {
    if (!iShared)
        return false;
    return iShared->isValid();
}

QString QEnginioCollectionObjectPrivate::collectionName() const {
    return iShared->collectionName();
}

QEnginioOperationObject *
QEnginioCollectionObjectPrivate::find(const QEnginioQueryObject *aQuery,
                                      QEnginioOperationObject::Callback aCallback)
{
    QEnginioOperationObject *op;
    QSharedPointer<QEnginioOperationShared> opShared;

    if (!aQuery) {
        return NULL;
    }

    opShared = iShared->find(iShared,aQuery->d_func()->sharedInstance(),
                [=](QSharedPointer<QEnginioOperationShared> aOperation) {
                    handleCompletedOperation(aOperation,aCallback);
                });

    if (!opShared) {
        return NULL;
    }

    op=new QEnginioOperationObject;
    op->d_func()->setSharedInstance(opShared);

    return op;
}

QEnginioOperationObject *
QEnginioCollectionObjectPrivate::findOne(const QString &aObjectId,
                                         QEnginioOperationObject::Callback aCallback)
{
    QEnginioOperationObject *op;
    QSharedPointer<QEnginioOperationShared> opShared;

    opShared = iShared->findOne(iShared,aObjectId,
                [=](QSharedPointer<QEnginioOperationShared> aOperation) {
                    handleCompletedOperation(aOperation,aCallback);
                });

    if (!opShared) {
        return NULL;
    }

    op=new QEnginioOperationObject;
    op->d_func()->setSharedInstance(opShared);

    return op;
}

QEnginioOperationObject *
QEnginioCollectionObjectPrivate::insert(const QEnginioObjectObject *aObject,
                                        QEnginioOperationObject::Callback aCallback)
{
    QEnginioOperationObject *op;
    QSharedPointer<QEnginioOperationShared> opShared;

    if (!aObject) {
        return NULL;
    }

    opShared = iShared->insert(iShared,aObject->d_func()->sharedInstance(),
                [=](QSharedPointer<QEnginioOperationShared> aOperation) {
                    handleCompletedOperation(aOperation,aCallback);
                });

    if (!opShared) {
        return NULL;
    }

    op=new QEnginioOperationObject;
    op->d_func()->setSharedInstance(opShared);

    return op;
}

QEnginioOperationObject *
QEnginioCollectionObjectPrivate::update(const QString &aObjectId,
                                        const QJsonObject &aObject,
                                        QEnginioOperationObject::Callback aCallback)
{
    QEnginioOperationObject *op;
    QSharedPointer<QEnginioOperationShared> opShared;

    opShared = iShared->update(iShared,aObjectId, aObject,
                [=](QSharedPointer<QEnginioOperationShared> aOperation) {
                    handleCompletedOperation(aOperation,aCallback);
                });

    if (!opShared) {
        return NULL;
    }

    op=new QEnginioOperationObject;
    op->d_func()->setSharedInstance(opShared);

    return op;
}

QEnginioOperationObject *
QEnginioCollectionObjectPrivate::remove(const QString &aObjectId,
                                        QEnginioOperationObject::Callback aCallback)
{
    QEnginioOperationObject *op;
    QSharedPointer<QEnginioOperationShared> opShared;

    opShared = iShared->remove(iShared,aObjectId,
                [=](QSharedPointer<QEnginioOperationShared> aOperation) {
                    handleCompletedOperation(aOperation,aCallback);
                });

    if (!opShared) {
        return NULL;
    }

    op=new QEnginioOperationObject;
    op->d_func()->setSharedInstance(opShared);

    return op;
}

QEnginioObjectObject *QEnginioCollectionObjectPrivate::fromJsonObject(const QJsonObject &aJsonObject) {
    QEnginioObjectObject *obj;

    obj = new QEnginioObjectObject();
    obj->d_func()->setSharedInstance(iShared->fromJsonObject(iShared,aJsonObject));

    return obj;
}

/*
QSharedPointer<QRestConnectionShared> QEnginioCollectionObjectPrivate::buildSharedInstance(QSharedPointer<QRestEndpointShared> aEndpoint)
{
    QSharedPointer<QEnginioDataStorageObject> eds;

    eds=qSharedPointerCast<QEnginioDataStorageObject>(aEndpoint);

    return QSharedPointer<QRestConnectionShared>(new QEnginioConnectionShared(eds));
}
*/

void QEnginioCollectionObjectPrivate::init() {

}

void QEnginioCollectionObjectPrivate::deinit()  {

}

QSharedPointer<QEnginioCollectionShared>
QEnginioCollectionObjectPrivate::sharedInstance() const {
    return iShared;
}
void QEnginioCollectionObjectPrivate::setSharedInstance(QSharedPointer<QEnginioCollectionShared> aShared) {
    if (iShared) {
        deinit();
    }

    iShared = aShared;

    if (iShared) {
        init();
    }
}

void QEnginioCollectionObjectPrivate::handleCompletedOperation(QSharedPointer<QEnginioOperationShared> aOperation,
                              QEnginioOperationObject::Callback aCallback)
{
    QEnginioOperationObject *obj;

    if (!aCallback) {
        return;
    }

    obj = new QEnginioOperationObject();
    obj->d_func()->setSharedInstance(aOperation);

    aCallback(obj);
}

/*
** Public
*/

QEnginioCollectionObject::QEnginioCollectionObject(QObject *aParent)
    : QObject(*new QEnginioCollectionObjectPrivate,aParent)
{

}

bool QEnginioCollectionObject::isValid() const {
    Q_D(const QEnginioCollectionObject);
    return d->isValid();
}
QString QEnginioCollectionObject::collectionName() const {
    Q_D(const QEnginioCollectionObject);
    return d->collectionName();
}

QEnginioOperationObject *QEnginioCollectionObject::find(const QEnginioQueryObject *aQuery,
                                                        QEnginioOperationObject::Callback aCallback)
{
    Q_D(QEnginioCollectionObject);
    return d->find(aQuery,aCallback);
}

QEnginioOperationObject *QEnginioCollectionObject::findOne(const QString &aObjectId,
                                                           QEnginioOperationObject::Callback aCallback)
{
    Q_D(QEnginioCollectionObject);
    return d->findOne(aObjectId,aCallback);
}

QEnginioOperationObject *QEnginioCollectionObject::insert(const QEnginioObjectObject *aObject,
                                                          QEnginioOperationObject::Callback aCallback)
{
    Q_D(QEnginioCollectionObject);
    return d->insert(aObject,aCallback);
}

QEnginioOperationObject *QEnginioCollectionObject::update(const QString &aObjectId,
                                                          const QJsonObject &aObject,
                                                          QEnginioOperationObject::Callback aCallback)
{
    Q_D(QEnginioCollectionObject);
    return d->update(aObjectId,aObject,aCallback);
}

QEnginioOperationObject *QEnginioCollectionObject::remove(const QString &aObjectId,
                                                          QEnginioOperationObject::Callback aCallback)
{
    Q_D(QEnginioCollectionObject);
    return d->remove(aObjectId,aCallback);
}

QEnginioObjectObject *QEnginioCollectionObject::fromJsonObject(const QJsonObject &aJsonObject) {
    Q_D(QEnginioCollectionObject);
    return d->fromJsonObject(aJsonObject);
}

void QEnginioCollectionObject::setSharedInstanceFrom(const QEnginioCollectionObject *aOther) {
    Q_D(QEnginioCollectionObject);
    d->setSharedInstance(aOther->d_func()->sharedInstance());
}

QT_END_NAMESPACE
