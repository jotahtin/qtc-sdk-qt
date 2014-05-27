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

#include "QtCloudServices/private/qenginioobject_p.h"
#include "QtCloudServices/private/qcloudservicesconstants_p.h"

QT_BEGIN_NAMESPACE

/*
** Shared Implementation
*/
QEnginioObjectObject::QEnginioObjectObject()
{

}
QEnginioObjectObject::QEnginioObjectObject(QSharedPointer<QEnginioCollectionObject> aCollection,
        QJsonObject aJsonObject)
    : iCollection(aCollection), iJsonObject(aJsonObject)
{

}
QEnginioObjectObject::~QEnginioObjectObject()
{

}

bool QEnginioObjectObject::isValid() const
{
    return true;
}

void QEnginioObjectObject::insert(const QString &aKey, const QJsonValue &aValue)
{
    iJsonObject.insert(aKey, aValue);
}
void QEnginioObjectObject::remove(const QString &aKey)
{
    iJsonObject.remove(aKey);
}
bool QEnginioObjectObject::contains(const QString &aKey) const
{
    return iJsonObject.contains(aKey);
}
QJsonValue QEnginioObjectObject::value(const QString &aKey) const
{
    return iJsonObject.value(aKey);
}
QJsonValueRef QEnginioObjectObject::valueRef(const QString &aKey)
{
    return iJsonObject[aKey];
}

const QJsonObject QEnginioObjectObject::jsonObject() const
{
    return iJsonObject;
}

const QString QEnginioObjectObject::objectId() const
{
    return value(QtCloudServicesConstants::id).toString();
}
const QString QEnginioObjectObject::objectType() const
{
    return value(QtCloudServicesConstants::objectType).toString();
}
const QTime QEnginioObjectObject::createAt() const
{
    return iCreatedAt;
}
const QEnginioUser QEnginioObjectObject::creator() const
{
    return iCreator;
}
const QTime QEnginioObjectObject::updatedAt() const
{
    return iUpdatedAt;
}
const QEnginioUser QEnginioObjectObject::updater() const
{
    return iUpdater;
}

QSharedPointer<QEnginioObjectObject> QEnginioObjectObject::get(QSharedPointer<QEnginioCollectionObject> aCollection,
        QJsonObject aJsonObject)
{
    QSharedPointer<QEnginioObjectObject> obj;
    obj = QSharedPointer<QEnginioObjectObject>(new QEnginioObjectObject(aCollection, aJsonObject));


    return obj;
}



/*
** Private Implementation
*/

QEnginioObjectPrivate::QEnginioObjectPrivate()
    : iObject()
{

}

bool QEnginioObjectPrivate::isValid() const
{
    if (iObject) {
        return iObject->isValid();
    }

    return false;
}

void QEnginioObjectPrivate::insert(const QString &aKey, const QJsonValue &aValue)
{
    if (!iObject) {
        iObject = QSharedPointer<QEnginioObjectObject>(new QEnginioObjectObject());
    }

    iObject->insert(aKey, aValue);
}
void QEnginioObjectPrivate::remove(const QString &aKey)
{
    if (iObject) {
        iObject->remove(aKey);
    }
}
bool QEnginioObjectPrivate::contains(const QString &aKey) const
{
    if (iObject) {
        return iObject->contains(aKey);
    }

    return false;
}
QJsonValue QEnginioObjectPrivate::value(const QString &aKey) const
{
    if (!iObject) {
        const_cast<QEnginioObjectPrivate *>(this)->iObject
            = QSharedPointer<QEnginioObjectObject>(new QEnginioObjectObject());
    }

    return iObject->value(aKey);
}
QJsonValueRef QEnginioObjectPrivate::valueRef(const QString &aKey)
{
    if (!iObject) {
        iObject = QSharedPointer<QEnginioObjectObject>(new QEnginioObjectObject());
    }

    return iObject->valueRef(aKey);
}

const QJsonObject QEnginioObjectPrivate::jsonObject() const
{
    if (iObject) {
        return iObject->jsonObject();
    }

    return QJsonObject();
}

const QString QEnginioObjectPrivate::objectId() const
{
    if (iObject) {
        return iObject->objectId();
    }

    return QString();
}
const QTime QEnginioObjectPrivate::createAt() const
{
    if (iObject) {
        return iObject->createAt();
    }

    return QTime();
}
const QEnginioUser QEnginioObjectPrivate::creator() const
{
    if (iObject) {
        return iObject->creator();
    }

    return QEnginioUser();
}
const QString QEnginioObjectPrivate::objectType() const
{
    if (iObject) {
        return iObject->objectType();
    }

    return QString();
}
const QTime QEnginioObjectPrivate::updatedAt() const
{
    if (iObject) {
        return iObject->updatedAt();
    }

    return QTime();
}
const QEnginioUser QEnginioObjectPrivate::updater() const
{
    if (iObject) {
        return iObject->updater();
    }

    return QEnginioUser();
}

