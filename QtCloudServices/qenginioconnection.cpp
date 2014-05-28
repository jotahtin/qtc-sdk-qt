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

#include <QtNetwork/qnetworkreply.h>

#include <QtCloudServices/qenginiomodel.h>

#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qenginiooperation_p.h>
#include <QtCloudServices/private/qenginiodatastorage_p.h>
#include <QtCloudServices/private/qenginiorequest_p.h>
#include <QtCloudServices/private/chunkdevice_p.h>


#if defined(ENGINIO_VALGRIND_DEBUG)
#include <QtNetwork/qsslcipher.h>
#include <QtNetwork/qsslconfiguration.h>
#endif

/*!
\module enginio-client
\title Enginio Client Interface

This interface provides access to the Enginio service through a set of C++ classes
based on Qt.
*/

/*!
\class QEnginioConnection
\since 5.3
\inmodule enginio-qt
\ingroup enginio-client
\target QEnginioConnectionCpp
\brief QEnginioConnection handles all communication with the Enginio server

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

/*!
\class QEnginioConnection
\since 5.3
\inmodule enginio-qt
\ingroup enginio-client
\brief QEnginioConnection keeps track of the authenticated connection to the server.

You should never use QEnginioConnection explicitly, instead use the derrived
QEnginioConnection.
\sa QEnginioConnection
*/

/*!
\namespace Enginio
\inmodule enginio-qt
\brief The Enginio namespace provides enums used throughout Enginio.
*/

/*!
\fn void QEnginioConnection::error(QEnginioOperation *reply)
\brief This signal is emitted when a request to the backend returns an error.

The \a reply contains the details about the error that occured.
\sa QEnginioOperation
*/

/*!
\fn void QEnginioConnection::finished(QEnginioOperation *reply)
\brief This signal is emitted when a request to the backend finishes.

The \a reply contains the data returned. This signal is emitted for both, successful requests and failed ones.

From this moment on ownership of the \a reply is moved from QEnginioConnection, therefore it is the developer's
responsibility to delete the \a reply after this signal is handled. That can be achieved
by calling the \l{QObject::deleteLater()}{deleteLater()} method of the \a reply.
\sa QEnginioOperation
*/

/*!
\property QEnginioConnection::authenticationState
\brief The state of the authentication.

Enginio provides convenient user management.
The authentication state reflects whether the current user is authenticated.
\sa QtCloudServices::AuthenticationState identity EnginioOAuth2Authentication
*/

/*!
\fn QEnginioConnection::sessionAuthenticated(QEnginioOperation *reply) const
\brief Emitted when a user logs in.

The signal is emitted after a user was successfully logged into the backend. From that
point on, all communication with the backend will be using these credentials.
The \a reply contains the information about the login and the user. The details may be different
depending on the authentication method used, but a typical reply look like this:
\code
{
"access_token": "...",              // oauth2 access token
"refresh_token": "...",             // oauth2 refresh token
"token_type": "bearer",             // oauth2 token type
"expires_in": 28799,                // oautth2 token expiry date
"enginio_data": {
"user": {
"id": "...",                    // this user Id
"createdAt": "...",             // when the user was created
"creator": {                    // who created the user
"id": "creatorId",
"objectType": "users"
},
"email": "user@user.com",       // the user's email address
"firstName": "John",            // the user's first name
"lastName": "Foo",              // the user's last name
"objectType": "users",
"updatedAt": "2013-11-25T14:54:58.957Z",
"username": "JohnFoo"           // the user's login
},
"usergroups": []                  // usergroups to which the user belongs
}
}
\endcode

\note The \a reply will be deleted automatically after this signal, so it can not be
stored.

\sa sessionAuthenticationError(), QEnginioOperation, EnginioOAuth2Authentication
*/

/*!
\fn QEnginioConnection::sessionAuthenticationError(QEnginioOperation *reply) const
\brief Emitted when a user login fails.

The \a reply contains the details about why the login failed.
\sa sessionAuthenticated(), QEnginioOperation QEnginioConnection::identity() EnginioOAuth2Authentication

\note The \a reply will be deleted automatically after this signal, so it can not be
stored.
*/

