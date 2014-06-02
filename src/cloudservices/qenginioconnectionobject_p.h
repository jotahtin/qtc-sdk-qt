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

#ifndef QCLOUDSERVICES_QENGINIOCONNECTION_OBJECT_P_H
#define QCLOUDSERVICES_QENGINIOCONNECTION_OBJECT_P_H

#include <QtCloudServices/qenginioconnectionobject.h>
#include <QtCloudServices/private/qrestconnectionobject_p.h>

QT_BEGIN_NAMESPACE

class QEnginioDataStorageObject;
class QEnginioConnectionPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioConnectionObjectPrivate : public QRestConnectionObjectPrivate {
    Q_DECLARE_PUBLIC(QEnginioConnectionObject)
private:
    Q_DISABLE_COPY(QEnginioConnectionObjectPrivate)
public:
    QEnginioConnectionObjectPrivate(const QEnginioDataStorageObject *aEnginioDataStorageObject=0);
    ~QEnginioConnectionObjectPrivate();

    QEnginioOperation customRequest(const QEnginioRequest &aRequest);

    // EnginioIdentity *identity() const Q_REQUIRED_RESULT;
    // void setIdentity(EnginioIdentity *identity);
    // QtCloudServices::AuthenticationState authenticationState() const Q_REQUIRED_RESULT;

    // QEnginioOperation fullTextSearch(const QJsonObject &query);
    // QEnginioOperation query(const QJsonObject &query, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    // QEnginioOperation create(const QJsonObject &object, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    // QEnginioOperation update(const QJsonObject &object, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    // QEnginioOperation remove(const QJsonObject &object, const QtCloudServices::Operation operation = QtCloudServices::ObjectOperation);
    // QEnginioOperation uploadFile(const QJsonObject &associatedObject, const QUrl &file);
    // QEnginioOperation downloadUrl(const QJsonObject &object);
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOCONNECTION_OBJECT_P_H */
