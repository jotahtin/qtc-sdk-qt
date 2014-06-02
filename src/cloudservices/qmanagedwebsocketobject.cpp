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


QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/

QManagedWebSocketObjectPrivate::QManagedWebSocketObjectPrivate()
{
    setSharedInstance(new QManagedWebSocketShared);
}

QManagedWebSocketObjectPrivate::QManagedWebSocketObjectPrivate(const QUrl &aInstanceAddress, const QString &aGatewayId)
{
    setSharedInstance(new QManagedWebSocketShared(aInstanceAddress,aGatewayId);
}

QManagedWebSocketObjectPrivate::~QManagedWebSocketObjectPrivate()
{
    if (iShared) {
        deinit();
    }
}

bool QManagedWebSocketObjectPrivate::isValid() const
{
    if (iShared.isNull()) {
        return false;
    }

    return iShared->isValid();
}

void QManagedWebSocketObjectPrivate::setGateway(const QUrl &aInstanceAddress, const QString &aGatewayId)
{
    Q_Q(QManagedWebSocketObject);
    bool chgAddress, chgId;

    chgAddress = (impl->instanceAddress() != aInstanceAddress);
    chgId = (impl->gatewayId() != aGatewayId);

    if (!chgAddress && !chgId) {
        return;
    }

    setSharedInstance(new QManagedWebSocketShared(aInstanceAddress, aGatewayId));

    if (chgAddress) {
        emit q->instanceAddressChanged(aInstanceAddress);
    }

    if (chgId) {
        emit q->gatewayIdChanged(aGatewayId);
    }

    emit q->gatewayChanged();
}

QUrl QManagedWebSocketObjectPrivate::instanceAddress() const
{
    return iShared->instanceAddress();
}
void QManagedWebSocketObjectPrivate::setInstanceAddress(const QUrl &aInstanceAddress)
{
    setGateway(aInstanceAddress, gatewayId());
}

QString QManagedWebSocketObjectPrivate::gatewayId() const
{
    return iShared->gatewayId();
}
void QManagedWebSocketObjectPrivate::setGatewayId(const QString &aBackendId)
{
    setGateway(instanceAddress(), aBackendId);
}

void QManagedWebSocketObjectPrivate::connectSocket()
{
    iShared->connectSocket();

}
void QManagedWebSocketObjectPrivate::disconnectSocket()
{
    iShared->disconnectSocket();
}

void QManagedWebSocketObjectPrivate::init() {

}

void QManagedWebSocketObjectPrivate::deinit() {

}

QSharedPointer<QManagedWebSocketShared> QManagedWebSocketObjectPrivate::sharedInstance() const {
    return iShared;
}
void QManagedWebSocketObjectPrivate::setSharedInstance(QSharedPointer<QManagedWebSocketShared> aShared) {
    if (iShared) {
        deinit();
    }

    iShared = aShared;

    if (iShared) {
        init();
    }
}

/*
** Public Interface
*/
QManagedWebSocketObject::QManagedWebSocket(QObject *aParent)
    : QObject(*new QManagedWebSocketPrivate, aParent)
{
}

QManagedWebSocketObject::QManagedWebSocketObject(const QUrl &aInstanceAddress, const QString &aGatewayId, QObject *aParent)
    : QObject(*new QManagedWebSocketPrivate(aInstanceAddress, aGatewayId), aParent)
{
}

bool QManagedWebSocketObject::isValid() const
{
    Q_D(const QManagedWebSocketObject);
    return d->isValid();
}

void QManagedWebSocketObject::setGateway(const QUrl &aInstanceAddress, const QString &aGatewayId)
{
    Q_D(QManagedWebSocketObject);
    d->setGateway(aInstanceAddress,aGatewayId);
}

QUrl QManagedWebSocketObject::instanceAddress() const
{
    Q_D(const QManagedWebSocketObject);
    return d->instanceAddress();
}
void QManagedWebSocketObject::setInstanceAddress(const QUrl &aInstanceAddress)
{
    Q_D(QManagedWebSocketObject);
    d->setInstanceAddress(aInstanceAddress);
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

void QManagedWebSocketObject::setSharedInstanceFrom(const QManagedWebSocketObject *aOther) {
    Q_D(QManagedWebSocketObject);
    QManagedWebSocketObjectPrivate *otherPrv;
    otherPrv=reinterpret_cast<QManagedWebSocketObjectPrivate *>(aOther->d_ptr);
    d->setSharedInstance(otherPrv->sharedInstance());
}

QT_END_NAMESPACE