/*!
\fn QEnginioConnection::sessionTerminated() const
\brief Emitted when a user logs out.
\sa QEnginioConnection::identity() EnginioOAuth2Authentication
*/

/*!
\enum QtCloudServices::Operation

Enginio has a unified API for several \c operations.
For example when using query(), the default is \c ObjectOperation,
which means that the query will return objects from the database.
When passing in UserOperation instead, the query will return
users.

\value ObjectOperation Operate on objects
\value AccessControlOperation Operate on the ACL
\value FileOperation Operate with files
\value UserOperation Operate on users
\value UsergroupOperation Operate on groups
\value UsergroupMembersOperation Operate on group members

\omitvalue SessionOperation
\omitvalue SearchOperation
\omitvalue FileChunkUploadOperation
\omitvalue FileGetDownloadUrlOperation
*/

/*!
\enum QtCloudServices::AuthenticationState

This enum describes the state of the user authentication.
\value NotAuthenticated No attempt to authenticate was made
\value Authenticating Authentication request has been sent to the server
\value Authenticated Authentication was successful
\value AuthenticationFailure Authentication failed

\sa QEnginioConnection::authenticationState
*/

QT_BEGIN_NAMESPACE









#if 0

QTCLOUDSERVICES_EXPORT bool gEnableEnginioDebugInfo = !qEnvironmentVariableIsSet("ENGINIO_DEBUG_INFO");

QEnginioConnectionPrivate::QEnginioConnectionPrivate()
    : QCloudServicesObjectPrivate(),

      _identity(),
      _serviceUrl(QtCloudServicesConstants::apiEnginIo),
      _networkManager(),
      _uploadChunkSize(512 * 1024),
      _authenticationState(QtCloudServices::NotAuthenticated)
{
    assignNetworkManager();

#if defined(ENGINIO_VALGRIND_DEBUG)
    QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
    conf.setCiphers(QList<QSslCipher>() << QSslCipher(QStringLiteral("ECDHE-RSA-DES-CBC3-SHA"), QSsl::SslV3));
    _request.setSslConfiguration(conf);
#endif

    _request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QStringLiteral("application/json"));
}

void QEnginioConnectionPrivate::init()
{
    QObject::connect(static_cast<QEnginioConnection*>(QTC_Q_PTR(this)), &QEnginioConnection::sessionTerminated, AuthenticationStateTrackerFunctor(this));
    QObject::connect(static_cast<QEnginioConnection*>(QTC_Q_PTR(this)), &QEnginioConnection::sessionAuthenticated, AuthenticationStateTrackerFunctor(this, QtCloudServices::Authenticated));
    QObject::connect(static_cast<QEnginioConnection*>(QTC_Q_PTR(this)), &QEnginioConnection::sessionAuthenticationError, AuthenticationStateTrackerFunctor(this, QtCloudServices::AuthenticationFailure));
    _request.setHeader(QNetworkRequest::UserAgentHeader,
                       QByteArrayLiteral("Qt:" QT_VERSION_STR " Enginio:" QTCLOUDSERVICES_VERSION_STR " Language:C++"));
}

bool QEnginioConnectionPrivate::finishDelayedReplies()
{
    // search if we can trigger an old finished signal.
    bool needToReevaluate = false;

    do {
        needToReevaluate = false;
        foreach (QEnginioOperation * reply, _delayedReplies) {
            if (!reply->delayFinishedSignal()) {
                reply->dataChanged();
                QEnginioOperationPrivate::get(reply)->emitFinished();
                emitFinished(reply);

//                if (gEnableEnginioDebugInfo) {
//                    _requestData.remove(reply->d_func()->_nreply);    // FIXME it is ugly, and breaks encapsulation
//                }

                _delayedReplies.remove(reply);
                needToReevaluate = true;
            }
        }
    } while (needToReevaluate);

    return !_delayedReplies.isEmpty();
}

QEnginioConnectionPrivate::~QEnginioConnectionPrivate()
{
    foreach (const QMetaObject::Connection & identityConnection, _identityConnections)
    QObject::disconnect(identityConnection);
    foreach (const QMetaObject::Connection & connection, _connections)
    QObject::disconnect(connection);
    QObject::disconnect(_networkManagerConnection);
}


