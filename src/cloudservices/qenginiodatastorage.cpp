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

#include "QtCloudServices/private/qenginiodatastorage_p.h"
#include "QtCloudServices/private/qenginiocollection_p.h"
#include "QtCloudServices/private/qenginioconnection_p.h"

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

QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/
QEnginioDataStoragePrivate::QEnginioDataStoragePrivate()
{

}
QEnginioDataStoragePrivate::QEnginioDataStoragePrivate(const QUrl &aInstanceAddress, const QString &aBackendId,
        QEnginioDataStoragePrivate *aPrevInstance)
    : QCloudServicesObjectPrivate(),
      iInstanceAddress(aInstanceAddress), iBackendId(aBackendId),
      iForwarding(false)
{
    qRegisterMetaType<QEnginioConnection*>();
    qRegisterMetaType<QEnginioOperation*>();

    /*
    qRegisterMetaType<EnginioModel*>();
    // qRegisterMetaType<EnginioIdentity*>();
    // qRegisterMetaType<EnginioOAuth2Authentication*>();
    */
    qRegisterMetaType<QtCloudServices::Operation>();
    qRegisterMetaType<QtCloudServices::AuthenticationState>();
    qRegisterMetaType<QtCloudServices::ErrorType>();

    if (aPrevInstance)  {
        iUsername = aPrevInstance->iUsername;
        iPassword = aPrevInstance->iPassword;
    }
}


QEnginioDataStoragePrivate::~QEnginioDataStoragePrivate()
{
    if (iForwarding) {
        unbindForwarding();
    }
}

QUrl QEnginioDataStoragePrivate::instanceAddress() const
{
    return iInstanceAddress;
}

QString QEnginioDataStoragePrivate::backendId() const
{
    return iBackendId;
}

QString QEnginioDataStoragePrivate::username() const
{
    return iUsername;
}

void QEnginioDataStoragePrivate::setUsername(const QString &aUsername)
{
    if (iUsername == aUsername) {
        return;
    }

    iUsername = aUsername;

    emit usernameChanged(aUsername);
}

QString QEnginioDataStoragePrivate::password() const
{
    return iPassword;
}

void QEnginioDataStoragePrivate::setPassword(const QString &aPassword)
{
    if (iPassword == aPassword) {
        return;
    }

    iPassword = aPassword;

    emit passwordChanged(aPassword);
}


QEnginioConnection QEnginioDataStoragePrivate::reserveConnection()
{
    QEnginioConnection connection;
    QMutexLocker locker(&iLock);

    if (!iConnectionPool.empty()) {
        connection = iConnectionPool.front();
        iConnectionPool.pop_front();
    }

    if (!connection) {
        connection = QEnginioConnection(*q<QEnginioDataStorage>());
    }

    return connection;
}
void QEnginioDataStoragePrivate::releaseConnection(const QEnginioConnection &aConnection)
{
    QMutexLocker locker(&iLock);

    iConnectionPool.push_back(aConnection);
}

QEnginioCollection QEnginioDataStoragePrivate::collection(const QString &aCollectionName)
{
    QMutexLocker locker(&iLock);

    QEnginioCollection collection;
    QMap<QString, QEnginioCollection>::iterator i;

    i = iCollections.find(aCollectionName);

    if (i != iCollections.end()) {
        collection = i.value();
    } else {
        collection = QEnginioCollection(*q<QEnginioDataStorage>(), aCollectionName);
        iCollections.insert(aCollectionName, collection);
    }

    return collection;
}

void QEnginioDataStoragePrivate::bindForwarding(QEnginioDataStorage *aInstance)
{
    if (iForwarding) {
        unbindForwarding();
    }

    iUsernameForwarding =
        connect(this, &QEnginioDataStoragePrivate::usernameChanged,
                aInstance, &QEnginioDataStorage::usernameChanged);
    iPasswordForwarding =
        connect(this, &QEnginioDataStoragePrivate::passwordChanged,
                aInstance, &QEnginioDataStorage::passwordChanged);
    iForwarding = true;

}
void QEnginioDataStoragePrivate::unbindForwarding()
{
    if (!iForwarding) {
        return;
    }

    disconnect(iUsernameForwarding);
    disconnect(iPasswordForwarding);

    iForwarding = false;
}


