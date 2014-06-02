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

#include "QtCloudServices/private/qenginiodatastorageshared_p.h"

#include "QtCloudServices/QEnginioConnectionObject"
#include "QtCloudServices/QEnginioOperationObject"

QT_BEGIN_NAMESPACE

QEnginioDataStorageShared::QEnginioDataStorageShared() {

}

QEnginioDataStorageShared::QEnginioDataStorageShared(const QUrl &aInstanceAddress, const QString &aBackendId,
                                                     QEnginioDataStorageShared *aPrevInstance)
    : QRestEndpointShared(aInstanceAddress),
      iBackendId(aBackendId)
{
    static bool firstInstance = true;
    if (firstInstance) {
        firstInstance=false;
        qRegisterMetaType<QEnginioConnectionObject *>();
        qRegisterMetaType<QEnginioOperationObject*>();

        /*
        qRegisterMetaType<EnginioModel*>();
        // qRegisterMetaType<EnginioIdentity*>();
        // qRegisterMetaType<EnginioOAuth2Authentication*>();
        */
        qRegisterMetaType<QtCloudServices::Operation>();
        qRegisterMetaType<QtCloudServices::AuthenticationState>();
        qRegisterMetaType<QtCloudServices::ErrorType>();
    }

    if (aPrevInstance)  {
        iUsername = aPrevInstance->iUsername;
        iPassword = aPrevInstance->iPassword;
    }
}

QEnginioDataStorageShared::~QEnginioDataStorageShared()
{
}

bool QEnginioDataStorageShared::isValid() const
{
    if (!QRestEndpointShared::isValid()) {
        return false;
    }

    if (backendId().isEmpty()) {
        return false;
    }

    return true;
}

QUrl QEnginioDataStorageShared::instanceAddress() const
{
    return endpointAddress();
}

QString QEnginioDataStorageShared::backendId() const
{
    return iBackendId;
}

QString QEnginioDataStorageShared::username() const
{
    return iUsername;
}

void QEnginioDataStorageShared::setUsername(const QString &aUsername)
{
    if (iUsername == aUsername) {
        return;
    }

    iUsername = aUsername;

    emit usernameChanged(aUsername);
}

QString QEnginioDataStorageShared::password() const
{
    return iPassword;
}

void QEnginioDataStorageShared::setPassword(const QString &aPassword)
{
    if (iPassword == aPassword) {
        return;
    }

    iPassword = aPassword;

    emit passwordChanged(aPassword);
}

QSharedPointer<QEnginioCollectionShared>
QEnginioDataStorageShared::collection(QSharedPointer<QEnginioDataStorageShared> aSelf,
                                      const QString &aCollectionName)
{
    QMutexLocker locker(&iLock);

    QSharedPointer<QEnginioCollectionShared> collection;
    QMap<QString, QSharedPointer<QEnginioCollectionShared> >::iterator i;

    i = iCollections.find(aCollectionName);

    if (i != iCollections.end()) {
        collection = i.value();
    } else {
        collection=QSharedPointer<QEnginioCollectionShared>(new QEnginioCollectionShared(aSelf,aCollectionName));
        iCollections.insert(aCollectionName, collection);
    }

    return collection;
}

QSharedPointer<QEnginioConnectionShared> QEnginioDataStorageShared::reserveConnection(QSharedPointer<QEnginioDataStorageShared> aSelf) {
    QSharedPointer<QRestConnectionShared> con;
    con = QRestEndpointShared::reserveConnection(aSelf);
    return qSharedPointerCast<QEnginioConnectionShared>(con);
}

void QEnginioDataStorageShared::releaseConnection(QSharedPointer<QEnginioConnectionShared> aConnection) {
    QRestEndpointShared::releaseConnection(aConnection);
}


QSharedPointer<QRestConnectionShared>
QEnginioDataStorageShared::buildConnectionInstance(QSharedPointer<QRestEndpointShared> aSelf) {
    return QSharedPointer<QRestConnectionShared>(new QEnginioConnectionShared(qSharedPointerCast<QEnginioDataStorageShared>(aSelf)));
}

QT_END_NAMESPACE
