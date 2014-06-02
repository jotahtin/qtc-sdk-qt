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

#ifndef QCLOUDSERVICES_QRESTENDPOINT_SHARED_P_H
#define QCLOUDSERVICES_QRESTENDPOINT_SHARED_P_H

#include <QUrl>
#include <QMutex>
#include <QVector>

#include <QtCloudServices/private/qrestconnectionshared_p.h>
#include <QtCloudServices/private/qrestoperationshared_p.h>

QT_BEGIN_NAMESPACE

class QRestEndpointShared : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(QRestEndpointShared)
public:
    QRestEndpointShared();
    QRestEndpointShared(const QUrl &aEndpointAddress);
    ~QRestEndpointShared();
public:
    virtual bool isValid() const;

    QUrl endpointAddress() const Q_REQUIRED_RESULT;

    QSharedPointer<QRestConnectionShared> reserveConnection(QSharedPointer<QRestEndpointShared> aSelf) Q_REQUIRED_RESULT;
    void releaseConnection(QSharedPointer<QRestConnectionShared> aConnection);
protected:
    virtual QSharedPointer<QRestConnectionShared>
    buildConnectionInstance(QSharedPointer<QRestEndpointShared> aSelf);

Q_SIGNALS:
    void operationError(QSharedPointer<QRestOperationShared> aOperation);
private:
    QUrl iEndpointAddress;

    // Mutable objects
    QMutex iLock;

    QVector< QRestConnectionShared > iConnectionPool;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTENDPOINT_SHARED_P_H */
