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

#include "QtCloudServices/private/qenginioobjectshared_p.h"
#include "QtCloudServices/private/qenginiocollectionshared_p.h"
#include "QtCloudServices/private/qcloudservicesconstants_p.h"

QT_BEGIN_NAMESPACE

QEnginioObjectShared::SaveCompletedFunctor::SaveCompletedFunctor(QSharedPointer<QEnginioObjectShared> aSelf)
    : iSelf(aSelf)
{}
void QEnginioObjectShared::SaveCompletedFunctor::operator ()
    (QSharedPointer<QRestOperationShared> aOperation)
{
    QSharedPointer<QEnginioOperationShared> op;
    op=qSharedPointerCast<QEnginioOperationShared>(aOperation);
        if (op.isNull() || op->isError()) {
            emit iSelf->operationFailed(op->errorString());
        } else {
            iSelf->markAsSynced();
        }
}

QEnginioObjectShared::QEnginioObjectShared()
{

}

QEnginioObjectShared::QEnginioObjectShared(const QJsonObject & aJsonObject)
    : iJsonObject(aJsonObject)
{
}


bool QEnginioObjectShared::isValid() const
{
    return true;
}

bool QEnginioObjectShared::isPersistent() const
{
    if (!isValid()) {
        return false;
    }

    return !objectId().isEmpty();
}


bool QEnginioObjectShared::isModified() const
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

void QEnginioObjectShared::insert(const QString &aKey, const QJsonValue &aValue)
{
    iJsonObject.insert(aKey, aValue);
}
void QEnginioObjectShared::remove(const QString &aKey)
{
    iJsonObject.remove(aKey);
}
bool QEnginioObjectShared::contains(const QString &aKey) const
{
    return iJsonObject.contains(aKey);
}
QJsonValue QEnginioObjectShared::value(const QString &aKey) const
{
    return iJsonObject.value(aKey);
}
QJsonValueRef QEnginioObjectShared::valueRef(const QString &aKey)
{
    return iJsonObject[aKey];
}

const QJsonObject QEnginioObjectShared::jsonObject() const
{
    return iJsonObject;
}

QString QEnginioObjectShared::objectId() const
{
    return value(QtCloudServicesConstants::id).toString();
}
QString QEnginioObjectShared::objectType() const
{
    return value(QtCloudServicesConstants::objectType).toString();
}
QTime QEnginioObjectShared::createAt() const
{
    return iCreatedAt;
}
QSharedPointer<QEnginioUserShared> QEnginioObjectShared::creator() const
{
    return iCreator;
}
QTime QEnginioObjectShared::updatedAt() const
{
    return iUpdatedAt;
}
QSharedPointer<QEnginioUserShared> QEnginioObjectShared::updater() const
{
    return iUpdater;
}

QSharedPointer<QEnginioOperationShared> QEnginioObjectShared::save(QSharedPointer<QEnginioObjectShared> aSelf)
{
    if (!isModified()) {
        return QSharedPointer<QEnginioOperationShared>();
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

    return iEnginioCollection->update(iEnginioCollection,
                                      objectId(), update,
                                      SaveCompletedFunctor(aSelf));
}

void QEnginioObjectShared::setEnginioCollection(QSharedPointer<QEnginioCollectionShared> aEnginioCollection)
{
    iEnginioCollection = aEnginioCollection;
}

void QEnginioObjectShared::setUpdatedContent(const QJsonObject &aJsonObject)
{
    iJsonObject = aJsonObject;
    markAsSynced();
}

void QEnginioObjectShared::markAsSynced()
{
    iPersistentJsonObject = iJsonObject;
    emit objectChanged();
}

QT_END_NAMESPACE

