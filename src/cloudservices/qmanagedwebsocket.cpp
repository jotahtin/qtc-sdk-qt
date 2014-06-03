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

#include "QtCloudServices/qmanagedwebsocket.h"
#include "QtCloudServices/private/qmanagedwebsocketobject_p.h"

QT_BEGIN_NAMESPACE

QManagedWebSocket::QManagedWebSocket(QManagedWebSocketObject *aObject)
    : QRestEndpoint(aObject)
{
}

QManagedWebSocket::QManagedWebSocket()
    : QRestEndpoint(new QManagedWebSocketObject)
{

}
QManagedWebSocket::QManagedWebSocket(const QUrl &aInstanceAddress, const QString &aGatewayId)
    : QRestEndpoint(new QManagedWebSocketObject(aInstanceAddress,aGatewayId))
{

}

QManagedWebSocket::QManagedWebSocket(const QString &aInstanceAddress, const QString &aGatewayId)
: QRestEndpoint(new QManagedWebSocketObject(QUrl(aInstanceAddress),aGatewayId))
{

}

QManagedWebSocket::QManagedWebSocket(const QManagedWebSocket &aOther)
    : QRestEndpoint(new QManagedWebSocketObject)
{
    object()->setSharedInstanceFrom(aOther.object());
}

QManagedWebSocket::~QManagedWebSocket()
{
}

QManagedWebSocket& QManagedWebSocket::operator=(const QManagedWebSocket &aOther) {
    object()->setSharedInstanceFrom(aOther.object());
    return *this;
}

// Backend Address & Identification
void QManagedWebSocket::setGateway(const QUrl &aInstanceAddress, const QString &aGatewayId) {
    QManagedWebSocketObject *obj;
    obj=reinterpret_cast<QManagedWebSocketObject *>(object());
    obj->setGateway(aInstanceAddress,aGatewayId);
}

QString QManagedWebSocket::gatewayId() const {
    const QManagedWebSocketObject *obj;
    obj=reinterpret_cast<const QManagedWebSocketObject *>(object());
    return obj->gatewayId();
}
void QManagedWebSocket::setGatewayId(const QString &aGatewayId) {
    QManagedWebSocketObject *obj;
    obj=reinterpret_cast<QManagedWebSocketObject *>(object());
    obj->setGatewayId(aGatewayId);
}

// Authentication
QString QManagedWebSocket::secret() const {
    const QManagedWebSocketObject *obj;
    obj=reinterpret_cast<const QManagedWebSocketObject *>(object());
    return obj->secret();
}
void QManagedWebSocket::setSecret(const QString &aSecret) {
    QManagedWebSocketObject *obj;
    obj=reinterpret_cast<QManagedWebSocketObject *>(object());
    obj->setSecret(aSecret);
}

// Operations
void QManagedWebSocket::connectSocket() {
    return object()->connectSocket();
}

void QManagedWebSocket::disconnectSocket() {
    object()->disconnectSocket();
}

const QManagedWebSocketObject* QManagedWebSocket::object() const {
    return reinterpret_cast<const QManagedWebSocketObject*>(QRestEndpoint::object());
}
QManagedWebSocketObject* QManagedWebSocket::object() {
    return reinterpret_cast<QManagedWebSocketObject*>(QRestEndpoint::object());
}

QT_END_NAMESPACE
