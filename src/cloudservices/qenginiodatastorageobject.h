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

#ifndef QCLOUDSERVICES_QENGINIODATASTORAGE_OBJECT_H
#define QCLOUDSERVICES_QENGINIODATASTORAGE_OBJECT_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QSharedPointer>

#include <QtCloudServices/qrestendpointobject.h>
#include <QtCloudServices/qenginiocollectionobject.h>
#include <QtCloudServices/qenginioconnectionobject.h>
#include <QtCloudServices/qenginiooperation.h>

QT_BEGIN_NAMESPACE

class QEnginioDataStorageObjectPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioDataStorageObject : public QRestEndpointObject {
    Q_OBJECT
public:
    Q_DECLARE_PRIVATE(QEnginioDataStorageObject)
    Q_PROPERTY(QString backendId READ backendId WRITE setBackendId NOTIFY backendIdChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
private:
    Q_DISABLE_COPY(QEnginioDataStorageObject)
public:
    QEnginioDataStorageObject(QObject *aParent=0);
    QEnginioDataStorageObject(const QUrl &aInstanceAddress, const QString &aBackendId, QObject *aParent = 0);

    // Backend Address & Identification
    void setBackend(const QUrl &aInstanceAddress, const QString &aBackendId);

    Q_INVOKABLE QString backendId() const Q_REQUIRED_RESULT;
    Q_INVOKABLE void setBackendId(const QString &aBackendId);

    // Authentication
    Q_INVOKABLE QString username() const Q_REQUIRED_RESULT;
    Q_INVOKABLE QString password() const Q_REQUIRED_RESULT;

    // Get object collection
    Q_INVOKABLE QEnginioCollectionObject *collection(const QString &aCollectionName);
public Q_SLOTS:
    void setUsername(const QString &aUsername);
    void setPassword(const QString &aPassword);
Q_SIGNALS:
    void backendIdChanged(QString backendId);
    void instanceAddressChanged(QUrl instanceAddress);
    void backendChanged();

    void usernameChanged(QString aUsername);
    void passwordChanged(QString aPassword);

    void operationError(QEnginioOperation aOperation);
};

// Q_DECLARE_TYPEINFO(QEnginioDataStorageObject, Q_COMPLEX_TYPE);

QT_END_NAMESPACE

// Q_DECLARE_METATYPE(QEnginioDataStorageObject)

#endif /* QCLOUDSERVICES_QENGINIODATASTORAGE_OBJECT_H */
