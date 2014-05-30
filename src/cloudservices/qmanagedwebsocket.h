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

#ifndef QCLOUDSERVICES_QMANAGEDWEBSOCKET_H
#define QCLOUDSERVICES_QMANAGEDWEBSOCKET_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QSharedPointer>

#include <QtCloudServices/qcloudservicesobject.h>
#include <QtCloudServices/QEnginioConnection>
#include <QtCloudServices/QEnginioCollection>

QT_BEGIN_NAMESPACE

class QManagedWebSocketPrivate;
class QTCLOUDSERVICES_EXPORT QManagedWebSocket : public QCloudServicesObject {
    Q_OBJECT
    QTC_DECLARE_PRIVATE(QManagedWebSocket)
public:
    // Default Constructor
    QManagedWebSocket(QObject *aParent = 0);
    QManagedWebSocket(const QUrl &aInstanceAddress, const QString &aGatewayId, QObject *parent = 0);
    QManagedWebSocket(const QString &aInstanceAddress, const QString &aGatewayId, QObject *parent = 0);
    QManagedWebSocket(const QManagedWebSocket &aManagedWebSocket);
    ~QManagedWebSocket();

    // Assignment
    QManagedWebSocket& operator=(const QManagedWebSocket &aManagedWebSocket);

    // IsValid
    bool operator!() const;
    virtual bool isValid() const;

    // Backend Address & Identification
    void setGateway(const QUrl &aInstanceAddress, const QString &aGatewayId);

    QUrl instanceAddress() const Q_REQUIRED_RESULT;
    void setInstanceAddress(const QUrl &aInstanceAddress);
    void setInstanceAddressString(const QString &aInstanceAddress);

    QString gatewayId() const Q_REQUIRED_RESULT;
    void setGatewayId(const QString &aGatewayId);
public Q_SLOTS:
    void connect();
    void disconnect();
Q_SIGNALS:
    void gatewayChanged();
    void gatewayIdChanged(const QString &backendId);
    void instanceAddressChanged(const QUrl &instanceAddress);
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QMANAGEDWEBSOCKET_H */
