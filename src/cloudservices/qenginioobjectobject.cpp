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

#include "QtCloudServices/private/qenginioobjectobject_p.h"
#include "QtCloudServices/private/qenginioobjectshared_p.h"
#include "QtCloudServices/private/qenginiooperationobject_p.h"
#include "QtCloudServices/private/qenginiooperationshared_p.h"
#include "QtCloudServices/private/qenginioobjectobject_p.h"
#include "QtCloudServices/private/qenginioobjectshared_p.h"
#include "QtCloudServices/private/qenginiouserobject_p.h"
#include "QtCloudServices/private/qenginiousershared_p.h"

QT_BEGIN_NAMESPACE

/*
** Private
*/
QEnginioObjectObjectPrivate::QEnginioObjectObjectPrivate(QSharedPointer<QEnginioObjectShared> aShared)
    : iShared(aShared)
{
}

QEnginioObjectObjectPrivate::QEnginioObjectObjectPrivate()
{    
}

QEnginioObjectObjectPrivate::QEnginioObjectObjectPrivate(const QJsonObject &aJsonObject)
{
    QSharedPointer<QEnginioObjectShared> shared(new QEnginioObjectShared(aJsonObject));
    setSharedInstance(shared);
}

bool QEnginioObjectObjectPrivate::isValid() const {
    if (!iShared) {
        return false;
    }
    return iShared->isValid();
}
bool QEnginioObjectObjectPrivate::isPersistent() const {
    if (!iShared) {
        return false;
    }
    return iShared->isPersistent();
}
bool QEnginioObjectObjectPrivate::isModified() const {
    if (!iShared) {
        return false;
    }
    return iShared->isModified();
}

void QEnginioObjectObjectPrivate::insert(const QString &aKey, const QJsonValue &aValue) {
    if (!iShared) {
        lazyInitialization();
    }
    iShared->insert(aKey,aValue);
}

void QEnginioObjectObjectPrivate::remove(const QString &aKey) {
    if (!iShared) {
        return;
    }
    iShared->remove(aKey);
}

bool QEnginioObjectObjectPrivate::contains(const QString &aKey) const {
    if (!iShared) {
        return false;
    }
    return iShared->contains(aKey);
}
QJsonValue QEnginioObjectObjectPrivate::value(const QString &aKey) const {
    if (!iShared) {
        return QJsonValue();
    }
    return iShared->value(aKey);
}
QJsonValueRef QEnginioObjectObjectPrivate::valueRef(const QString &aKey)  {
    if (!iShared) {
        lazyInitialization();
    }
    return iShared->valueRef(aKey);
}

QJsonObject QEnginioObjectObjectPrivate::jsonObject() const {
    if (!iShared) {
        return jsonObject();
    }
    return iShared->jsonObject();
}

QString QEnginioObjectObjectPrivate::objectId() const {
    if (!iShared) {
       return QString();
    }
    return iShared->objectId();
}
QString QEnginioObjectObjectPrivate::objectType() const {
    if (!iShared) {
       return QString();
    }
    return iShared->objectType();
}

QTime QEnginioObjectObjectPrivate::createAt() const {
    if (!iShared) {
       return QTime();
    }
    return iShared->createAt();
}
QEnginioUserObject *QEnginioObjectObjectPrivate::creator() const {
    QEnginioUserObject *obj;

    if (!iShared) {
       return NULL;
    }

    QSharedPointer<QEnginioUserShared> user;
    user = iShared->creator();

    if (user.isNull()) {
        return NULL;
    }

    obj = new QEnginioUserObject;
    obj->d_func()->setSharedInstance(user);

    return obj;
}
QTime QEnginioObjectObjectPrivate::updatedAt() const {
    if (!iShared) {
       return QTime();
    }
    return iShared->updatedAt();
}
QEnginioUserObject *QEnginioObjectObjectPrivate::updater() const {
    QEnginioUserObject *obj;

    if (!iShared) {
       return NULL;
    }

    QSharedPointer<QEnginioUserShared> user;
    user = iShared->updater();

    if (user.isNull()) {
        return NULL;
    }

    obj = new QEnginioUserObject;
    obj->d_func()->setSharedInstance(user);

    return obj;
}

