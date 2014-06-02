/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
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

#ifndef QCLOUDSERVICES_QENGINIOCONNECTION_OBJECT_H
#define QCLOUDSERVICES_QENGINIOCONNECTION_OBJECT_H

#include <QtCloudServices/qrestconnectionobject.h>
#include <QtCloudServices/qenginiorequestobject.h>

QT_BEGIN_NAMESPACE

class QEnginioDataStorageObject;
class QEnginioOperationObject;
class QEnginioConnectionObjectPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioConnectionObject : public QRestConnectionObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QEnginioConnectionObject)

    // Q_PROPERTY(QByteArray backendId READ backendId WRITE setBackendId NOTIFY backendIdChanged FINAL)
    // Q_PROPERTY(QUrl serviceUrl READ serviceUrl WRITE setServiceUrl NOTIFY serviceUrlChanged FINAL)

    // Q_PROPERTY(EnginioIdentity *identity READ identity WRITE setIdentity NOTIFY identityChanged FINAL)
    // Q_PROPERTY(QtCloudServices::AuthenticationState authenticationState READ authenticationState NOTIFY authenticationStateChanged FINAL)

    // Q_ENUMS(QtCloudServices::Operation); // TODO remove me QTBUG-33577
    // Q_ENUMS(QtCloudServices::AuthenticationState); // TODO remove me QTBUG-33577
private:
    Q_DISABLE_COPY(QEnginioConnectionObject)
protected:
    QEnginioConnectionObject(QEnginioConnectionObjectPrivate &dd,QObject *aParent);
public:
    QEnginioConnectionObject(const QEnginioDataStorageObject *aEnginioDataStorageObject = 0,
                             QObject *aParent = 0);

    Q_INVOKABLE QEnginioOperationObject *customRequest(const QEnginioRequestObject *aRequest);

    // EnginioIdentity *identity() const Q_REQUIRED_RESULT;
    // void setIdentity(EnginioIdentity *identity);
    // QtCloudServices::AuthenticationState authenticationState() const Q_REQUIRED_RESULT;
    // bool finishDelayedReplies();
    // From QEnginioConnection.h
    //Q_INVOKABLE QEnginioOperation fullTextSearch(const QJsonObject &query);
    //Q_INVOKABLE QEnginioOperation query(const QJsonObject &query, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    //Q_INVOKABLE QEnginioOperation create(const QJsonObject &object, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    //Q_INVOKABLE QEnginioOperation update(const QJsonObject &object, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    //Q_INVOKABLE QEnginioOperation remove(const QJsonObject &object, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    //Q_INVOKABLE QEnginioOperation uploadFile(const QJsonObject &associatedObject, const QUrl &file);
    //Q_INVOKABLE QEnginioOperation downloadUrl(const QJsonObject &object);
Q_SIGNALS:
//    void backendIdChanged(const QByteArray &backendId);
//    void serviceUrlChanged(const QUrl& url);
//    void authenticationStateChanged(QtCloudServices::AuthenticationState state);
//    void identityChanged(EnginioIdentity *identity);
//    void sessionAuthenticated(const QEnginioOperation &aReply) const;
//    void sessionAuthenticationError(const QEnginioOperation &aReply) const;
//    void sessionTerminated() const;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOCONNECTION_OBJECT_H */