void QEnginioConnectionPrivate::emitSessionTerminated() const
{
    emit static_cast<QEnginioConnection*>(QTC_Q_PTR(this))->sessionTerminated();
}

void QEnginioConnectionPrivate::emitSessionAuthenticated(QEnginioOperation *reply)
{
    QTC_Q(QEnginioConnection);
    emit static_cast<QEnginioConnection*>(q)->sessionAuthenticated(static_cast<QEnginioOperation*>(reply));
}

void QEnginioConnectionPrivate::emitSessionAuthenticationError(QEnginioOperation *reply)
{
    QTC_Q(QEnginioConnection);
    emit static_cast<QEnginioConnection*>(q)->sessionAuthenticationError(static_cast<QEnginioOperation*>(reply));
}

void QEnginioConnectionPrivate::emitFinished(QEnginioOperation *reply)
{
    QTC_Q(QEnginioConnection);
    emit static_cast<QEnginioConnection*>(q)->finished(static_cast<QEnginioOperation*>(reply));
}

void QEnginioConnectionPrivate::emitError(QEnginioOperation *reply)
{
    QTC_Q(QEnginioConnection);
    emit static_cast<QEnginioConnection*>(q)->error(static_cast<QEnginioOperation*>(reply));
}

QEnginioOperation * QEnginioConnectionPrivate::createReply(QNetworkReply *nreply)
{
    return new QEnginioOperation(this, nreply);
}

void QEnginioConnectionPrivate::uploadChunk(QEnginioOperation *ereply, QIODevice *device, qint64 startPos)
{
    QUrl serviceUrl = _serviceUrl;
    {
        QString path;
        QByteArray errorMsg;

        if (!getPath(ereply->data(), QtCloudServices::FileChunkUploadOperation, &path, &errorMsg).successful()) {
            Q_UNREACHABLE();    // sequential upload can not have an invalid path!
        }

        serviceUrl.setPath(path);
    }

    QNetworkRequest req = prepareRequest(serviceUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QtCloudServicesConstants::Application_octet_stream);

    // Content-Range: bytes {chunkStart}-{chunkEnd}/{totalFileSize}
    qint64 size = device->size();
    qint64 endPos = qMin(startPos + _uploadChunkSize, size);
    req.setRawHeader(QtCloudServicesConstants::Content_Range,
                     QByteArray::number(startPos) + QtCloudServicesConstants::Minus
                     + QByteArray::number(endPos) + QtCloudServicesConstants::Div
                     + QByteArray::number(size));

    // qDebug() << "Uploading chunk from " << startPos << " to " << endPos << " of " << size;

    Q_ASSERT(device->isOpen());

    ChunkDevice *chunkDevice = new ChunkDevice(device, startPos, _uploadChunkSize);
    chunkDevice->open(QIODevice::ReadOnly);

    QNetworkReply *reply = networkManager()->put(req, chunkDevice);
    chunkDevice->setParent(reply);
    _chunkedUploads.insert(reply, qMakePair(device, endPos));
    ereply->setNetworkReply(reply);
    _connections.append(QObject::connect(reply, &QNetworkReply::uploadProgress, UploadProgressFunctor(this, reply)));
}

QByteArray QEnginioConnectionPrivate::constructErrorMessage(const QByteArray &msg)
{
    static QByteArray msgBegin = QByteArrayLiteral("{\"errors\": [{\"message\": \"");
    static QByteArray msgEnd = QByteArrayLiteral("\",\"reason\": \"BadRequest\"}]}");
    return msgBegin + msg + msgEnd;
}

#endif

/*
** Private Implementation
*/

QEnginioConnectionPrivate::ReplyFinishedFunctor::ReplyFinishedFunctor(QEnginioConnection::dvar aConnection)
    : iConnection(aConnection)
{
}
void QEnginioConnectionPrivate::ReplyFinishedFunctor::operator ()(QNetworkReply *aNetworkReply)
{
    QEnginioConnection::dvar connection;

    if (connection = iConnection.lock()) {
        connection->replyFinished(aNetworkReply);
    }
}

QThreadStorage < QWeakPointer<QNetworkAccessManager> > QEnginioConnectionPrivate::gNetworkManager;

QEnginioConnectionPrivate::QEnginioConnectionPrivate()
{
}

