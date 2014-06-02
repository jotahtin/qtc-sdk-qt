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

#ifndef QCLOUDSERVICES_QRESTENDPOINT_OBJECT_H
#define QCLOUDSERVICES_QRESTENDPOINT_OBJECT_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QSharedPointer>

#include <QtCloudServices/qtcloudservices_global.h>
#include <QtCloudServices/QRestConnectionObject>

QT_BEGIN_NAMESPACE

class QRestEndpointObjectPrivate;
class QTCLOUDSERVICES_EXPORT QRestEndpointObject : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QRestEndpointObject)
    Q_PROPERTY(QUrl endpointAddress READ endpointAddress WRITE setEndpointAddress NOTIFY endpointAddressChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
private:
    Q_DISABLE_COPY(QRestEndpointObject)
protected:
    QRestEndpointObject(QRestEndpointObjectPrivate &dd,QObject *aParent);
public:
    QRestEndpointObject(QObject *aParent=0);
    QRestEndpointObject(const QUrl &aEndpointAddress, QObject *aParent = 0);

    // IsValid
    Q_INVOKABLE bool isValid() const Q_REQUIRED_RESULT;

    Q_INVOKABLE QUrl endpointAddress() const Q_REQUIRED_RESULT;
    Q_INVOKABLE void setEndpointAddress(const QUrl &aEndpointAddress);

    // Get Plain connection
    Q_INVOKABLE virtual QRestConnectionObject *reserveConnection() Q_REQUIRED_RESULT;
    Q_INVOKABLE virtual void releaseConnection(const QRestConnectionObject *aConnection);
public:
    void setSharedInstanceFrom(const QRestEndpointObject *aOther);
Q_SIGNALS:
    void endpointAddressChanged(const QUrl &aEndpointAddress);
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTENDPOINT_OBJECT_H */
