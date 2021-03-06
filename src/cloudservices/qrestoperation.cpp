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

#include "stdafx.h"

#include <QtCloudServices/qrestoperation.h>
#include <QtCloudServices/qrestconnection.h>

#include <QtCloudServices/private/qrestoperationobject_p.h>

QT_BEGIN_NAMESPACE

/*!
  \class QRestOperation
  \since 5.3
  \brief The QEnginioOperation class contains the data from a request to the Enginio database.
  \inmodule enginio-qt
  \ingroup enginio-client

  The reply, when finished, contains information received from the server:
  \list
  \li Data - object, which is a result from an earlier request,
    see the \l {QEnginioOperation::data}{data} function
  \li Network status - in case of a network problem, additional information can
  be accessed through: errorType, errorString, networkError
  \li Backend status - a finished request is always associated with a backend status
  code, which is just an HTTP code, and it can be queried through backendStatus
  \endlist

  The finished signal is emitted when the query is done.

  \sa QEnginioConnection
*/

/*!
  \enum QtCloudServices::ErrorType
  Describes the type of error that occured when making a request to the Enginio backend.
  \value NoError The reply returned without errors
  \value NetworkError The error was a networking problem
  \value BackendError The backend did not accept the query
*/

/*!
  \fn QEnginioOperation::finished(QEnginioOperation *reply)
  This signal is emitted when the QEnginioOperation \a reply is finished.
  After the network operation, use the \l{QEnginioOperation::isError()}{isError()} function to check for
  potential problems and then use the \l data property to access the returned data.
*/

/*!
  \fn QEnginioOperation::progress(qint64 bytesSent, qint64 bytesTotal)
  This signal is emitted for file operations, indicating the progress of up or downloads.
  The \a bytesSent is the current progress relative to the total \a bytesTotal.
*/



QRestOperation::QRestOperation(QRestOperationObject *aObject)
    : iObject(aObject)
{
    Q_ASSERT(iObject);
}
QRestOperation::QRestOperation()
    : iObject(new QRestOperationObject())
{
}

QRestOperation::QRestOperation(const QRestOperation &aOther)
    : iObject(new QRestOperationObject())
{
    object()->setSharedInstanceFrom(aOther.object());
}

QRestOperation::~QRestOperation() {
    if (iObject) {
        delete iObject;
    }
}

// Assignment
QRestOperation& QRestOperation::operator=(const QRestOperation &aOther) {
    object()->setSharedInstanceFrom(aOther.object());
    return *this;
}

// connection & request
QRestConnection QRestOperation::connection() const {
    return QRestConnection(iObject->connection());
}
QRestRequest QRestOperation::request() const {
   return QRestRequest(iObject->request());
}

// IsValid
bool QRestOperation::operator!() const {
    return !isValid();
}
bool QRestOperation::isValid() const {
    return iObject->isValid();
}

// Status
bool QRestOperation::isError() const {
    return iObject->isError();
}
bool QRestOperation::isFinished() const {
    return iObject->isFinished();
}

int QRestOperation::backendStatus() const {
    return iObject->backendStatus();
}
QString QRestOperation::requestId() const {
    return iObject->requestId();
}

QtCloudServices::ErrorType QRestOperation::errorType() const {
    return iObject->errorType();
}
QNetworkReply::NetworkError QRestOperation::errorCode() const {
    return iObject->errorCode();
}
QString QRestOperation::errorString() const {
    return iObject->errorString();
}

QJsonObject QRestOperation::resultJson() const {
    return iObject->resultJson();
}
QByteArray QRestOperation::resultBytes() const {
    return iObject->resultBytes();
}


#ifndef QT_NO_DEBUG_STREAM
void QRestOperation::dumpDebugInfo(QDebug &d) const {
    iObject->dumpDebugInfo(d);
}
#endif /* QT_NO_DEBUG_STREAM */

const QRestOperationObject* QRestOperation::object() const {
    return iObject;
}
QRestOperationObject* QRestOperation::object() {
    return iObject;
}

#ifndef QT_NO_DEBUG_STREAM
QTCLOUDSERVICES_EXPORT
QDebug operator<<(QDebug d, const QRestOperation &aOperation) {
    aOperation.dumpDebugInfo(d);
    return d;
}
#endif /* QT_NO_DEBUG_STREAM */

QT_END_NAMESPACE
