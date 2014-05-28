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
** Private Implementation
*/

QEnginioObjectPrivate::QEnginioObjectPrivate()
{

}

QEnginioObjectPrivate::QEnginioObjectPrivate(const QJsonObject & aJsonObject)
    : iJsonObject(aJsonObject)
{
}

/*
QEnginioObjectPrivate::QEnginioObjectPrivate(const QEnginioCollection &aEnginioCollection,
        const QJsonObject & aJsonObject)
    : iEnginioCollection(aEnginioCollection), iJsonObject(aJsonObject)
{

}
*/


bool QEnginioObjectPrivate::isValid() const
{
    return true;
}

bool QEnginioObjectPrivate::isModified() const
{
    QJsonObject::const_iterator i;

    for (i = iJsonObject.begin(); i != iJsonObject.end(); ++i) {
        if (i.key() == QtCloudServicesConstants::id ||
                i.key() == QtCloudServicesConstants::objectType) {
            continue;
        }

        if (!iPersistentJsonObject.contains(i.key())) {
            return true; // field added
        }

        if (iPersistentJsonObject.value(i.key()) != i.value()) {
            return true; // field changed
        }
    }

    for (i = iPersistentJsonObject.begin(); i != iPersistentJsonObject.end(); ++i) {
        if (!iPersistentJsonObject.contains(i.key())) {
            return true; // field removed
        }
    }

    return false;
}

void QEnginioObjectPrivate::insert(const QString &aKey, const QJsonValue &aValue)
{
    iJsonObject.insert(aKey, aValue);
}
void QEnginioObjectPrivate::remove(const QString &aKey)
{
    iJsonObject.remove(aKey);
}
bool QEnginioObjectPrivate::contains(const QString &aKey) const
{
    return iJsonObject.contains(aKey);
}
QJsonValue QEnginioObjectPrivate::value(const QString &aKey) const
{
    return iJsonObject.value(aKey);
}
QJsonValueRef QEnginioObjectPrivate::valueRef(const QString &aKey)
{
    return iJsonObject[aKey];
}

const QJsonObject QEnginioObjectPrivate::jsonObject() const
{
    return iJsonObject;
}

const QString QEnginioObjectPrivate::objectId() const
{
    return value(QtCloudServicesConstants::id).toString();
}
const QString QEnginioObjectPrivate::objectType() const
{
    return value(QtCloudServicesConstants::objectType).toString();
}
const QTime QEnginioObjectPrivate::createAt() const
{
    return iCreatedAt;
}
const QEnginioUser QEnginioObjectPrivate::creator() const
{
    return iCreator;
}
const QTime QEnginioObjectPrivate::updatedAt() const
{
    return iUpdatedAt;
}
const QEnginioUser QEnginioObjectPrivate::updater() const
{
    return iUpdater;
}

QEnginioOperation QEnginioObjectPrivate::save()
{
    if (!isModified()) {
        return QEnginioOperation();
    }

    QJsonObject update;
    QJsonObject::iterator i;

    for (i = iJsonObject.begin(); i != iJsonObject.end(); ++i) {
        if (i.key() == QtCloudServicesConstants::id ||
                i.key() == QtCloudServicesConstants::objectType) {
            update.insert(i.key(), i.value());
            continue;
        }

        if (iPersistentJsonObject.contains(i.key()) &&
                iPersistentJsonObject[i.key()] == i.value()) {
            continue;
        }

        update.insert(i.key(), i.value());
    }

    QEnginioObject::dvar self = getThis<QEnginioObject>();
    return iEnginioCollection.update(objectId(), update,
    [ = ](QEnginioOperation & op) {
        self->saveCompleted(op);
    });
}

void QEnginioObjectPrivate::setEnginioCollection(const QEnginioCollection &aEnginioCollection)
{
    iEnginioCollection = aEnginioCollection;
}

void QEnginioObjectPrivate::setUpdatedContent(const QJsonObject &aJsonObject)
{
    iJsonObject = aJsonObject;
    markAsSynced();
}

void QEnginioObjectPrivate::markAsSynced()
{
    iPersistentJsonObject = iJsonObject;
    emit objectChanged();
}

void QEnginioObjectPrivate::saveCompleted(QEnginioOperation & op)
{
    if (!op) {
        emit operationFailed(op.errorString());
    } else {
        markAsSynced();
    }
}

