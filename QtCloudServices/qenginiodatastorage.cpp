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

QT_BEGIN_NAMESPACE

/*
** Shared Implementation
*/
QEnginioDataStorageObject::QEnginioDataStorageObject(const QUrl &aBackendAddress, const QString &aBackendId)
    : iBackendAddress(aBackendAddress), iBackendId(aBackendId)
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
}

QUrl QEnginioDataStorageObject::backendAddress() const
{
    return iBackendAddress;
}
QString QEnginioDataStorageObject::backendId() const
{
    return iBackendId;
}

QString QEnginioDataStorageObject::username() const
{
    return iUsername;
}

void QEnginioDataStorageObject::setUsername(const QString &aUsername)
{
    /* Locker Scope */ {
        QMutexLocker locker(&iLock);

        if (iUsername == aUsername) {
            return;
        }

        iUsername = aUsername;
    }

    emit usernameChanged(aUsername);
}

QString QEnginioDataStorageObject::password() const
{
    return iPassword;
}

void QEnginioDataStorageObject::setPassword(const QString &aPassword)
{
    /* Locker Scope */ {
        QMutexLocker locker(&iLock);

        if (iPassword == aPassword) {
            return;
        }

        iPassword = aPassword;
    }

    emit passwordChanged(aPassword);
}

QSharedPointer<QEnginioConnectionObject> QEnginioDataStorageObject::reserveConnection(QSharedPointer<QEnginioDataStorageObject> aSelf)
{
    QSharedPointer<QEnginioConnectionObject> connection;
    QMutexLocker locker(&iLock);

    if (!iConnectionPool.empty()) {
        connection = iConnectionPool.front();
        iConnectionPool.pop_front();
    }

    if (!connection) {
        connection = QEnginioConnectionObject::get(aSelf);
    }

    return connection;
}
void QEnginioDataStorageObject::releaseConnection(QSharedPointer<QEnginioConnectionObject> aConnection)
{
    QMutexLocker locker(&iLock);

    iConnectionPool.push_back(aConnection);
}

QSharedPointer<QEnginioCollectionObject> QEnginioDataStorageObject::collection(const QString &aCollectionName,
        QSharedPointer<QEnginioDataStorageObject> aSelf)
{
    QMutexLocker locker(&iLock);

    QSharedPointer<QEnginioCollectionObject> collection;
    QMap<QString, QSharedPointer<QEnginioCollectionObject>>::iterator i;

    i = iCollections.find(aCollectionName);

    if (i != iCollections.end()) {
        collection = i.value();
    } else {
        collection = QSharedPointer<QEnginioCollectionObject>(new QEnginioCollectionObject(aSelf, aCollectionName));
        iCollections.insert(aCollectionName, collection);
    }

    return collection;

}

/*
** Private Implementation
*/
QEnginioDataStoragePrivate::QEnginioDataStoragePrivate()
    : QCloudServicesObjectPrivate(),
      iForwarding(false)
{}

QEnginioDataStoragePrivate::~QEnginioDataStoragePrivate()
{
    if (iForwarding) {
        unbindForwarding();
    }
}

void QEnginioDataStoragePrivate::setBackend(const QUrl &aBackendAddress, const QString &aBackendId)
{
    iObject = QSharedPointer<QEnginioDataStorageObject>(new QEnginioDataStorageObject(aBackendAddress, aBackendId));
}

QUrl QEnginioDataStoragePrivate::backendAddress() const
{
    if (iObject) {
        return iObject->backendAddress();
    }

    return QUrl();
}

void QEnginioDataStoragePrivate::setBackendAddress(const QUrl &aBackendAddress)
{
    setBackend(aBackendAddress, backendId());
}

QString QEnginioDataStoragePrivate::backendId() const
{
    if (iObject) {
        return iObject->backendId();
    }

    return QString();
}
void QEnginioDataStoragePrivate::setBackendId(const QString &aBackendId)
{
    setBackend(backendAddress(), aBackendId);
}

QString QEnginioDataStoragePrivate::username() const
{
    if (iObject) {
        return iObject->username();
    }

    return QString();
}

void QEnginioDataStoragePrivate::setUsername(const QString &aUsername)
{
    if (iObject) {
        return iObject->setUsername(aUsername);
    } else {
        qCritical() << tr("Setting username to invalid QEnginioDataStorage - Ignored.");
    }
}

QString QEnginioDataStoragePrivate::password() const
{
    if (iObject) {
        return iObject->password();
    }

    return QString();
}

void QEnginioDataStoragePrivate::setPassword(const QString &aPassword)
{
    if (iObject) {
        return iObject->setPassword(aPassword);
    } else {
        qCritical() << tr("Setting password to invalid QEnginioDataStorage - Ignored.");

    }
}

QSharedPointer<QEnginioConnectionObject> QEnginioDataStoragePrivate::reserveConnection()
{
    QSharedPointer<QEnginioConnectionObject> connection;

    if (iObject) {
        connection = iObject->reserveConnection(iObject);
    }

    return connection;

}
void QEnginioDataStoragePrivate::releaseConnection(QSharedPointer<QEnginioConnectionObject> aConnection)
{
    if (iObject) {
        iObject->releaseConnection(aConnection);
    }
}

QSharedPointer<QEnginioCollectionObject> QEnginioDataStoragePrivate::collection(const QString &aCollectionName)
{
    QSharedPointer<QEnginioCollectionObject> collection;

    if (iObject) {
        collection = iObject->collection(aCollectionName, iObject);
    }

    return collection;
}