QEnginioConnectionPrivate::QEnginioConnectionPrivate(const QEnginioDataStorage &aEnginioDataStorage)
    : iEnginioDataStorage(aEnginioDataStorage)
{
    iNetworkManager = gNetworkManager.localData().toStrongRef();

    if (!iNetworkManager) {
        iNetworkManager = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager());
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0) && !defined(QT_NO_SSL) && !defined(ENGINIO_VALGRIND_DEBUG)
        iNetworkManager->connectToHostEncrypted(aEnginioDataStorage.instanceAddress().host());
#else
# error "SSL Connection required."
#endif
        gNetworkManager.setLocalData(iNetworkManager);
    }
}

QEnginioConnectionPrivate::~QEnginioConnectionPrivate()
{
    if (iNetworkManager) {
        disconnect(iNetworkManagerConnection);
    }
}

bool QEnginioConnectionPrivate::isValid() const
{
    if (!iEnginioDataStorage) {
        return false;
    }

    return true;
}

QSharedPointer<QNetworkAccessManager> QEnginioConnectionPrivate::networkManager() const
{
    return iNetworkManager;
}

QEnginioOperation QEnginioConnectionPrivate::customRequest(const QEnginioRequest &aRequest)
{
    QBuffer *buffer = 0;
    QNetworkRequest request;
    QEnginioOperation operation(*q<QEnginioConnection>(), aRequest);
    QByteArray verb;
    QNetworkReply *reply;

    switch (aRequest.operation()) {
    case QtCloudServices::RESTOperationGet:
        verb = QtCloudServicesConstants::RESTOperationGet;
        break;

    case QtCloudServices::RESTOperationPost:
        verb = QtCloudServicesConstants::RESTOperationPost;
        break;

    case QtCloudServices::RESTOperationPut:
        verb = QtCloudServicesConstants::RESTOperationPut;
        break;

    case QtCloudServices::RESTOperationDelete:
        verb = QtCloudServicesConstants::RESTOperationDelete;
        break;
    }

    request = prepareRequest(aRequest.path(),
                             aRequest.urlQuery(),
                             aRequest.extraHeaders());

    QJsonObject jsonPayload = aRequest.payload();

    if (!jsonPayload.empty()) {
        QByteArray payload;
        payload = QJsonDocument(jsonPayload).toJson(QJsonDocument::Compact);
        buffer = new QBuffer();
        buffer->setData(payload);
        buffer->open(QIODevice::ReadOnly);
    }

    reply = networkManager()->sendCustomRequest(request, verb, buffer);
    operation.d<QEnginioOperation>()->setNetworkReply(reply);

    if (buffer) {
        buffer->setParent(reply);
    }

    return operation;
}

void QEnginioConnectionPrivate::replyFinished(QNetworkReply *aNetworkReply)
{
    QEnginioOperation operation;
    operation = iReplyOperationMap.take(aNetworkReply);

    if (!operation) {
        return;
    }

    operation.d<QEnginioOperation>()->operationFinished();

#if 0

    if (aNetworkReply->error() != QNetworkReply::NoError) {
        QPair<QIODevice *, qint64> deviceState = _chunkedUploads.take(nreply);
        delete deviceState.first;
        emitError(ereply);
    }

    // continue chunked upload
    else if (_chunkedUploads.contains(nreply)) {
        QPair<QIODevice *, qint64> deviceState = _chunkedUploads.take(nreply);
        QString status = ereply->data().value(QtCloudServicesConstants::status).toString();

        if (status == QtCloudServicesConstants::empty || status == QtCloudServicesConstants::incomplete) {
            Q_ASSERT(ereply->data().value(QtCloudServicesConstants::objectType).toString() == QtCloudServicesConstants::files);
            uploadChunk(ereply, deviceState.first, deviceState.second);
            return;
        }

        // should never get here unless upload was successful
        Q_ASSERT(status == QtCloudServicesConstants::complete);
        delete deviceState.first;

        if (_connections.count() * 2 > _chunkedUploads.count()) {
            _connections.removeAll(QMetaObject::Connection());
        }
    }

    if (Q_UNLIKELY(ereply->delayFinishedSignal())) {
        // delay emittion of finished signal for autotests
        _delayedReplies.insert(ereply);
    } else {
        ereply->dataChanged();
        QEnginioOperationPrivate::get(ereply)->emitFinished();
        emitFinished(ereply);

        if (gEnableEnginioDebugInfo) {
            _requestData.remove(nreply);
        }
    }

    if (Q_UNLIKELY(_delayedReplies.count())) {
        finishDelayedReplies();
    }

#endif
}


