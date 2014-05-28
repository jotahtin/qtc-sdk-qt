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

#ifndef QCLOUDSERVICES_QENGINIODATASTORAGE_P_H
#define QCLOUDSERVICES_QENGINIODATASTORAGE_P_H

#include <QtCloudServices/qtcloudservices_global.h>

#include <QMap>
#include <QUrl>

#include <QtCloudServices/qenginiodatastorage.h>

#include <QtCloudServices/private/qcloudservicesobject_p.h>
#include <QtCloudServices/private/qenginiocollection_p.h>
#include <QtCloudServices/private/qenginioconnection_p.h>

QT_BEGIN_NAMESPACE

/*
** QEnginioDataStoragePrivate
*/
class QEnginioDataStoragePrivate : public QCloudServicesObjectPrivate {
    Q_OBJECT;
    friend class QEnginioCollectionPrivate;
public:
    QEnginioDataStoragePrivate();
    QEnginioDataStoragePrivate(const QUrl &aBackendAddress, const QString &aBackendId,
                               QEnginioDataStoragePrivate *aPrevInstance = 0);
    ~QEnginioDataStoragePrivate();
public:
    void setBackend(const QUrl &aBackendAddress, const QString &aBackendId);

    QUrl backendAddress() const Q_REQUIRED_RESULT;
    void setBackendAddress(const QUrl &aBackendAddress);

    QString backendId() const Q_REQUIRED_RESULT;
    void setBackendId(const QString &aBackendId);

    QString username() const Q_REQUIRED_RESULT;
    void setUsername(const QString &aUsername);

    QString password() const Q_REQUIRED_RESULT;
    void setPassword(const QString &aPassword);

    QEnginioConnection reserveConnection() Q_REQUIRED_RESULT;
    void releaseConnection(const QEnginioConnection &aConnection);

    QEnginioCollection collection(const QString &aCollectionName);
public:
    void bindForwarding(QEnginioDataStorage *aInstance);
    void unbindForwarding();
public:
    QTC_DECLARE_PUBLIC(QEnginioDataStorage);
Q_SIGNALS:
    void usernameChanged(const QString &aUsername);
    void passwordChanged(const QString &aPassword);
protected:
    QString iBackendId;
    QUrl iBackendAddress;

    // Mutable objects
    QMutex iLock;

    QString iUsername;
    QString iPassword;

    QVector< QEnginioConnection > iConnectionPool;
    QMap<QString, QEnginioCollection > iCollections;

    bool iForwarding;
    QMetaObject::Connection iUsernameForwarding;
    QMetaObject::Connection iPasswordForwarding;

};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIODATASTORAGE_P_H */

