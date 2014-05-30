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

#include "QtCloudServices/private/qmanagedwebsocket_p.h"

/*!
\class QManagedWebSocket
\since 5.3
\inmodule QtCloudServices
\ingroup QManagedWebSocket
\target QManagedWebSocket
\brief QManagedWebSocket handles all communication with the ManagedWebSockets

Plah...

The Enginio server supports several separate "backends" with each account.
By setting the \l{QEnginioConnection::backendId}{backendId} a backend is chosen.
After setting the ID interaction with the server is possible.
The information about the backend is available on the Enginio Dashboard
after logging in to \l {http://engin.io}{Enginio}.
\code
QEnginioConnection *client = new QEnginioConnection(parent);
client->setBackendId(QByteArrayLiteral("YOUR_BACKEND_ID"));
\endcode

The basic functions used to interact with the backend are
\l create(), \l query(), \l remove() and \l update().
It is possible to do a fulltext search on the server using \l fullTextSearch().
For file handling \l downloadUrl() and \l uploadFile() are provided.
The functions are asynchronous, which means that they are not blocking
and the result of them will be delivered together with QEnginioOperation::finished()
signal.

\note After the request has finished, it is the responsibility of the
user to delete the QEnginioOperation object at an appropriate time.
Do not directly delete it inside the slot connected to finished().
You can use the \l{QObject::deleteLater()}{deleteLater()} function.

In order to make queries that return an array of data more convenient
a model is provided by \l {EnginioModelCpp}{EnginioModel}.
*/


QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/
QManagedWebSocketPrivate::QManagedWebSocketPrivate()
{

}
QManagedWebSocketPrivate::QManagedWebSocketPrivate(const QUrl &aInstanceAddress, const QString &aGatewayId,
        QManagedWebSocketPrivate *aPrevInstance)
    : QCloudServicesObjectPrivate(),
      iInstanceAddress(aInstanceAddress), iGatewayId(aGatewayId)
{
    Q_UNUSED(aPrevInstance);
}


QManagedWebSocketPrivate::~QManagedWebSocketPrivate()
{
}

QUrl QManagedWebSocketPrivate::instanceAddress() const
{
    return iInstanceAddress;
}

QString QManagedWebSocketPrivate::gatewayId() const
{
    return iGatewayId;
}

// https://mws-eu-1.qtc.io/v1/gateways/:mws_gateway_id

void QManagedWebSocketPrivate::connect()
{
    QUrl url;

    // iWebSocket.open(url);
}
void QManagedWebSocketPrivate::disconnect()
{

}

/*
** Public Interface
*/
QManagedWebSocket::QManagedWebSocket(QObject *aParent)
    : QCloudServicesObject(QManagedWebSocket::dvar(new QManagedWebSocketPrivate), aParent)
{
}

QManagedWebSocket::QManagedWebSocket(const QUrl &aInstanceAddress, const QString &aGatewayId, QObject *aParent)
    : QCloudServicesObject(QManagedWebSocket::dvar(new QManagedWebSocketPrivate(aInstanceAddress, aGatewayId)), aParent)
{
}

QManagedWebSocket::QManagedWebSocket(const QString &aInstanceAddress, const QString &aGatewayId, QObject *aParent)
    : QCloudServicesObject(QManagedWebSocket::dvar(new QManagedWebSocketPrivate(QUrl(aInstanceAddress), aGatewayId)), aParent)
{
}

QManagedWebSocket::QManagedWebSocket(const QManagedWebSocket &aManagedWebSocket)
    : QCloudServicesObject(aManagedWebSocket.d<QManagedWebSocket>())
{

}

QManagedWebSocket::~QManagedWebSocket()
{

}

QManagedWebSocket& QManagedWebSocket::operator=(const QManagedWebSocket &aManagedWebSocket)
{
    setPIMPL(aManagedWebSocket.d<QManagedWebSocket>());
    return *this;
}

bool QManagedWebSocket::operator!() const
{
    return !isValid();
}
bool QManagedWebSocket::isValid() const
{
    if (isNull()) {
        return false;
    }

    if (instanceAddress().isEmpty() || gatewayId().isEmpty()) {
        return false;
    }

    return true;
}

void QManagedWebSocket::setGateway(const QUrl &aInstanceAddress, const QString &aGatewayId)
{
    bool chgAddress, chgId;
    QManagedWebSocket::dvar impl;

    impl = d<QManagedWebSocket>();

    chgAddress = (impl->instanceAddress() != aInstanceAddress);
    chgId = (impl->gatewayId() != aGatewayId);

    if (!chgAddress && !chgId) {
        return;
    }

    impl = QManagedWebSocket::dvar(new QManagedWebSocketPrivate(aInstanceAddress, aGatewayId));
    setPIMPL(impl);

    if (chgAddress) {
        emit instanceAddressChanged(aInstanceAddress);
    }

    if (chgId) {
        emit gatewayIdChanged(aGatewayId);
    }

    emit gatewayChanged();
}

QUrl QManagedWebSocket::instanceAddress() const
{
    return d<const QManagedWebSocket>()->instanceAddress();
}
void QManagedWebSocket::setInstanceAddress(const QUrl &aInstanceAddress)
{
    setGateway(aInstanceAddress, gatewayId());
}
void QManagedWebSocket::setInstanceAddressString(const QString &aInstanceAddress)
{
    setInstanceAddress(QUrl(aInstanceAddress));
}

QString QManagedWebSocket::gatewayId() const
{
    return d<const QManagedWebSocket>()->gatewayId();
}
void QManagedWebSocket::setGatewayId(const QString &aBackendId)
{
    setGateway(instanceAddress(), aBackendId);
}

void QManagedWebSocket::connect()
{
    d<const QManagedWebSocket>()->connect();

}
void QManagedWebSocket::disconnect()
{
    d<const QManagedWebSocket>()->disconnect();

}

QT_END_NAMESPACE