/*
** Public Interface
*/
QEnginioObject::QEnginioObject(QEnginioObject::dvar aPIMPL, QObject *aParent)
    : QCloudServicesObject(aPIMPL, aParent)
{
}
QEnginioObject::QEnginioObject(QObject *aParent)
    : QCloudServicesObject(aParent)
{

}
QEnginioObject::QEnginioObject(const QEnginioObject &aOther)
{
    if (!aOther.isNull()) {
        setPIMPL(aOther.d<QEnginioObject>());
    }
}

QEnginioObject::QEnginioObject(const QJsonObject &aJsonObject)
    : QCloudServicesObject(QEnginioObject::dvar(new QEnginioObjectPrivate(aJsonObject)))
{
}

QEnginioObject & QEnginioObject::operator=(const QEnginioObject &aOther)
{
    if (!aOther.isNull()) {
        setPIMPL(aOther.d<QEnginioObject>());
    } else {
        setPIMPL(nullptr);
    }

    return *this;
}

bool QEnginioObject::isValid() const
{
    if (isNull()) {
        return false;
    }

    return d<const QEnginioObject>()->isValid();
}

bool QEnginioObject::isPersistent() const
{
    if (!isValid()) {
        return false;
    }

    return !objectId().isEmpty();
}

bool QEnginioObject::isModified() const
{
    if (isNull()) {
        return false;
    }

    return d<const QEnginioObject>()->isModified();
}

QEnginioObject & QEnginioObject::insert(const QString &aKey, const QJsonValue &aValue)
{
    if (isNull()) {
        lazyInitialization();
    }

    d<QEnginioObject>()->insert(aKey, aValue);
    return *this;
}
QEnginioObject & QEnginioObject::remove(const QString &aKey)
{
    if (!isNull()) {
        d<QEnginioObject>()->remove(aKey);
    }

    return *this;
}
bool QEnginioObject::contains(const QString &aKey) const
{
    if (isNull()) {
        return false;
    }

    return d<const QEnginioObject>()->contains(aKey);
}
QJsonValue QEnginioObject::value(const QString &aKey) const
{
    if (isNull()) {
        return QJsonValue();
    }

    return d<const QEnginioObject>()->value(aKey);
}
QJsonValue QEnginioObject::operator[](const QString &aKey) const
{
    if (isNull()) {
        return QJsonValue();
    }

    return d<const QEnginioObject>()->value(aKey);
}
QJsonValueRef QEnginioObject::operator[](const QString &aKey)
{
    if (isNull()) {
        lazyInitialization();
    }

    return d<QEnginioObject>()->valueRef(aKey);
}

const QJsonObject QEnginioObject::jsonObject() const
{
    if (isNull()) {
        return QJsonObject();
    }

    return d<const QEnginioObject>()->jsonObject();
}
const QString QEnginioObject::objectId() const
{
    if (isNull()) {
        return QString();
    }

    return d<const QEnginioObject>()->objectId();
}
const QTime QEnginioObject::createAt() const
{
    if (isNull()) {
        return QTime();
    }

    return d<const QEnginioObject>()->createAt();
}
const QEnginioUser QEnginioObject::creator() const
{
    if (isNull()) {
        return QEnginioUser();
    }

    return d<const QEnginioObject>()->creator();
}
const QString QEnginioObject::objectType() const
{
    if (isNull()) {
        return QString();
    }

    return d<const QEnginioObject>()->objectType();
}
const QTime QEnginioObject::updatedAt() const
{
    if (isNull()) {
        return QTime();
    }

    return d<const QEnginioObject>()->updatedAt();
}
const QEnginioUser QEnginioObject::updater() const
{
    if (isNull()) {
        return QEnginioUser();
    }

    return d<const QEnginioObject>()->updater();
}

void QEnginioObject::save()
{
    if (isNull()) {
        return;
    }

    d<const QEnginioObject>()->save();
}

void QEnginioObject::lazyInitialization()
{
    if (isNull()) {
        setPIMPL(QEnginioObject::dvar(new QEnginioObjectPrivate));
    }
}

void QEnginioObject::setPIMPL(QCloudServicesObject::dvar aPIMPL)
{
    QCloudServicesObject::setPIMPL(aPIMPL);

    if (isNull()) {
        return;
    }

    connect(d<QEnginioObject>().get(), SIGNAL(objectChanged()),
            this, SIGNAL(objectChanged()));
    connect(d<QEnginioObject>().get(), SIGNAL(operationFailed(QString)),
            this, SIGNAL(operationFailed(QString)));
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

