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

#ifndef QCLOUDSERVICES_QENGINIODATASTORAGE_OBJECT_P_H
#define QCLOUDSERVICES_QENGINIODATASTORAGE_OBJECT_P_H

#include <QMap>
#include <QUrl>

#include <QtCloudServices/qenginiodatastorageobject.h>
#include <QtCloudServices/qenginiocollectionobject.h>
#include <QtCloudServices/private/qrestendpointobject_p.h>

QT_BEGIN_NAMESPACE

/*
** QEnginioDataStorageObjectPrivate
*/
class QEnginioDataStorageShared;
class QEnginioDataStorageObjectPrivate : public QRestEndpointObjectPrivate {
    Q_DECLARE_PUBLIC(QEnginioDataStorageObject)
private:
    Q_DISABLE_COPY(QEnginioDataStorageObjectPrivate)
public:
    QEnginioDataStorageObjectPrivate();

    // Backend Address & Identification
    void setBackend(const QUrl &aInstanceAddress, const QString &aBackendId);

    void setBackendId(const QString &aBackendId);
    virtual void setEndpointAddress(const QUrl &aEndpointAddress);

    QString backendId() const Q_REQUIRED_RESULT;

    // Authentication
    QString secret() const Q_REQUIRED_RESULT;
    QString username() const Q_REQUIRED_RESULT;
    QString password() const Q_REQUIRED_RESULT;

    // Get object collection
    QEnginioCollectionObject *collection(const QString &collectionName);

    void setSecret(const QString &aSecret);
    void setUsername(const QString &aUsername);
    void setPassword(const QString &aPassword);
protected:
    virtual QRestConnectionObject* buildConnectionObject() const;
protected:
    virtual void init();
    virtual void deinit();
private:
    QMetaObject::Connection iConnectionSecretChanged;
    QMetaObject::Connection iConnectionUsernameChanged;
    QMetaObject::Connection iConnectionPasswordChanged;
    QMetaObject::Connection iConnectionOperationError;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIODATASTORAGE_OBJECT_P_H */
