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

#ifndef QCLOUDSERVICES_QENGINIODATASTORAGE_H
#define QCLOUDSERVICES_QENGINIODATASTORAGE_H

#include <QtCloudServices/QRestEndpoint>
#include <QtCloudServices/QEnginioConnection>
#include <QtCloudServices/QEnginioCollection>
#include <QtCloudServices/QEnginioDataStorageObject>

QT_BEGIN_NAMESPACE

class QEnginioDataStorageObject;
class QTCLOUDSERVICES_EXPORT QEnginioDataStorage : public QRestEndpoint {
protected:
    QEnginioDataStorage(QEnginioDataStorageObject *aObject);
public:
    // Constructors
    QEnginioDataStorage();
    QEnginioDataStorage(const QUrl &aInstanceAddress, const QString &aBackendId);
    QEnginioDataStorage(const QString &aInstanceAddress, const QString &aBackendId);
    QEnginioDataStorage(const QEnginioDataStorage &aOther);
    virtual ~QEnginioDataStorage();

    // Assignment
    QEnginioDataStorage& operator=(const QEnginioDataStorage &aOther);

    // Backend Address & Identification
    void setBackend(const QUrl &aInstanceAddress, const QString &aBackendId);

    QString backendId() const Q_REQUIRED_RESULT;
    void setBackendId(const QString &aBackendId);

    // Authentication
    QString username() const Q_REQUIRED_RESULT;
    void setUsername(const QString &aUsername);

    QString password() const Q_REQUIRED_RESULT;
    void setPassword(const QString &aPassword);

    // Get object collection
    QEnginioCollection collection(const QString &aCollectionName) Q_REQUIRED_RESULT;

    // Get Plain connection
    QEnginioConnection reserveConnection() Q_REQUIRED_RESULT;
    void releaseConnection(const QEnginioConnection &aConnection);
public:
    // Get implementation object
    const QEnginioDataStorageObject* object() const;
    QEnginioDataStorageObject* object();
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIODATASTORAGE_H */
