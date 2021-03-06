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

#ifndef QCLOUDSERVICES_QRESTOPERATION_H
#define QCLOUDSERVICES_QRESTOPERATION_H

#include <QObject>
#include <QDebug>

#include <QtCore/qjsonobject.h>

#include <QtNetwork/qnetworkreply.h>

#include <QtCloudServices/qtcloudservices_global.h>
#include <QtCloudServices/qrestrequest.h>

QT_BEGIN_NAMESPACE

class QRestConnection;
class QRestOperationObject;
class QTCLOUDSERVICES_EXPORT QRestOperation {
    friend class QRestConnection;
    friend class QRestRequest;
public:
    typedef std::function<void(QRestOperation aRestOperation)> Callback;
protected:
    QRestOperation(QRestOperationObject *aObject);
public:
    // Constructors
    QRestOperation();
    QRestOperation(const QRestOperation &aOther);
    virtual ~QRestOperation();

    // Assignment
    QRestOperation& operator=(const QRestOperation &aOther);

    // connection & request
    QRestConnection connection() const Q_REQUIRED_RESULT;
    QRestRequest request() const Q_REQUIRED_RESULT;

    // IsValid
    bool operator!() const Q_REQUIRED_RESULT;
    bool isValid() const Q_REQUIRED_RESULT;

    // Status
    bool isError() const Q_REQUIRED_RESULT;
    bool isFinished() const Q_REQUIRED_RESULT;

    int backendStatus() const Q_REQUIRED_RESULT;
    QString requestId() const Q_REQUIRED_RESULT;

    QtCloudServices::ErrorType errorType() const Q_REQUIRED_RESULT;
    QNetworkReply::NetworkError errorCode() const Q_REQUIRED_RESULT;
    QString errorString() const Q_REQUIRED_RESULT;

    QJsonObject resultJson() const Q_REQUIRED_RESULT;
    QByteArray resultBytes() const Q_REQUIRED_RESULT;

#ifndef QT_NO_DEBUG_STREAM
    void dumpDebugInfo(QDebug &d) const;
#endif
public:
    // Get implementation object
    const QRestOperationObject* object() const;
    QRestOperationObject* object();
private:
    QRestOperationObject *iObject;
};

#ifndef QT_NO_DEBUG_STREAM
QTCLOUDSERVICES_EXPORT
QDebug operator<<(QDebug d, const QRestOperation &aOperation);
#endif

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTOPERATION_H */
