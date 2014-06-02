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

#include <QtCloudServices/qenginiooperation.h>
#include <QtCloudServices/qenginioconnection.h>

#include <QtCloudServices/private/qenginiooperationobject_p.h>

QT_BEGIN_NAMESPACE

/*!
  \class QEnginioOperation
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

QEnginioOperation::QEnginioOperation(QEnginioOperationObject *aEnginioOperationObject)
    : QRestOperation(aEnginioOperationObject)
{

}

QEnginioOperation::QEnginioOperation()
   : QRestOperation(new QEnginioOperationObject)
{

}
QEnginioOperation::QEnginioOperation(const QEnginioOperation &aOther)
    : QRestOperation(new QEnginioOperationObject)
{
    object()->setSharedInstanceFrom(aOther.object());
}

// Assignment
QEnginioOperation& QEnginioOperation::operator=(const QEnginioOperation &aOther) {
    object()->setSharedInstanceFrom(aOther.object());
    return *this;
}

// connection & request
QEnginioConnection QEnginioOperation::connection() const {
    return QEnginioConnection(reinterpret_cast<QEnginioConnectionObject *>(object()->connection()));
}
QEnginioRequest QEnginioOperation::request() const {
    return QEnginioRequest(reinterpret_cast<QEnginioRequestObject *>(object()->request()));
}

int QEnginioOperation::resultObjectCount() const {
    const QEnginioOperationObject *obj=reinterpret_cast<const QEnginioOperationObject *>(object());
    return obj->resultObjectCount();
}
QEnginioObject QEnginioOperation::resultObject() const {
    const QEnginioOperationObject *obj=reinterpret_cast<const QEnginioOperationObject *>(object());
    return obj->resultObject();
}
QList<QEnginioObject> QEnginioOperation::resultObjects() const {
    const QEnginioOperationObject *obj=reinterpret_cast<const QEnginioOperationObject *>(object());
    return obj->resultObjects();
}

QT_END_NAMESPACE
