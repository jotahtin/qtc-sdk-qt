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

#include "QtCloudServices/private/qmanagedwebsocketobject_p.h"
#include "QtCloudServices/private/qmanagedwebsocketshared_p.h"
#include "QtCloudServices/private/qmanagedwebsocketconnectionshared_p.h"
#include "QtCloudServices/private/qmanagedwebsocketconnectionobject_p.h"


QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/

QManagedWebSocketObjectPrivate::QManagedWebSocketObjectPrivate()
{
}

void QManagedWebSocketObjectPrivate::setGateway(const QUrl &aInstanceAddress, const QString &aGatewayId)
{
    Q_Q(QManagedWebSocketObject);
    bool chgAddress, chgId;

    chgAddress = (endpointAddress() != aInstanceAddress);
    chgId = (gatewayId() != aGatewayId);

    if (!chgAddress && !chgId) {
        return;
    }

    QSharedPointer<QManagedWebSocketShared> shared
            (new QManagedWebSocketShared(aInstanceAddress,aGatewayId));
    setSharedInstance(shared);

    if (chgAddress) {
        emit q->instanceAddressChanged(aInstanceAddress);
    }

    if (chgId) {
        emit q->gatewayIdChanged(aGatewayId);
    }

    emit q->gatewayChanged();
}

void QManagedWebSocketObjectPrivate::setEndpointAddress(const QUrl &aEndpointAddress)
{
    setGateway(aEndpointAddress, gatewayId());
}

void QManagedWebSocketObjectPrivate::setGatewayId(const QString &aBackendId)
{
    setGateway(endpointAddress(), aBackendId);
}

QString QManagedWebSocketObjectPrivate::gatewayId() const
{
    QSharedPointer<const QManagedWebSocketShared> shared;
    shared = qSharedPointerCast<const QManagedWebSocketShared>(sharedInstance());
    if (shared.isNull()) {
        return QString();
    }

    return shared->gatewayId();
}

QString QManagedWebSocketObjectPrivate::secret() const {
    QSharedPointer<const QManagedWebSocketShared> shared;
    shared = qSharedPointerCast<const QManagedWebSocketShared>(sharedInstance());
    if (shared.isNull()) {
        return QString();
    }
    return shared->secret();
}

void QManagedWebSocketObjectPrivate::setSecret(const QString &aSecret)
{
    QSharedPointer<QManagedWebSocketShared> shared;
    shared = qSharedPointerCast<QManagedWebSocketShared>(sharedInstance());
    shared->setSecret(aSecret);
}



void QManagedWebSocketObjectPrivate::connectSocket()
{
    QSharedPointer<QManagedWebSocketShared> shared;
    shared = qSharedPointerCast<QManagedWebSocketShared>(sharedInstance());

    shared->connectSocket();

}
void QManagedWebSocketObjectPrivate::disconnectSocket()
{
    QSharedPointer<QManagedWebSocketShared> shared;
    shared = qSharedPointerCast<QManagedWebSocketShared>(sharedInstance());

    shared->disconnectSocket();
}

QRestConnectionObject* QManagedWebSocketObjectPrivate::buildConnectionObject() const {
    return new QManagedWebSocketConnectionObject;
}

void QManagedWebSocketObjectPrivate::init() {
    Q_Q(QManagedWebSocketObject);

    QRestEndpointObjectPrivate::init();

    QSharedPointer<QManagedWebSocketShared> shared;
    shared = qSharedPointerCast<QManagedWebSocketShared>(sharedInstance());

    iConnectionSecretChanged
            = QObject::connect(shared.data(), &QManagedWebSocketShared::secretChanged,
                      q, &QManagedWebSocketObject::secretChanged);
    iConnectionOperationError
    = QObject::connect(shared.data(), &QManagedWebSocketShared::operationError,
              q, &QManagedWebSocketObject::operationError);

}

void QManagedWebSocketObjectPrivate::deinit() {
    QObject::disconnect(iConnectionSecretChanged);
    QObject::disconnect(iConnectionOperationError);

    QRestEndpointObjectPrivate::deinit();
}

/*
** Public Interface
*/
QManagedWebSocketObject::QManagedWebSocketObject(QObject *aParent)
    : QRestEndpointObject(*new QManagedWebSocketObjectPrivate, aParent)
{
}

QManagedWebSocketObject::QManagedWebSocketObject(const QUrl &aInstanceAddress, const QString &aGatewayId, QObject *aParent)
    : QRestEndpointObject(*new QManagedWebSocketObjectPrivate, aParent)
{
    Q_D(QManagedWebSocketObject);
    QSharedPointer<QManagedWebSocketShared> shared
            (new QManagedWebSocketShared(aInstanceAddress,aGatewayId));
    d->setSharedInstance(shared);
}

void QManagedWebSocketObject::setGateway(const QUrl &aInstanceAddress, const QString &aGatewayId)
{
    Q_D(QManagedWebSocketObject);
    d->setGateway(aInstanceAddress,aGatewayId);
}

QString QManagedWebSocketObject::gatewayId() const
{
    Q_D(const QManagedWebSocketObject);
    return d->gatewayId();
}
void QManagedWebSocketObject::setGatewayId(const QString &aGatewayId)
{
    Q_D(QManagedWebSocketObject);
    d->setGatewayId(aGatewayId);
}

QString QManagedWebSocketObject::secret() const {
    Q_D(const QManagedWebSocketObject);
    return d->secret();
}

void QManagedWebSocketObject::setSecret(const QString &aSecret) {
    Q_D(QManagedWebSocketObject);
    d->setSecret(aSecret);
}

void QManagedWebSocketObject::connectSocket()
{
    Q_D(QManagedWebSocketObject);
    d->connectSocket();

}
void QManagedWebSocketObject::disconnectSocket()
{
    Q_D(QManagedWebSocketObject);
    d->disconnectSocket();

}

QT_END_NAMESPACE