/*
** Public Interface
*/
QEnginioDataStorage::QEnginioDataStorage(QObject *aParent)
    : QCloudServicesObject(QEnginioDataStorage::dvar(new QEnginioDataStoragePrivate), aParent)
{
}

QEnginioDataStorage::QEnginioDataStorage(const QUrl &instanceAddress, const QString &backendId, QObject *aParent)
    : QCloudServicesObject(QEnginioDataStorage::dvar(new QEnginioDataStoragePrivate), aParent)
{
    setInstanceAddress(instanceAddress);
    setBackendId(backendId);
}

QEnginioDataStorage::QEnginioDataStorage(const QString &instanceAddress, const QString &backendId, QObject *aParent)
    : QCloudServicesObject(QEnginioDataStorage::dvar(new QEnginioDataStoragePrivate), aParent)
{
    setInstanceAddress(QUrl(instanceAddress));
    setBackendId(backendId);
}

QEnginioDataStorage::QEnginioDataStorage(const QEnginioDataStorage &aEnginioDataStorage)
    : QCloudServicesObject(aEnginioDataStorage.d<QEnginioDataStorage>())
{

}

QEnginioDataStorage::~QEnginioDataStorage()
{

}

QEnginioDataStorage& QEnginioDataStorage::operator=(const QEnginioDataStorage &aEnginioDataStorage)
{
    setPIMPL(aEnginioDataStorage.d<QEnginioDataStorage>());
    return *this;
}

bool QEnginioDataStorage::operator!() const
{
    return !isValid();
}
bool QEnginioDataStorage::isValid() const
{
    if (isNull()) {
        return false;
    }

    if (instanceAddress().isEmpty() || backendId().isEmpty()) {
        return false;
    }

    return true;
}

void QEnginioDataStorage::setBackend(const QUrl &aInstanceAddress, const QString &aBackendId)
{
    bool chgAddress, chgId;
    QEnginioDataStorage::dvar impl;

    impl = d<QEnginioDataStorage>();

    chgAddress = (impl->instanceAddress() != aInstanceAddress);
    chgId = (impl->backendId() != aBackendId);

    if (!chgAddress && !chgId) {
        return;
    }

    impl->unbindForwarding();

    impl = QEnginioDataStorage::dvar(new QEnginioDataStoragePrivate(aInstanceAddress, aBackendId));
    setPIMPL(impl);

    impl->bindForwarding(this);

    if (chgAddress) {
        emit instanceAddressChanged(aInstanceAddress);
    }

    if (chgId) {
        emit backendIdChanged(aBackendId);
    }

    emit backendChanged();
}

QUrl QEnginioDataStorage::instanceAddress() const
{
    return d<const QEnginioDataStorage>()->instanceAddress();
}
void QEnginioDataStorage::setInstanceAddress(const QUrl &aInstanceAddress)
{
    setBackend(aInstanceAddress, backendId());
}
void QEnginioDataStorage::setInstanceAddressString(const QString &aInstanceAddress)
{
    setInstanceAddress(QUrl(aInstanceAddress));
}

QString QEnginioDataStorage::backendId() const
{
    return d<const QEnginioDataStorage>()->backendId();
}
void QEnginioDataStorage::setBackendId(const QString &aBackendId)
{
    setBackend(instanceAddress(), aBackendId);
}

QString QEnginioDataStorage::username() const
{
    return d<const QEnginioDataStorage>()->username();
}

QString QEnginioDataStorage::password() const
{
    return d<const QEnginioDataStorage>()->password();
}

