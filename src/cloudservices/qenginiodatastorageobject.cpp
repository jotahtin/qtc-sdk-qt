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

#include "QtCloudServices/private/qenginiodatastorageobject_p.h"
#include "QtCloudServices/private/qenginiodatastorageshared_p.h"
#include "QtCloudServices/private/qenginiocollection_p.h"
#include "QtCloudServices/private/qenginioconnection_p.h"

QT_BEGIN_NAMESPACE

/*
** Private
*/

QEnginioDataStorageObjectPrivate::QEnginioDataStorageObjectPrivate()
    : iShared(new QEnginioDataStorageShared)
{
}

QEnginioDataStorageObjectPrivate::QEnginioDataStorageObjectPrivate(const QUrl &aInstanceAddress, const QString &aBackendId)
    : iShared(new QEnginioDataStorageShared(aInstanceAddress,aBackendId))
{
}

QEnginioDataStorageObjectPrivate::~QEnginioDataStorageObjectPrivate()
{
    if (iShared) {
        deinit();
    }
}

void QEnginioDataStorageObjectPrivate::init() {
    Q_Q(QEnginioDataStorageObject);
    iConnectionUsernameChanged
            = QObject::connect(iShared.data(), &QEnginioDataStorageShared::usernameChanged,
                      q, &QEnginioDataStorageObject::usernameChanged);
    iConnectionPasswordChanged
    = QObject::connect(iShared.data(), &QEnginioDataStorageShared::passwordChanged,
              q, &QEnginioDataStorageObject::passwordChanged);
    iConnectionOperationError
    = QObject::connect(iShared.data(), &QEnginioDataStorageShared::operationError,
              q, &QEnginioDataStorageObject::operationError);
}

void QEnginioDataStorageObjectPrivate::deinit() {
    QObject::disconnect(iConnectionUsernameChanged);
    QObject::disconnect(iConnectionPasswordChanged);
    QObject::disconnect(iConnectionOperationError);
}

bool QEnginioDataStorageObjectPrivate::isValid() const
{
    if (iShared.isNull()) {
        return false;
    }

    return iShared->isValid();
}

void QEnginioDataStorageObjectPrivate::setSharedInstance(const QEnginioDataStorageObject *aOther) {
   if (iShared) {
       deinit();
   }

   iShared = aOther->d_func()->sharedInstance();

   init();
}

void QEnginioDataStorageObjectPrivate::setBackend(const QUrl &aInstanceAddress, const QString &aBackendId)
{
    Q_Q(QEnginioDataStorageObject);
    bool chgAddress, chgId;

    chgAddress = (instanceAddress() != aInstanceAddress);
    chgId = (backendId() != aBackendId);

    if (!chgAddress && !chgId) {
        return;
    }

    if (iShared) {
        deinit();
    }
    iShared = QSharedPointer<QEnginioDataStorageShared>
            (new QEnginioDataStorageShared(aInstanceAddress,aBackendId));
    init();

    if (chgAddress) {
        emit q->instanceAddressChanged(aInstanceAddress);
    }

    if (chgId) {
        emit q->backendIdChanged(aBackendId);
    }

    emit q->backendChanged();
}

QUrl QEnginioDataStorageObjectPrivate::instanceAddress() const
{
    return iShared->instanceAddress();
}

QString QEnginioDataStorageObjectPrivate::backendId() const
{
    return iShared->backendId();
}

QString QEnginioDataStorageObjectPrivate::username() const
{
    return iShared->username();
}

QString QEnginioDataStorageObjectPrivate::password() const
{
    return iShared->password();
}

void QEnginioDataStorageObjectPrivate::setUsername(const QString &aUsername)
{
    iShared->setUsername(aUsername);
}

void QEnginioDataStorageObjectPrivate::setPassword(const QString &aPassword)
{
    iShared->setPassword(aPassword);
}

QEnginioCollection QEnginioDataStorageObjectPrivate::collection(const QString &aCollectionName)
{
    return iShared->collection(aCollectionName);
}

QEnginioConnection QEnginioDataStorageObjectPrivate::reserveConnection()
{
    return iShared->reserveConnection(iShared);
}
void QEnginioDataStorageObjectPrivate::releaseConnection(const QEnginioConnection &aConnection)
{
    iShared->releaseConnection(aConnection);
}

QSharedPointer<QEnginioDataStorageShared> QEnginioDataStorageObjectPrivate::sharedInstance() const {
    return iShared;
}


/*
** Public
*/

QEnginioDataStorageObject::QEnginioDataStorageObject(QObject *aParent)
    : QObject(*new QEnginioDataStorageObjectPrivate(),aParent)
{
    Q_D(QEnginioDataStorageObject);
    d->init();
}

QEnginioDataStorageObject::QEnginioDataStorageObject(const QUrl &aInstanceAddress, const QString &aBackendId, QObject *aParent)
    : QObject(*new QEnginioDataStorageObjectPrivate(aInstanceAddress,aBackendId),aParent)
{
    Q_D(QEnginioDataStorageObject);
    d->init();
}

QEnginioDataStorageObject::~QEnginioDataStorageObject() {

}

bool QEnginioDataStorageObject::isValid() const {
    Q_D(const QEnginioDataStorageObject);
    return d->isValid();
}

void QEnginioDataStorageObject::setBackend(const QEnginioDataStorageObject *aOther) {
    Q_D(QEnginioDataStorageObject);
    d->setBackend(aOther);
}

void QEnginioDataStorageObject::setBackend(const QUrl &aInstanceAddress, const QString &aBackendId) {
    Q_D(QEnginioDataStorageObject);
    d->setBackend(aInstanceAddress,aBackendId);
}

QUrl QEnginioDataStorageObject::instanceAddress() const {
    Q_D(const QEnginioDataStorageObject);
    return d->instanceAddress();
}

void QEnginioDataStorageObject::setInstanceAddress(const QUrl &aInstanceAddress) {
    Q_D(QEnginioDataStorageObject);
    d->setBackend(aInstanceAddress, backendId());
}

QString QEnginioDataStorageObject::backendId() const {
    Q_D(const QEnginioDataStorageObject);
    return d->backendId();
}

void QEnginioDataStorageObject::setBackendId(const QString &aBackendId) {
    Q_D(QEnginioDataStorageObject);
    d->setBackend(instanceAddress(), aBackendId);
}

// Authentication
QString QEnginioDataStorageObject::username() const {
    Q_D(const QEnginioDataStorageObject);
    return d->username();
}
QString QEnginioDataStorageObject::password() const {
    Q_D(const QEnginioDataStorageObject);
    return d->password();
}

// Get object collection
QEnginioCollection QEnginioDataStorageObject::collection(const QString &aCollectionName) {
    Q_D(QEnginioDataStorageObject);
    return d->collection(aCollectionName);
}

QEnginioConnection QEnginioDataStorageObject::reserveConnection() {
    Q_D(QEnginioDataStorageObject);
    return d->reserveConnection();
}

void QEnginioDataStorageObject::releaseConnection(const QEnginioConnection &aConnection) {
    Q_D(QEnginioDataStorageObject);
    d->releaseConnection(aConnection);
}

void QEnginioDataStorageObject::setUsername(const QString &aUsername) {
    Q_D(QEnginioDataStorageObject);
    d->setUsername(aUsername);
}

void QEnginioDataStorageObject::setPassword(const QString &aPassword) {
    Q_D(QEnginioDataStorageObject);
    d->setPassword(aPassword);
}

QT_END_NAMESPACE
