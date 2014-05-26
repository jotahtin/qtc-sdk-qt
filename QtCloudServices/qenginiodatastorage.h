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

#include <QObject>
#include <QUrl>
#include <QString>
#include <QSharedPointer>

#include <QtCloudServices/qcloudservicesobject.h>
#include <QtCloudServices/QEnginioConnection>
#include <QtCloudServices/QEnginioCollection>

QT_BEGIN_NAMESPACE

/*

QtCloudServices (lib/dll)
   QEnginioDataStorage (EDS)  - QEds
   QEdsCollection
   QManagedWebSocket (MWS)
*/

class QEnginioDataStoragePrivate;
class QTCLOUDSERVICES_EXPORT QEnginioDataStorage : public QCloudServicesObject {
    Q_OBJECT
    Q_PROPERTY(QString backendId READ backendId WRITE setBackendId NOTIFY backendIdChanged)
    Q_PROPERTY(QUrl backendAddress READ backendAddress WRITE setBackendAddress NOTIFY backendAddressChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    friend class QEnginioCollection;
public:
    // Default Constructor
    QEnginioDataStorage(QObject *parent = 0);
    QEnginioDataStorage(const QUrl &backendAddress, const QString &backendId, QObject *parent = 0);
    QEnginioDataStorage(const QString &backendAddress, const QString &backendId, QObject *parent = 0);
    ~QEnginioDataStorage();

    // Backend Address & Identification
    void setBackend(const QUrl &aBackendAddress, const QString &aBackendId);

    QUrl backendAddress() const Q_REQUIRED_RESULT;
    void setBackendAddress(const QUrl &aBackendAddress);
    void setBackendAddressString(const QString &aBackendAddress);

    QString backendId() const Q_REQUIRED_RESULT;
    void setBackendId(const QString &aBackendId);

    // Authentication
    QString username() const Q_REQUIRED_RESULT;
    QString password() const Q_REQUIRED_RESULT;

public Q_SLOTS:
    void setUsername(const QString &aUsername);
    void setPassword(const QString &aPassword);

public:
    // Access Storage Resources (Collections)
    QEnginioCollection collection(const QString &collectionName);

    // Get Plain connection
    QEnginioConnection reserveConnection();
    void releaseConnection(QEnginioConnection &aConnection);
Q_SIGNALS:
    void backendIdChanged(const QString &backendId);
    void backendAddressChanged(const QUrl &backendAddress);

    void usernameChanged(const QString &aUsername);
    void passwordChanged(const QString &aPassword);

    //
    void operationError(const QEnginioOperation &aOperation);

    // What are these
Q_SIGNALS:
//	void dataChanged();
//	void aboutToDestroy();
//protected:
//	virtual void prepareSessionToken(QEnginioConnectionPrivate *enginio) Q_DECL_OVERRIDE;
//	virtual void removeSessionToken(QEnginioConnectionPrivate *enginio) Q_DECL_OVERRIDE;

protected:
    QTC_DECLARE_PRIVATE(QEnginioDataStorage)
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QEDS_H */