void QEnginioConnectionPrivate::registerReply(QNetworkReply *aNetworkReply, const QEnginioOperation &aOperation)
{
    aNetworkReply->setParent(aOperation.d<QEnginioOperation>().get());
    iReplyOperationMap[aNetworkReply] = aOperation;
}

void QEnginioConnectionPrivate::unregisterReply(QNetworkReply *aNetworkReply)
{
    iReplyOperationMap.remove(aNetworkReply);
}

QNetworkRequest QEnginioConnectionPrivate::prepareRequest(const QString &aPath,
        const QUrlQuery &aQuery,
        const QJsonObject &aExtraHeaders)
{
    QUrl url, relativeUrl;
    QByteArray requestId;
    QNetworkRequest request;

    if (!iEnginioDataStorage) {
        qCritical() << tr("QEnginioConnectionObject not bound to QEnginioDataStoreObject.");
        return QNetworkRequest();
    }

    relativeUrl.setPath(aPath);
    url = iEnginioDataStorage.instanceAddress().resolved(relativeUrl);

    requestId = QUuid::createUuid().toByteArray();
    // Remove unneeded pretty-formatting.
    // before: "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"
    // after:  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    requestId.chop(1);      // }
    requestId.remove(0, 1); // {
    requestId.remove(23, 1);
    requestId.remove(18, 1);
    requestId.remove(13, 1);
    requestId.remove(8, 1);

    if (!aQuery.isEmpty()) {
        url.setQuery(aQuery);
    }

    request.setUrl(url);

    request.setRawHeader(QtCloudServicesConstants::X_Request_Id, requestId);
    request.setRawHeader(QtCloudServicesConstants::Host, url.host().toLatin1());
    request.setRawHeader(QtCloudServicesConstants::Accept_Encoding,
                         QtCloudServicesConstants::Accept_Encoding_Any);
    request.setRawHeader(QtCloudServicesConstants::User_Agent,
                         QtCloudServicesConstants::User_Agent_Default);
    request.setRawHeader(QtCloudServicesConstants::Enginio_Backend_Id,
                         iEnginioDataStorage.backendId().toLatin1());

    if (!aExtraHeaders.empty()) {
        QJsonObject::const_iterator end = aExtraHeaders.constEnd();

        for (QJsonObject::const_iterator i = aExtraHeaders.constBegin(); i != end; i++) {
            QByteArray headerName = i.key().toUtf8();
            QByteArray headerValue = i.value().toString().toUtf8();
            request.setRawHeader(headerName, headerValue);
        }
    }

    return request;
}

/*
** Public Implementation
*/
QEnginioConnection::QEnginioConnection(const QEnginioDataStorage &aEnginioDataStorage)
    : QCloudServicesObject(QEnginioConnection::dvar(new QEnginioConnectionPrivate(aEnginioDataStorage)))
{
    QEnginioConnection::dvar pimpl = d<QEnginioConnection>();

    if (pimpl->iNetworkManager) {
        pimpl->iNetworkManagerConnection = QObject::connect(pimpl->iNetworkManager.data(),
                                           &QNetworkAccessManager::finished,
                                           QEnginioConnectionPrivate::ReplyFinishedFunctor(pimpl));
    }
}
QEnginioConnection::QEnginioConnection(QObject *aParent)
    : QCloudServicesObject(QEnginioConnection::dvar(new QEnginioConnectionPrivate()), aParent)
{
}

QEnginioConnection::QEnginioConnection(const QEnginioConnection &aOther)
    : QCloudServicesObject(aOther.d<QEnginioConnection>())
{
}
QEnginioConnection & QEnginioConnection::operator=(const QEnginioConnection &aOther)
{
    setPIMPL(aOther.d<QEnginioConnection>());
    return *this;
}

QEnginioConnection::~QEnginioConnection()
{
    // qDeleteAll(findChildren<QEnginioOperation *>());
}

