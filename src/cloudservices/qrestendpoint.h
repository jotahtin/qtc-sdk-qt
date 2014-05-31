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

#ifndef QCLOUDSERVICES_QRESTENDPOINT_H
#define QCLOUDSERVICES_QRESTENDPOINT_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QSharedPointer>

#include <QtCloudServices/QRestConnection>
#include <QtCloudServices/QEnginioDataStorageObject>

QT_BEGIN_NAMESPACE

class QRestEndpointObject;
class QTCLOUDSERVICES_EXPORT QRestEndpoint {
protected:
    QRestEndPoint(QRestEndpointObject *aObject);
public:
    // Constructors
    QRestEndpointObject();
    QRestEndpointObject(const QUrl &aEndpointAddress);
    QRestEndpointObject(const QString &aEndpointAddress);
    QRestEndpointObject(const QEnginioDataStorage &aOther);
    ~QRestEndpointObject();

    // Assignment
    QRestEndpointObject& operator=(const QRestEndpointObject &aOther);

    // Get implementation object
    const QRestEndpointObject* restEndpointObject() const;

    // IsValid
    virtual bool operator!() const Q_REQUIRED_RESULT;
    virtual bool isValid() const Q_REQUIRED_RESULT;

    // Backend Address & Identification
    void setBackend(const QUrl &aInstanceAddress, const QString &aBackendId);

    QUrl enpointAddress() const Q_REQUIRED_RESULT;
    void setEndpointAddress(const QUrl &aEndpointAddress);
    void setEndpointAddress(const QString &aEndpointAddress);

    // Get Plain connection
    QRestConnection reserveConnection() Q_REQUIRED_RESULT;
    void releaseConnection(const QRestConnection &aConnection);
private:
    QEnginioDataStorageObject *iEnginioDataStorageObject;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTENDPOINT_H */