void QEnginioDataStorage::setUsername(const QString &aUsername)
{
    d<QEnginioDataStorage>()->setUsername(aUsername);
}

void QEnginioDataStorage::setPassword(const QString &aPassword)
{
    d<QEnginioDataStorage>()->setPassword(aPassword);
}

QEnginioCollection QEnginioDataStorage::collection(const QString &aCollectionName)
{
    return d<QEnginioDataStorage>()->collection(aCollectionName);
}

QEnginioConnection QEnginioDataStorage::reserveConnection()
{
    return d<QEnginioDataStorage>()->reserveConnection();
}
void QEnginioDataStorage::releaseConnection(const QEnginioConnection &aConnection)
{
    d<QEnginioDataStorage>()->releaseConnection(aConnection);
}

QT_END_NAMESPACE


#if 0

#include "stdafx.h"

#include <QtCloudServices/enginioidentity.h>
#include <QtCloudServices/enginiooauth2authentication.h>
#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qcloudservicesobject_p.h>
#include <QtCloudServices/QEnginioOperation.h>

#include <QtCore/qjsonobject.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qstring.h>
#include <QtNetwork/qnetworkreply.h>

QT_BEGIN_NAMESPACE

class EnginioUserPassAuthenticationPrivate : public EnginioIdentityPrivate {
    template<typename T>
    class SessionSetterFunctor {
        QEnginioConnectionPrivate *_enginio;
        QNetworkReply *_reply;
        EnginioUserPassAuthenticationPrivate *_auth;
    public:
        SessionSetterFunctor(QEnginioConnectionPrivate *enginio, QNetworkReply *reply, EnginioUserPassAuthenticationPrivate *auth)
            : _enginio(enginio)
            , _reply(reply)
            , _auth(auth)
        {}
        void operator ()()
        {
            QEnginioOperation *ereply = _enginio->createReply(_reply);

            if (_reply->error() != QNetworkReply::NoError) {
                emit _enginio->emitSessionAuthenticationError(ereply);
            } else {
                _auth->thisAs<T>()->proccessToken(_enginio, ereply);
                _enginio->emitSessionAuthenticated(ereply);
            }
        }
    };

    QPointer<QNetworkReply> _reply;
    QMetaObject::Connection _replyFinished;
    QMetaObject::Connection _enginioDestroyed;

public:
    EnginioUserPassAuthenticationPrivate()
        : EnginioIdentityPrivate()
    {}
public:

    ~EnginioUserPassAuthenticationPrivate();

    template<class Derived>
    Derived *thisAs()
    {
        return static_cast<Derived*>(this);
    }

    void cleanupConnections()
    {
        if (_reply) {
            QObject::disconnect(_replyFinished);
            QObject::disconnect(_enginioDestroyed);
            QObject::connect(_reply.data(), &QNetworkReply::finished, _reply.data(), &QNetworkReply::deleteLater);
            _reply = 0;
        }
    }

    template<typename Derived>
    void prepareSessionToken(QEnginioConnectionPrivate *enginio)
    {
        cleanupConnections();

        _reply = thisAs<Derived>()->makeRequest(enginio);
        enginio->setAuthenticationState(QtCloudServices::Authenticating);
        _replyFinished = QObject::connect(_reply.data(), &QNetworkReply::finished, SessionSetterFunctor<Derived>(enginio, _reply.data(), this));
        _enginioDestroyed = QObject::connect(QTC_Q_PTR(enginio), &QEnginioConnection::destroyed, DisconnectConnection(this));
    }

    template<typename Derived>
    void removeSessionToken(QEnginioConnectionPrivate *enginio)
    {
        cleanupConnections();
        thisAs<Derived>()->cleanupClient(enginio);
        _reply = 0;
        enginio->emitSessionTerminated();
    }
};