bool QEnginioConnection::operator!() const
{
    return !isValid();
}

bool QEnginioConnection::isValid() const
{
    if (isNull()) {
        return false;
    }

    return d<QEnginioConnection>()->isValid();
}

/*!
\brief Get the QNetworkAccessManager used by the Enginio library.

\note that this QNetworkAccessManager may be shared with other QEnginioConnection instances
and it is owned by them.
*/
QSharedPointer<QNetworkAccessManager> QEnginioConnection::networkManager() const
{
    return d<const QEnginioConnection>()->networkManager();
}


/*!
\brief Create custom request to the enginio REST API

\a aRESTOperation Supported Verb to the server.
\a aPath The for custom request. This path is appended to current endpoint url.
\a aData optional JSON object possibly containing custom payload
\a aExtraHeaders optional JSON object possibly containing additional extra headers.

\return QEnginioOperation containing the status and the result once it is finished.
\sa QEnginioOperation, create(), query(), update(), remove()
\internal
*/
QEnginioOperation QEnginioConnection::customRequest(const QEnginioRequest &aRequest)
{
    return d<QEnginioConnection>()->customRequest(aRequest);
}



//
// OBSOLATE METHODS
//


#if 0
void QEnginioConnection::setBackendId(const QByteArray &backendId)
{
    QTC_D(QEnginioConnection);

    if (d->_backendId != backendId) {
        d->_backendId = backendId;
        d->_request.setRawHeader("Enginio-Backend-Id", d->_backendId);
        // emit backendIdChanged(backendId);
    }
}
#endif



/*!
\property QEnginioConnection::identity
Property that represents a user. Setting the property will create an asynchronous authentication request,
the result of it updates \l{QEnginioConnection::authenticationState()}{authenticationState}
QEnginioConnection does not take ownership of the \a identity object. The object
has to be valid to keep the authenticated session alive. When the identity object is deleted the session
is terminated. It is allowed to assign a null pointer to the property to terminate the session.
\sa EnginioIdentity EnginioOAuth2Authentication
*/
EnginioIdentity * QEnginioConnection::identity() const
{
    /*
    QTC_D(const QEnginioConnection);
    return d->identity();
    */
    return NULL;
}

void QEnginioConnection::setIdentity(EnginioIdentity *identity)
{
    /*
    QTC_D(QEnginioConnection);

    if (d->_identity == identity) {
        return;
    }

    d->setIdentity(identity);
    */
}


QtCloudServices::AuthenticationState QEnginioConnection::authenticationState() const
{
    /*
    QTC_D(const QEnginioConnection);
    return d->authenticationState();
    */

    return QtCloudServices::AuthenticationState::NotAuthenticated;
}

/*!
\internal
Tries to emit finished signal from all replies that used to be delayed.
\return false if all replies were finished, true otherwise.
*/
bool QEnginioConnection::finishDelayedReplies()
{
    /*
    QTC_D(QEnginioConnection);
    return d->finishDelayedReplies();

    */
    return false;
}

/*!
\brief Fulltext search on the Enginio backend

The \a query is JSON sent to the backend to perform a fulltext search.
Note that the search requires the searched properties to be indexed (on the server, configureable in the backend).


\return QEnginioOperation containing the status and the result once it is finished.
\sa QEnginioOperation, create(), query(), update(), remove(), {cloudaddressbook}{Address book example},
{https://engin.io/documentation/rest/parameters/fulltext_query}{JSON request structure}
*/
QEnginioOperation QEnginioConnection::fullTextSearch(const QJsonObject &query)
{
    /*
    QTC_D(QEnginioConnection);

    QNetworkReply *nreply = d->query<QJsonObject>(query, QtCloudServices::SearchOperation);
    QEnginioOperation *ereply = new QEnginioOperation(d, nreply);
    return ereply;
    */

    return QEnginioOperation();
}

/*!
\include client-query.qdocinc

To query the database for all objects of the type "objects.todo":
\snippet QEnginioConnection/tst_QEnginioConnection.cpp query-todo

\return an QEnginioOperation containing the status and the result once it is finished.
*/
QEnginioOperation QEnginioConnection::query(const QJsonObject &query, const QtCloudServices::Operation operation)
{
    /*
    QTC_D(QEnginioConnection);

    QNetworkReply *nreply = d->query<QJsonObject>(query, operation);
    QEnginioOperation *ereply = new QEnginioOperation(d, nreply);

    return ereply;
    */

    return QEnginioOperation();
}