QEnginioOperationObject *QEnginioObjectObjectPrivate::save() {
    QEnginioOperationObject *operation = NULL;
    QSharedPointer<QEnginioOperationShared> opShared;

    if (!iShared) {
        return operation;
    }

    opShared = iShared->save(iShared);

    if (opShared) {
        operation = new QEnginioOperationObject;
        operation->d_func()->setSharedInstance(opShared);
    }

    return operation;
}

void QEnginioObjectObjectPrivate::lazyInitialization() {
    QSharedPointer<QEnginioObjectShared> shared(new QEnginioObjectShared);
    setSharedInstance(shared);
}

void QEnginioObjectObjectPrivate::setEnginioCollection(QSharedPointer<QEnginioCollectionShared> aEnginioCollection) {
    if (!iShared) {
       lazyInitialization();
    }
    iShared->setEnginioCollection(aEnginioCollection);
}

void QEnginioObjectObjectPrivate::init() {

}

void QEnginioObjectObjectPrivate::deinit() {

}

QSharedPointer<QEnginioObjectShared> QEnginioObjectObjectPrivate::sharedInstance() const {
    return iShared;
}
void QEnginioObjectObjectPrivate::setSharedInstance(QSharedPointer<QEnginioObjectShared> aShared) {
    if (iShared) {
        init();
    }

    iShared = aShared;

    if (iShared) {
        deinit();
    }
}


/*
** Public
*/

QEnginioObjectObject::QEnginioObjectObject(QEnginioObjectObjectPrivate &dd,QObject *aParent)
    : QObject(dd,aParent)
{}

QEnginioObjectObject::QEnginioObjectObject(QObject *aParent)
    : QObject(*new QEnginioObjectObjectPrivate,aParent)
{

}

QEnginioObjectObject::QEnginioObjectObject(const QJsonObject &aJsonObject,QObject *aParent)
    : QObject(*new QEnginioObjectObjectPrivate(aJsonObject),aParent)
{

}

bool QEnginioObjectObject::isValid() const {
    Q_D(const QEnginioObjectObject);
    return d->isValid();
}
bool QEnginioObjectObject::isPersistent() const {
    Q_D(const QEnginioObjectObject);
    return d->isPersistent();
}
bool QEnginioObjectObject::isModified() const {
    Q_D(const QEnginioObjectObject);
    return d->isModified();
}

void QEnginioObjectObject::insert(const QString &aKey, const QJsonValue &aValue) {
    Q_D(QEnginioObjectObject);
    d->insert(aKey,aValue);
}

void QEnginioObjectObject::remove(const QString &aKey) {
    Q_D(QEnginioObjectObject);
    d->remove(aKey);
}

bool QEnginioObjectObject::contains(const QString &aKey) const {
    Q_D(const QEnginioObjectObject);
    return d->contains(aKey);
}
QJsonValue QEnginioObjectObject::value(const QString &aKey) const {
    Q_D(const QEnginioObjectObject);
    return d->value(aKey);
}
QJsonValueRef QEnginioObjectObject::valueRef(const QString &aKey) {
    Q_D(QEnginioObjectObject);
    return d->valueRef(aKey);
}

QJsonObject QEnginioObjectObject::jsonObject() const {
    Q_D(const QEnginioObjectObject);
    return d->jsonObject();
}

QString QEnginioObjectObject::objectId() const {
    Q_D(const QEnginioObjectObject);
    return d->objectId();
}
QString QEnginioObjectObject::objectType() const {
    Q_D(const QEnginioObjectObject);
    return d->objectType();
}
QTime QEnginioObjectObject::createAt() const {
    Q_D(const QEnginioObjectObject);
    return d->createAt();
}
QEnginioUserObject *QEnginioObjectObject::creator() const {
    Q_D(const QEnginioObjectObject);
    return d->creator();
}
QTime QEnginioObjectObject::updatedAt() const {
    Q_D(const QEnginioObjectObject);
    return d->updatedAt();
}
QEnginioUserObject *QEnginioObjectObject::updater() const {
    Q_D(const QEnginioObjectObject);
    return d->updater();
}

QEnginioOperationObject *QEnginioObjectObject::save() {
    Q_D(QEnginioObjectObject);
    return d->save();
}

void QEnginioObjectObject::setSharedInstanceFrom(const QEnginioObjectObject *aOther) {
    Q_D(QEnginioObjectObject);    
    d->setSharedInstance(aOther->d_func()->sharedInstance());
}

QT_END_NAMESPACE