QNetworkReply *makeRequest(QEnginioConnectionPrivate *enginio)
{
    QByteArray data;
    {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem(QtCloudServicesConstants::grant_type, QtCloudServicesConstants::password);
        urlQuery.addQueryItem(QtCloudServicesConstants::username, _user);
        urlQuery.addQueryItem(QtCloudServicesConstants::password, _pass);
        data = urlQuery.query().toUtf8();
    }

    QUrl url(enginio->_serviceUrl);
    url.setPath(QtCloudServicesConstants::v1_auth_oauth2_token);

    QNetworkRequest request(enginio->prepareRequest(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QtCloudServicesConstants::Application_x_www_form_urlencoded);
    request.setRawHeader(QtCloudServicesConstants::Accept, QtCloudServicesConstants::Application_json);

    return enginio->networkManager()->post(request, data);
}

void proccessToken(QEnginioConnectionPrivate *enginio, QEnginioOperation *ereply)
{
    QByteArray header;
    header = QtCloudServicesConstants::Bearer_ + ereply->data()[QtCloudServicesConstants::access_token].toString().toUtf8();
    enginio->_request.setRawHeader(QtCloudServicesConstants::Authorization, header);
}

void cleanupClient(QEnginioConnectionPrivate *enginio)
{
    enginio->_request.setRawHeader(QtCloudServicesConstants::Authorization, QByteArray());
}
};

/*!
\class EnginioOAuth2Authentication
\since 5.3
\inmodule enginio-qt
\ingroup enginio-client
\brief Represents a user that is authenticated directly by the backend using OAuth2 standard.

This class can authenticate a user by verifying the user's login and password.
The user has to exist in the backend already.

To authenticate an instance of QEnginioConnection called \a client such code may be used:
\code
EnginioOAuth2Authentication identity;
identity.setUser(_user);
identity.setPassword(_user);

client.setIdentity(&identity);
\endcode

Setting the identity will trigger an asynchronous request, resulting in QEnginioConnection::authenticationState()
changing.

\sa QEnginioConnection::authenticationState() QEnginioConnection::identity() QEnginioConnection::sessionAuthenticated()
\sa QEnginioConnection::sessionAuthenticationError() QEnginioConnection::sessionTerminated()
*/

/*!
Constructs a EnginioPasswordOAuth2 instance with \a parent as QObject parent.
*/
EnginioOAuth2Authentication::EnginioOAuth2Authentication(QObject *parent)
    : EnginioIdentity(*new EnginioOAuth2AuthenticationPrivate(), parent)
{
    connect(this, &EnginioOAuth2Authentication::userChanged, this, &EnginioIdentity::dataChanged);
    connect(this, &EnginioOAuth2Authentication::passwordChanged, this, &EnginioIdentity::dataChanged);
}

/*!
Destructs this EnginioPasswordOAuth2 instance.
*/
EnginioOAuth2Authentication::~EnginioOAuth2Authentication()
{
    emit aboutToDestroy();
}


/*!
\internal
*/
void EnginioOAuth2Authentication::prepareSessionToken(QEnginioConnectionPrivate *enginio)
{
    Q_ASSERT(enginio);
    Q_ASSERT(enginio->identity());
    QTC_D(EnginioOAuth2Authentication);
    d->prepareSessionToken<EnginioOAuth2AuthenticationPrivate>(enginio);
}

/*!
\internal
*/
void EnginioOAuth2Authentication::removeSessionToken(QEnginioConnectionPrivate *enginio)
{
    Q_ASSERT(enginio);
    Q_ASSERT(enginio->identity());
    QTC_D(EnginioOAuth2Authentication);
    d->removeSessionToken<EnginioOAuth2AuthenticationPrivate>(enginio);
}

void DisconnectConnection::operator ()() const
{
    auth->cleanupConnections();
}

EnginioUserPassAuthenticationPrivate::~EnginioUserPassAuthenticationPrivate()
{
    cleanupConnections();
}

QT_END_NAMESPACE


#endif