void QEnginioDataStoragePrivate::bindForwarding(QEnginioDataStorage *aInstance)
{
    if (!iObject) {
        return;
    }

    if (iForwarding) {
        unbindForwarding();
    }

    iUsernameForwarding =
        connect(iObject.data(), &QEnginioDataStorageObject::usernameChanged,
                aInstance, &QEnginioDataStorage::usernameChanged);
    iPasswordForwarding =
        connect(iObject.data(), &QEnginioDataStorageObject::passwordChanged,
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
QEnginioDataStorage::QEnginioDataStorage(QObject *parent)
    : QCloudServicesObject(*new QEnginioDataStoragePrivate(), parent)
{
}

QEnginioDataStorage::QEnginioDataStorage(const QUrl &backendAddress, const QString &backendId, QObject *parent)
    : QCloudServicesObject(*new QEnginioDataStoragePrivate(), parent)
{
    setBackendAddress(backendAddress);
    setBackendId(backendId);
}

QEnginioDataStorage::QEnginioDataStorage(const QString &backendAddress, const QString &backendId, QObject *parent)
    : QCloudServicesObject(*new QEnginioDataStoragePrivate(), parent)
{
    setBackendAddress(QUrl(backendAddress));
    setBackendId(backendId);
}

QEnginioDataStorage::~QEnginioDataStorage()
{

}

void QEnginioDataStorage::setBackend(const QUrl &aBackendAddress, const QString &aBackendId)
{
    bool chgAddress, chgId;
    QTC_D(QEnginioDataStorage);

    chgAddress = (d->backendAddress() != aBackendAddress);
    chgId = (d->backendId() != aBackendId);

    if (!chgAddress && !chgId) {
        return;
    }

    d->unbindForwarding();
    d->setBackend(aBackendAddress, aBackendId);
    d->bindForwarding(this);

    if (chgAddress) {
        emit backendAddressChanged(aBackendAddress);
    }

    if (chgId) {
        emit backendIdChanged(aBackendId);
    }
}

QUrl QEnginioDataStorage::backendAddress() const
{
    QTC_D(const QEnginioDataStorage);
    return d->backendAddress();
}

void QEnginioDataStorage::setBackendAddress(const QUrl &aBackendAddress)
{
    QTC_D(QEnginioDataStorage);

    if (d->backendAddress() == aBackendAddress) {
        return;
    }

    d->unbindForwarding();
    d->setBackendAddress(aBackendAddress);
    d->bindForwarding(this);

    emit backendAddressChanged(aBackendAddress);
}
void QEnginioDataStorage::setBackendAddressString(const QString &aBackendAddress)
{
    setBackendAddress(QUrl(aBackendAddress));
}

QString QEnginioDataStorage::backendId() const
{
    QTC_D(const QEnginioDataStorage);
    return d->backendId();
}

void QEnginioDataStorage::setBackendId(const QString &aBackendId)
{
    QTC_D(QEnginioDataStorage);

    if (d->backendId() == aBackendId) {
        return;
    }

    d->unbindForwarding();
    d->setBackendId(aBackendId);
    d->bindForwarding(this);

    emit backendIdChanged(aBackendId);
}

QString QEnginioDataStorage::username() const
{
    QTC_D(const QEnginioDataStorage);
    return d->username();
}

QString QEnginioDataStorage::password() const
{
    QTC_D(const QEnginioDataStorage);
    return d->password();
}

void QEnginioDataStorage::setUsername(const QString &aUsername)
{
    QTC_D(QEnginioDataStorage);
    d->setUsername(aUsername);
}

void QEnginioDataStorage::setPassword(const QString &aPassword)
{
    QTC_D(QEnginioDataStorage);
    d->setPassword(aPassword);
}

QEnginioCollection QEnginioDataStorage::collection(const QString &aCollectionName)
{
    QTC_D(QEnginioDataStorage);

    QEnginioCollection collection;
    QSharedPointer<QEnginioCollectionObject> collectionObject;

    collectionObject = d->collection(aCollectionName);

    if (collectionObject) {
        QEnginioCollectionPrivate *col;
        col = reinterpret_cast<QEnginioCollectionPrivate *>(QTC_D_PTR(&collection));

        if (col) {
            col->setEnginioCollectionObject(collectionObject);
        }
    }

    return collection;
}

QEnginioConnection QEnginioDataStorage::reserveConnection()
{
    QTC_D(QEnginioDataStorage);

    QEnginioConnection connection;
    QSharedPointer<QEnginioConnectionObject> connectionObject;

    connectionObject = d->reserveConnection();

    if (connectionObject) {
        QEnginioConnectionPrivate *con;
        con = reinterpret_cast<QEnginioConnectionPrivate *>(QTC_D_PTR(&connection));

        if (con) {
            con->setEnginioConnectionObject(connectionObject);
        }
    }

    return connection;
}
void QEnginioDataStorage::releaseConnection(QEnginioConnection &aConnection)
{
    QTC_D(QEnginioDataStorage);

    QEnginioConnectionPrivate *con;
    con = reinterpret_cast<QEnginioConnectionPrivate *>(QTC_D_PTR(&aConnection));

    if (con) {
        d->releaseConnection(con->enginioConnectionObject());
        con->setEnginioConnectionObject(QSharedPointer<QEnginioConnectionObject>());
    }
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