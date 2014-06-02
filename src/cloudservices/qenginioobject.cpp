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

#include "QtCloudServices/qenginioobject.h"
#include "QtCloudServices/qenginiouser.h"

#include "QtCloudServices/private/qenginioobjectobject_p.h"

QT_BEGIN_NAMESPACE

QEnginioObject::QEnginioObject(QEnginioObjectObject *aObject)
    : iObject(aObject)
{
    Q_ASSERT(iObject);
}

QEnginioObject::QEnginioObject()
    : iObject(new QEnginioObjectObject)
{

}
QEnginioObject::QEnginioObject(const QEnginioObject &aOther)
    : iObject(new QEnginioObjectObject)
{
    object()->setSharedInstanceFrom(aOther.object());
}

QEnginioObject::QEnginioObject(const QJsonObject &aJsonObject)
    : iObject(new QEnginioObjectObject(aJsonObject))
{

}

QEnginioObject::~QEnginioObject()
{
    if (iObject) {
        delete iObject;
    }
}

QEnginioObject& QEnginioObject::operator=(const QEnginioObject &aOther) {
    object()->setSharedInstanceFrom(aOther.object());
    return *this;
}

// IsValid
bool QEnginioObject::isValid() const {
    if (!iObject) {
        return false;
    }

    return iObject->isValid();
}

// Status
bool QEnginioObject::isPersistent() const {
    return iObject->isPersistent();
}
bool QEnginioObject::isModified() const {
    return iObject->isModified();
}

QEnginioObject& QEnginioObject::insert(const QString &aKey, const QJsonValue &aValue) {
    iObject->insert(aKey,aValue);
    return *this;
}

QEnginioObject& QEnginioObject::remove(const QString &aKey) {
    iObject->remove(aKey);
    return *this;
}

bool QEnginioObject::contains(const QString &aKey) const {
    return iObject->contains(aKey);
}
QJsonValue QEnginioObject::value(const QString &aKey) const {
    return iObject->value(aKey);
}
QJsonValue QEnginioObject::operator[](const QString &aKey) const {
    return iObject->value(aKey);
}
QJsonValueRef QEnginioObject::operator[](const QString &aKey) {
    return iObject->valueRef(aKey);
}

const QJsonObject QEnginioObject::jsonObject() const {
    return iObject->jsonObject();
}

const QString QEnginioObject::objectId() const {
    return iObject->objectId();
}
const QString QEnginioObject::objectType() const {
    return iObject->objectType();
}
const QTime QEnginioObject::createAt() const {
    return iObject->createAt();
}
const QEnginioUser QEnginioObject::creator() const {
    return QEnginioUser(iObject->creator());
}
const QTime QEnginioObject::updatedAt() const {
    return iObject->updatedAt();
}
const QEnginioUser QEnginioObject::updater() const {
    return QEnginioUser(iObject->updater());
}

void QEnginioObject::save() {
    iObject->save();
}

#ifndef QT_NO_DEBUG_STREAM
void QEnginioObject::dumpDebugInfo(QDebug &d) const {
    d.nospace();
    d << "QEnginioObject("; // << hex << (void *)aReply << dec;
    d << "objectId = " << objectId() << ", ";
    d << "createAt = " << createAt(); //  << ", ";

    /*
    const QTime createAt() const Q_REQUIRED_RESULT;
    const QEnginioUser creator() const Q_REQUIRED_RESULT;
    const QString objectType() const Q_REQUIRED_RESULT;
    const QTime updatedAt() const Q_REQUIRED_RESULT;
    const QEnginioUser updater() const Q_REQUIRED_RESULT;
    */
    d << ")";

    d.space();
}
#endif

const QEnginioObjectObject* QEnginioObject::object() const {
    return iObject;
}
QEnginioObjectObject* QEnginioObject::object() {
    return iObject;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, const QEnginioObject &aObject)
{
    if (!aObject.isValid()) {
        d << "QEnginioObject(null)";
    } else {
        aObject.dumpDebugInfo(d);
    }
    return d;
}

#endif /* QT_NO_DEBUG_STREAM */

QT_END_NAMESPACE