QSharedPointer<QEnginioObjectObject> QEnginioObjectPrivate::enginioObjectObject() const
{
    return iObject;
}

void QEnginioObjectPrivate::setEnginioObjectObject(QSharedPointer<QEnginioObjectObject> aObject)
{
    iObject = aObject;
}


/*
** Public Interface
*/
QEnginioObject::QEnginioObject(QEnginioObjectPrivate &dd, QObject *aParent)
    : QCloudServicesObject(dd, aParent)
{
}
QEnginioObject::QEnginioObject(QObject *aParent)
    : QCloudServicesObject(*new QEnginioObjectPrivate(), aParent)
{

}
QEnginioObject::QEnginioObject(const QEnginioObject &aOther)
    : QCloudServicesObject(*new QEnginioObjectPrivate())
{
    *this = aOther;
}

QEnginioObject::QEnginioObject(const QJsonObject &aJsonObject)
    : QCloudServicesObject(*new QEnginioObjectPrivate())
{
    QTC_D(QEnginioObject);
    QSharedPointer<QEnginioObjectObject> obj(new QEnginioObjectObject(QSharedPointer<QEnginioCollectionObject>(), aJsonObject));
    d->setEnginioObjectObject(obj);
}

QEnginioObject& QEnginioObject::operator=(const QEnginioObject &aOther)
{

    QTC_D(QEnginioObject);
    QEnginioObjectPrivate *other;

    other = reinterpret_cast<QEnginioObjectPrivate *>(QTC_D_PTR(&aOther));

    if (other) {
        d->setEnginioObjectObject(other->enginioObjectObject());
    }

    return *this;
}

bool QEnginioObject::isValid() const
{
    QTC_D(const QEnginioObject);
    return d->isValid();
}

QEnginioObject& QEnginioObject::insert(const QString &aKey, const QJsonValue &aValue)
{
    QTC_D(QEnginioObject);
    d->insert(aKey, aValue);
    return *this;
}
QEnginioObject& QEnginioObject::remove(const QString &aKey)
{
    QTC_D(QEnginioObject);
    d->remove(aKey);
    return *this;
}
bool QEnginioObject::contains(const QString &aKey) const
{
    QTC_D(const QEnginioObject);
    return d->contains(aKey);
}
QJsonValue QEnginioObject::value(const QString &aKey) const
{
    QTC_D(const QEnginioObject);
    return d->value(aKey);
}
QJsonValue QEnginioObject::operator[](const QString &aKey) const
{
    QTC_D(const QEnginioObject);
    return d->value(aKey);
}
QJsonValueRef QEnginioObject::operator[](const QString &aKey)
{
    QTC_D(QEnginioObject);
    return d->valueRef(aKey);
}

const QJsonObject QEnginioObject::jsonObject() const
{
    QTC_D(const QEnginioObject);
    return d->jsonObject();
}
const QString QEnginioObject::objectId() const
{
    QTC_D(const QEnginioObject);
    return d->objectId();
}
const QTime QEnginioObject::createAt() const
{
    QTC_D(const QEnginioObject);
    return d->createAt();
}
const QEnginioUser QEnginioObject::creator() const
{
    QTC_D(const QEnginioObject);
    return d->creator();
}
const QString QEnginioObject::objectType() const
{
    QTC_D(const QEnginioObject);
    return d->objectType();
}
const QTime QEnginioObject::updatedAt() const
{
    QTC_D(const QEnginioObject);
    return d->updatedAt();
}
const QEnginioUser QEnginioObject::updater() const
{
    QTC_D(const QEnginioObject);
    return d->updater();
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, QEnginioObject aObject)
{
    if (!aObject.isValid()) {
        d << "QEnginioObject(null)";
        return d;
    }

    d.nospace();
    d << "QEnginioObject("; // << hex << (void *)aReply << dec;
    d << "objectId = " << aObject.objectId() << ", ";
    d << "createAt = " << aObject.createAt(); //  << ", ";

    /*
    const QTime createAt() const Q_REQUIRED_RESULT;
    const QEnginioUser creator() const Q_REQUIRED_RESULT;
    const QString objectType() const Q_REQUIRED_RESULT;
    const QTime updatedAt() const Q_REQUIRED_RESULT;
    const QEnginioUser updater() const Q_REQUIRED_RESULT;
    */
    d << ")";

    return d.space();
}

#endif /* QT_NO_DEBUG_STREAM */

QT_END_NAMESPACE

