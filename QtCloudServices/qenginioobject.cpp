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

QT_BEGIN_NAMESPACE

/*
** Shared Implementation
*/
QEnginioObjectObject::QEnginioObjectObject(QSharedPointer<QEnginioCollectionObject> aCollection)
    : iCollection(aCollection)
{

}
QEnginioObjectObject::~QEnginioObjectObject()
{

}

bool QEnginioObjectObject::isValid() const
{
    return true;
}

const QJsonObject QEnginioObjectObject::jsonObject() const
{
    return iJsonObject;
}

const QString QEnginioObjectObject::objectId() const
{
    return iObjectId;
}
const QTime QEnginioObjectObject::createAt() const
{
    return iCreatedAt;
}
const QEnginioUser QEnginioObjectObject::creator() const
{
    return iCreator;
}
const QString QEnginioObjectObject::objectType() const
{
    return iObjectType;
}
const QTime QEnginioObjectObject::updatedAt() const
{
    return iUpdatedAt;
}
const QEnginioUser QEnginioObjectObject::updater() const
{
    return iUpdater;
}

QSharedPointer<QEnginioObjectObject> QEnginioObjectObject::get(QSharedPointer<QEnginioCollectionObject> aCollection)
{
    QSharedPointer<QEnginioObjectObject> obj;
    obj = QSharedPointer<QEnginioObjectObject>(new QEnginioObjectObject(aCollection));


    return obj;
}




/*
** Private Implementation
*/

QEnginioObjectPrivate::QEnginioObjectPrivate()
{

}

bool QEnginioObjectPrivate::isValid() const
{
    if (iObject) {
        return iObject->isValid();
    }

    return false;
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

QT_END_NAMESPACE

