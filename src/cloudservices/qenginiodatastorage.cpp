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

#include "stdafx.h"

#include "QtCloudServices/qenginiodatastorage.h"
#include "QtCloudServices/private/qenginiodatastorageobject_p.h"

QT_BEGIN_NAMESPACE

/*!
\class QEnginioDataStorage
\since 5.3
\inmodule QtCloudServices
\ingroup EnginioDataStorage
\target QEnginioDataStorage
\brief QEnginioDataStorage handles all communication with the Enginio server

Plah...

The Enginio server supports several separate "backends" with each account.
By setting the \l{QEnginioConnection::backendId}{backendId} a backend is chosen.
After setting the ID interaction with the server is possible.
The information about the backend is available on the Enginio Dashboard
after logging in to \l {http://engin.io}{Enginio}.
\code
QEnginioConnection *client = new QEnginioConnection(parent);
client->setBackendId(QByteArrayLiteral("YOUR_BACKEND_ID"));
\endcode

The basic functions used to interact with the backend are
\l create(), \l query(), \l remove() and \l update().
It is possible to do a fulltext search on the server using \l fullTextSearch().
For file handling \l downloadUrl() and \l uploadFile() are provided.
The functions are asynchronous, which means that they are not blocking
and the result of them will be delivered together with QEnginioOperation::finished()
signal.

\note After the request has finished, it is the responsibility of the
user to delete the QEnginioOperation object at an appropriate time.
Do not directly delete it inside the slot connected to finished().
You can use the \l{QObject::deleteLater()}{deleteLater()} function.

In order to make queries that return an array of data more convenient
a model is provided by \l {EnginioModelCpp}{EnginioModel}.
*/

QEnginioDataStorage::QEnginioDataStorage(QEnginioDataStorageObject *aObject)
    : QRestEndpoint(aObject)
{
}

QEnginioDataStorage::QEnginioDataStorage()
    : QRestEndpoint(new QEnginioDataStorageObject)
{

}

QEnginioDataStorage::QEnginioDataStorage(const QUrl &aInstanceAddress, const QString &aBackendId)
    : QRestEndpoint(new QEnginioDataStorageObject(aInstanceAddress,aBackendId))
{
}

QEnginioDataStorage::QEnginioDataStorage(const QString &aInstanceAddress, const QString &aBackendId)
    : QRestEndpoint(new QEnginioDataStorageObject(QUrl(aInstanceAddress),aBackendId))
{
}

QEnginioDataStorage::QEnginioDataStorage(const QEnginioDataStorage &aOther)
    : QRestEndpoint(new QEnginioDataStorageObject)
{
    object()->setSharedInstanceFrom(aOther.object());
}

QEnginioDataStorage& QEnginioDataStorage::operator=(const QEnginioDataStorage &aOther) {
    object()->setSharedInstanceFrom(aOther.object());
    return *this;
}

/*!
\brief Set Enginio Data Storage environment variables.

Internally QEnginioDataStorage hold reference into private shared implementation,
that contains these connection variables and resource pools for connections, collections
and other resources.

When new environment is initialized, this QEnginioDataStorage instance will release its
shared reference to actual implementation object and construct new instace.

Setting these values do not affect any started operations. New values are used only when new
resources (for example QEnginioCollection's) are requested from QEnginioDataStorage.

\code
    QEnginioDataStorage eds;
    eds.setBackend(QUrl("https://api.engino.io","12345....");
\endcode
*/
void QEnginioDataStorage::setBackend(const QUrl &aInstanceAddress, const QString &aBackendId) {
    QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<QEnginioDataStorageObject *>(object());
    obj->setBackend(aInstanceAddress,aBackendId);
}

QString QEnginioDataStorage::backendId() const {
    const QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<const QEnginioDataStorageObject *>(object());
    return obj->backendId();
}
void QEnginioDataStorage::setBackendId(const QString &aBackendId) {
    QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<QEnginioDataStorageObject *>(object());
    obj->setBackendId(aBackendId);
}

QString QEnginioDataStorage::username() const {
    const QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<const QEnginioDataStorageObject *>(object());
    return obj->username();
}

void QEnginioDataStorage::setUsername(const QString &aUsername) {
    QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<QEnginioDataStorageObject *>(object());
    obj->setUsername(aUsername);
}

QString QEnginioDataStorage::password() const {
    const QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<const QEnginioDataStorageObject *>(object());
    return obj->password();
}
void QEnginioDataStorage::setPassword(const QString &aPassword) {
    QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<QEnginioDataStorageObject *>(object());
    obj->setPassword(aPassword);
}

QEnginioCollection QEnginioDataStorage::collection(const QString &aCollectionName) {
    QEnginioDataStorageObject *obj;
    obj=reinterpret_cast<QEnginioDataStorageObject *>(object());
    return QEnginioCollection(obj->collection(aCollectionName));
}

QEnginioConnection QEnginioDataStorage::reserveConnection() {
    QRestConnectionObject *obj;
    QEnginioConnectionObject *engObj;

    obj = object()->reserveConnection();
    if (obj == NULL) {
        return QEnginioConnection();
    }

    engObj = reinterpret_cast<QEnginioConnectionObject *>(obj);

    return QEnginioConnection(engObj);
}

void QEnginioDataStorage::releaseConnection(const QEnginioConnection &aConnection) {
    object()->releaseConnection(aConnection.object());
}

QT_END_NAMESPACE
