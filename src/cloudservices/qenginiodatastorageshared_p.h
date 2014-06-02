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

#ifndef QCLOUDSERVICES_QENGINIODATASTORAGE_SHARED_P_H
#define QCLOUDSERVICES_QENGINIODATASTORAGE_SHARED_P_H

#include <QMap>
#include <QUrl>

#include <QtCloudServices/private/qrestendpointshared_p.h>
#include <QtCloudServices/private/qenginiocollectionshared_p.h>
#include <QtCloudServices/private/qenginioconnectionshared_p.h>

#include <QtCloudServices/QEnginioOperation>

QT_BEGIN_NAMESPACE

/*
** QEnginioDataStorageShared
*/
class QEnginioDataStorageShared : public QRestEndpointShared {
    Q_OBJECT
    Q_DISABLE_COPY(QEnginioDataStorageShared)
public:
    QEnginioDataStorageShared();
    QEnginioDataStorageShared(const QUrl &aInstanceAddress, const QString &aBackendId,
                               QEnginioDataStorageShared *aPrevInstance = 0);
    ~QEnginioDataStorageShared();
public:
    virtual bool isValid() const;

    QUrl instanceAddress() const Q_REQUIRED_RESULT;
    QString backendId() const Q_REQUIRED_RESULT;

    QString username() const Q_REQUIRED_RESULT;
    void setUsername(const QString &aUsername);

    QString password() const Q_REQUIRED_RESULT;
    void setPassword(const QString &aPassword);

    QSharedPointer<QEnginioCollectionShared>
    collection(QSharedPointer<QEnginioDataStorageShared> aSelf,
               const QString &aCollectionName);

    QSharedPointer<QEnginioConnectionShared> reserveConnection(QSharedPointer<QEnginioDataStorageShared> aSelf) Q_REQUIRED_RESULT;
    void releaseConnection(QSharedPointer<QEnginioConnectionShared> aConnection);
protected:
    virtual QSharedPointer<QRestConnectionShared>
    buildConnectionInstance(QSharedPointer<QRestEndpointShared> aSelf);
Q_SIGNALS:
    void usernameChanged(const QString &aUsername);
    void passwordChanged(const QString &aPassword);

    void operationError(const QEnginioOperation &aOperation);
private:
    QString iBackendId;

    // Mutable objects
    QMutex iLock;

    QString iUsername;
    QString iPassword;

    QMap<QString, QSharedPointer<QEnginioCollectionShared> > iCollections;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIODATASTORAGE_SHARED_P_H */