/*!
\include client-create.qdocinc

\snippet QEnginioConnection/tst_QEnginioConnection.cpp create-todo

To add a new member to a usergroup, the JSON needs to look like the example below.
\code
{
"id": "groupId",
"member": { "id": "userId", "objectType": "users" }
}
\endcode
It can be constructed like this:
\snippet QEnginioConnection/tst_QEnginioConnection.cpp create-newmember

\return an QEnginioOperation containing the status and data once it is finished.
*/
QEnginioOperation QEnginioConnection::create(const QJsonObject &object, const QtCloudServices::Operation operation)
{
    /*
    QTC_D(QEnginioConnection);

    QNetworkReply *nreply = d->create<QJsonObject>(object, operation);
    QEnginioOperation *ereply = new QEnginioOperation(d, nreply);

    return ereply;
    */

    return QEnginioOperation();
}

/*!
\include client-update.qdocinc

In C++, the updating of the ACL could be done like this:
\snippet QEnginioConnection/tst_QEnginioConnection.cpp update-access

\return an QEnginioOperation containing the status of the query and the data once it is finished.
*/
QEnginioOperation QEnginioConnection::update(const QJsonObject &object, const QtCloudServices::Operation operation)
{
    /*
    QTC_D(QEnginioConnection);

    QNetworkReply *nreply = d->update<QJsonObject>(object, operation);
    QEnginioOperation *ereply = new QEnginioOperation(d, nreply);

    return ereply;
    */

    return QEnginioOperation();
}

/*!
\include client-remove.qdocinc

To remove a todo object:
\snippet QEnginioConnection/tst_QEnginioConnection.cpp remove-todo

\return an QEnginioOperation containing the status and the data once it is finished.
*/
QEnginioOperation QEnginioConnection::remove(const QJsonObject &object, const QtCloudServices::Operation operation)
{
    /*
    QTC_D(QEnginioConnection);

    QNetworkReply *nreply = d->remove<QJsonObject>(object, operation);
    QEnginioOperation *ereply = new QEnginioOperation(d, nreply);

    return ereply;
    */
    return QEnginioOperation();
}


/*!
\brief Stores a \a file attached to an \a object in Enginio

Each uploaded file needs to be associated with an object in the database.
\note The upload will only work with the propper server setup: in the dashboard create a property
of the type that you will use. Set this property to be a reference to files.

Each uploaded file needs to be associated with an object in the database.

In order to upload a file, first create an object:
\snippet files/tst_files.cpp upload-create-object

Then do the actual upload:
\snippet files/tst_files.cpp upload

Note: There is no need to directly delete files.
Instead when the object that contains the link to the file gets deleted,
the file will automatically be deleted as well.

\sa downloadUrl()
*/
QEnginioOperation QEnginioConnection::uploadFile(const QJsonObject &object, const QUrl &file)
{
    /*
    QTC_D(QEnginioConnection);

    QNetworkReply *nreply = d->uploadFile<QJsonObject>(object, file);
    QEnginioOperation *ereply = new QEnginioOperation(d, nreply);

    return ereply;
    */
    return QEnginioOperation();
}

/*!
\brief Get a temporary URL for a file stored in Enginio

From this URL a file can be downloaded. The URL is valid for a certain amount of time as indicated
in the reply.

\snippet files/tst_files.cpp download
The propertyName can be anything, but it must be the same as the one used to upload the file with.
This way one object can have several files attached to itself (one per propertyName).

If a file provides several variants, it is possible to request a variant by including it in the
\a object.
\code
{
"id": "abc123",
"variant": "thumbnail"
}
\endcode
*/
QEnginioOperation QEnginioConnection::downloadUrl(const QJsonObject &object)
{
    /*
    QTC_D(QEnginioConnection);

    QNetworkReply *nreply = d->downloadUrl<QJsonObject>(object);
    QEnginioOperation *ereply = new QEnginioOperation(d, nreply);

    return ereply;
    */
    return QEnginioOperation();
}

QT_END_NAMESPACE
