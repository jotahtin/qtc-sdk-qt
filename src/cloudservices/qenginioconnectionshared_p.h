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

#ifndef QCLOUDSERVICES_QENGINIOCONNECTION_P_H
#define QCLOUDSERVICES_QENGINIOCONNECTION_P_H

#include <QtCore/qthreadstorage.h>
#include <QtCore/qpointer.h>
#include <QtCore/qurl.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qurlquery.h>
#include <QtCore/qfile.h>
#include <QtCore/qmimedatabase.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qbuffer.h>
#include <QtCore/qlinkedlist.h>
#include <QtCore/quuid.h>

#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qhttpmultipart.h>

#include <QtCloudServices/qenginioconnection.h>
#include <QtCloudServices/qenginiodatastorage.h>

#include <QtCloudServices/private/enginiofakereply_p.h>
#include <QtCloudServices/private/enginioobjectadaptor_p.h>

#include <QtCloudServices/private/qcloudservicesconstants_p.h>
#include <QtCloudServices/private/qcloudservicesobject_p.h>
#include <QtCloudServices/private/qenginiooperation_p.h>

QT_BEGIN_NAMESPACE

#define CHECK_AND_SET_URL_PATH_IMPL(Url, Object, Operation, Flags) \
    QString dataPropertyName; \
    {\
        QString _path; \
        QByteArray _errorMsg; \
        GetPathReturnValue _ret = getPath(Object, Operation, &_path, &_errorMsg, Flags); \
    if (!_ret.successful()) \
            return new EnginioFakeReply(this, _errorMsg); \
        dataPropertyName = _ret; \
        Url.setPath(_path); \
    }

#define CHECK_AND_SET_PATH(Url, Object, Operation) \
    CHECK_AND_SET_URL_PATH_IMPL(Url, Object, Operation, QEnginioConnectionPrivate::Default)

#define CHECK_AND_SET_PATH_WITH_ID(Url, Object, Operation) \
    CHECK_AND_SET_URL_PATH_IMPL(Url, Object, Operation, QEnginioConnectionPrivate::IncludeIdInPath)

/*
** QEnginioConnectionPrivate
*/
class QEnginioDataStorage;
class QEnginioConnectionPrivate : public QCloudServicesObjectPrivate {
    Q_OBJECT
    friend class QEnginioOperationPrivate;
public:
    class ReplyFinishedFunctor {
    public:
        ReplyFinishedFunctor(QEnginioConnection::dvar aConnection);
        void operator ()(QNetworkReply *aNetworkReply);
    private:
        QEnginioConnection::wvar iConnection;
    };



#if 0
    virtual void init();

    bool finishDelayedReplies();

    void setAuthenticationState(const QtCloudServices::AuthenticationState state)
    {
        QTC_Q(QEnginioConnection);

        if (_authenticationState == state) {
            return;
        }

        _authenticationState = state;
        emit q->authenticationStateChanged(state);
    }

    QtCloudServices::AuthenticationState authenticationState() const Q_REQUIRED_RESULT
    {
        return _authenticationState;
    }

    QJsonObject identityToken() const Q_REQUIRED_RESULT
    {
        return _identityToken;
    }



    EnginioIdentity *identity() const Q_REQUIRED_RESULT
    {
        return _identity;
    }

    void setIdentity(EnginioIdentity *identity)
    {
#if 0 /* IDENTITY MOVED TO EnginioDataStorage Object*/
        QTC_Q(QEnginioConnection);
        foreach(const QMetaObject::Connection & identityConnection, _identityConnections)
        QObject::disconnect(identityConnection);
        _identityConnections.clear();

        if (!identity) {
            // invalidate old identity token
            _identity->removeSessionToken(this);
            _identity = 0;
            return;
        }

        _identity = identity;
        CallPrepareSessionToken callPrepareSessionToken(this, identity);

        if (_backendId.isEmpty()) {
            _identityConnections.append(QObject::connect(q, &QEnginioConnection::backendIdChanged, callPrepareSessionToken));
        } else {
            identity->prepareSessionToken(this);
        }

        _identityConnections.append(QObject::connect(identity, &EnginioIdentity::dataChanged, callPrepareSessionToken));
        _identityConnections.append(QObject::connect(identity, &EnginioIdentity::aboutToDestroy, IdentityInstanceDestroyed(this)));
        emit q->identityChanged(identity);
#endif
    }

    template<class T>
    QNetworkReply *update(const ObjectAdaptor<T> &object, const QtCloudServices::Operation operation)
    {
        QUrl url(_serviceUrl);
        CHECK_AND_SET_PATH_WITH_ID(url, object, operation);

        QNetworkRequest req = prepareRequest(url);

        QByteArray data = dataPropertyName.isEmpty() ? object.toJson() : object[dataPropertyName].toJson();

        QNetworkReply *reply = networkManager()->put(req, data);

        if (gEnableEnginioDebugInfo) {
            _requestData.insert(reply, data);
        }

        return reply;
    }

    template<class T>
    QNetworkReply *remove(const ObjectAdaptor<T> &object, const QtCloudServices::Operation operation)
    {
        QUrl url(_serviceUrl);
        CHECK_AND_SET_PATH_WITH_ID(url, object, operation);

        QNetworkRequest req = prepareRequest(url);

        QNetworkReply *reply = 0;
        QByteArray data;
#if 1 // QT_VERSION < QT_VERSION_CHECK(5, 4, 0) ?

        if (operation != QtCloudServices::AccessControlOperation) {
            reply = networkManager()->deleteResource(req);
        } else {
            data = object[dataPropertyName].toJson();
            QBuffer *buffer = new QBuffer();
            buffer->setData(data);
            buffer->open(QIODevice::ReadOnly);
            reply = networkManager()->sendCustomRequest(req, QtCloudServicesConstants::Delete, buffer);
            buffer->setParent(reply);
        }

#else
        // TODO enable me https://codereview.qt-project.org/#change,56920
        data = dataPropertyName.isEmpty() ? object.toJson() : object[dataPropertyName].toJson();
        reply = networkManager()->deleteResource(req, data);
#endif

        Q_ASSERT(reply);

        if (gEnableEnginioDebugInfo && !data.isEmpty()) {
            _requestData.insert(reply, data);
        }

        return reply;
    }

    template<class T>
    QNetworkReply *create(const ObjectAdaptor<T> &object, const QtCloudServices::Operation operation)
    {
        QUrl url(_serviceUrl);

        CHECK_AND_SET_PATH(url, object, operation);

        QNetworkRequest req = prepareRequest(url);

        QByteArray data = dataPropertyName.isEmpty() ? object.toJson() : object[dataPropertyName].toJson();

        QNetworkReply *reply = networkManager()->post(req, data);

        if (gEnableEnginioDebugInfo) {
            _requestData.insert(reply, data);
        }

        return reply;
    }

    template<class T>
    QNetworkReply *query(const ObjectAdaptor<T> &object, const QtCloudServices::Operation operation)
    {
        QUrl url(_serviceUrl);
        CHECK_AND_SET_PATH(url, object, operation);

        // TODO add all params here
        QUrlQuery urlQuery;

        if (int limit = object[QtCloudServicesConstants::limit].toInt()) {
            urlQuery.addQueryItem(QtCloudServicesConstants::limit, QString::number(limit));
        }

        if (int offset = object[QtCloudServicesConstants::offset].toInt()) {
            urlQuery.addQueryItem(QtCloudServicesConstants::offset, QString::number(offset));
        }

        if (object.contains(QtCloudServicesConstants::count)) { // TODO docs are saying about integer but it is not interpreted.
            urlQuery.addQueryItem(QtCloudServicesConstants::count, QString(0, Qt::Uninitialized));
        }

        ValueAdaptor<T> include = object[QtCloudServicesConstants::include];

        if (include.isComposedType()) {
            urlQuery.addQueryItem(QtCloudServicesConstants::include,
                                  QString::fromUtf8(include.toJson()));
        }

        ValueAdaptor<T> sort = object[QtCloudServicesConstants::sort];

        if (sort.isComposedType()) {
            urlQuery.addQueryItem(QtCloudServicesConstants::sort,
                                  QString::fromUtf8(sort.toJson()));
        }

        if (operation == QtCloudServices::SearchOperation) {
            ValueAdaptor<T> search = object[QtCloudServicesConstants::search];
            ArrayAdaptor<T> objectTypes = object[QtCloudServicesConstants::objectTypes].toArray();

            if (Q_UNLIKELY(objectTypes.isEmpty())) {
                return new EnginioFakeReply(this, constructErrorMessage(QtCloudServicesConstants::Fulltext_Search_objectTypes_parameter_is_missing_or_it_is_not_an_array));
            }

            if (search.isComposedType()) {
                for (typename ArrayAdaptor<T>::const_iterator i = objectTypes.constBegin(); i != objectTypes.constEnd(); ++i) {
                    urlQuery.addQueryItem(QStringLiteral("objectTypes[]"), (*i).toString());
                }

                urlQuery.addQueryItem(QtCloudServicesConstants::search,
                                      QString::fromUtf8(search.toJson()));
            } else {
                return new EnginioFakeReply(this, constructErrorMessage(QtCloudServicesConstants::Fulltext_Search_search_parameter_missing));
            }
        } else if (object[QtCloudServicesConstants::query].isComposedType()) { // TODO docs are inconsistent on that
            urlQuery.addQueryItem(QStringLiteral("q"),
                                  QString::fromUtf8(object[QtCloudServicesConstants::query].toJson()));
        }

        url.setQuery(urlQuery);

        QNetworkRequest req = prepareRequest(url);

        return networkManager()->get(req);
    }

    template<class T>
    QNetworkReply *downloadUrl(const ObjectAdaptor<T> &object)
    {
        QUrl url(_serviceUrl);
        CHECK_AND_SET_PATH(url, object, QtCloudServices::FileGetDownloadUrlOperation);

        if (object.contains(QtCloudServicesConstants::variant)) {
            QString variant = object[QtCloudServicesConstants::variant].toString();
            QUrlQuery query;
            query.addQueryItem(QtCloudServicesConstants::variant, variant);
            url.setQuery(query);
        }

        QNetworkRequest req = prepareRequest(url);

        QNetworkReply *reply = networkManager()->get(req);
        return reply;
    }

    template<class T>
    QNetworkReply *uploadFile(const ObjectAdaptor<T> &object, const QUrl &fileUrl)
    {
        if (!fileUrl.scheme().isEmpty() && !fileUrl.isLocalFile()) {
            qWarning() << "Enginio: Upload must be local file.";
        }

        QString path = fileUrl.isLocalFile() ? fileUrl.toLocalFile() : fileUrl.path();

        QFile *file = new QFile(path);

        if (!file->exists()) {
            QByteArray msg = QByteArray("Cannot upload a not existing file ('") + path.toUtf8() + QByteArray("')");
            msg = constructErrorMessage(msg);
            delete file;
            return new EnginioFakeReply(this, msg);
        }

        if (!file->open(QFile::ReadOnly)) {
            QByteArray msg = QByteArray("File ('") + path.toUtf8() + QByteArray("') could not be opened for reading");
            msg = constructErrorMessage(msg);
            delete file;
            return new EnginioFakeReply(this, msg);
        }

        QMimeDatabase mimeDb;
        QString mimeType = mimeDb.mimeTypeForFile(path).name();
        return upload(object, file, mimeType);
    }

    template<class T>
    QNetworkReply *upload(const ObjectAdaptor<T> &object, QIODevice *device, const QString &mimeType)
    {
        QNetworkReply *reply = 0;

        if (!device->isSequential() && device->size() < _uploadChunkSize) {
            reply = uploadAsHttpMultiPart(object, device, mimeType);
        } else {
            reply = uploadChunked(object, device);
        }

        if (gEnableEnginioDebugInfo) {
            QByteArray data = object.toJson();
            _requestData.insert(reply, data);
        }

        return reply;
    }

    static QSharedPointer<QNetworkAccessManager> prepareNetworkManagerInThread() Q_REQUIRED_RESULT;

    class UploadProgressFunctor {
    public:
        UploadProgressFunctor(QEnginioConnectionPrivate *client, QNetworkReply *reply)
            : _client(client), _reply(reply)
        {
            Q_ASSERT(_client);
            Q_ASSERT(_reply);
        }

        void operator ()(qint64 progress, qint64 total)
        {
            if (!progress || !total) { // TODO sometimes we get garbage as progress, it seems like a bug of Qt or Enginio web engine
                return;
            }

            QEnginioOperation *ereply = _client->_replyReplyMap.value(_reply);

            if (_client->_chunkedUploads.contains(_reply)) {
                QPair<QIODevice*, qint64> chunkData = _client->_chunkedUploads.value(_reply);
                total = chunkData.first->size();
                progress += chunkData.second;

                if (progress > total) { // TODO assert?!
                    return;
                }
            }

            emit ereply->progress(progress, total);
        }
    private:
        QEnginioConnectionPrivate *_client;
        QNetworkReply *_reply;
    };

    virtual void emitSessionTerminated() const;
    virtual void emitSessionAuthenticated(QEnginioOperation *reply);
    virtual void emitSessionAuthenticationError(QEnginioOperation *reply);
    virtual void emitFinished(QEnginioOperation *reply);
    virtual void emitError(QEnginioOperation *reply);
    virtual QEnginioOperation *createReply(QNetworkReply *nreply);

private:
    template<class T>
    QNetworkReply *uploadAsHttpMultiPart(const ObjectAdaptor<T> &object, QIODevice *device, const QString &mimeType)
    {
        QUrl serviceUrl = _serviceUrl;
        CHECK_AND_SET_PATH(serviceUrl, QJsonObject(), QtCloudServices::FileOperation);

        QNetworkRequest req = prepareRequest(serviceUrl);
        req.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray());

        QHttpMultiPart *multiPart = createHttpMultiPart(object, device, mimeType);
        QNetworkReply *reply = networkManager()->post(req, multiPart);
        multiPart->setParent(reply);
        device->setParent(multiPart);
        _connections.append(QObject::connect(reply, &QNetworkReply::uploadProgress, UploadProgressFunctor(this, reply)));
        return reply;
    }


    /* Create a multi part upload:
    * That means the JSON metadata and the actual file get sent in one http-post.
    * The associatedObject has to be a valid object type on the server.
    * If it does not contain an id, it needs to be manually associated later or will get garbage collected eventually.
    */
    template<class T>
    QHttpMultiPart *createHttpMultiPart(const ObjectAdaptor<T> &object, QIODevice *data, const QString &mimeType)
    {
        // check file/chunk size
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        data->setParent(multiPart);

        QHttpPart objectPart;
        objectPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                             QStringLiteral("form-data; name=\"object\""));

        objectPart.setBody(object.toJson());
        multiPart->append(objectPart);

        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QStringLiteral("form-data; name=\"file\"; filename=\"%1\"").arg(object[QtCloudServicesConstants::file].toObject()[QtCloudServicesConstants::fileName].toString()));
        filePart.setBodyDevice(data);
        multiPart->append(filePart);
        return multiPart;
    }

    template<class T>
    QNetworkReply *uploadChunked(const ObjectAdaptor<T> &object, QIODevice *device)
    {
        QUrl serviceUrl = _serviceUrl;
        CHECK_AND_SET_PATH(serviceUrl, QJsonObject(), QtCloudServices::FileOperation);

        QNetworkRequest req = prepareRequest(serviceUrl);

        QNetworkReply *reply = networkManager()->post(req, object.toJson());
        _chunkedUploads.insert(reply, qMakePair(device, static_cast<qint64>(0)));
        _connections.append(QObject::connect(reply, &QNetworkReply::uploadProgress, UploadProgressFunctor(this, reply)));
        return reply;
    }

    void uploadChunk(QEnginioOperation *ereply, QIODevice *device, qint64 startPos);
#endif

#if 0
    enum PathOptions { Default, IncludeIdInPath = 1};

    struct GetPathReturnValue : public QPair<bool, QString> {
        GetPathReturnValue(bool value)
            : QPair<bool, QString>(value, QString())
        {
        }
        GetPathReturnValue(bool value, const QString &propertyName)
            : QPair<bool, QString>(value, propertyName)
        {
        }
        bool successful() const
        {
            return first;
        }
        operator QString() const
        {
            return second;
        }
    };

    template<class T>
    static GetPathReturnValue getPath(const T &object, int operation, QString *path, QByteArray *errorMsg, PathOptions flags = Default)
    {
        enum {Failed = false};
        QByteArray &msg = *errorMsg;

        QString &result = *path;
        result.reserve(96);
        result.append(QStringLiteral("/v1/"));

        switch (operation) {
        case QtCloudServices::ObjectOperation: {
            QString objectType = object[QtCloudServicesConstants::objectType].toString();

            if (objectType.isEmpty()) {
                msg = constructErrorMessage(QtCloudServicesConstants::Requested_object_operation_requires_non_empty_objectType_value);
                return GetPathReturnValue(Failed);
            }

            result.append(objectType.replace('.', '/'));
            break;
        }

        case QtCloudServices::AccessControlOperation: {
            QString objectType = object[QtCloudServicesConstants::objectType].toString();

            if (objectType.isEmpty()) {
                msg = constructErrorMessage(QtCloudServicesConstants::Requested_object_acl_operation_requires_non_empty_objectType_value);
                return GetPathReturnValue(Failed);
            }

            result.append(objectType.replace('.', '/'));
            QString id = object[QtCloudServicesConstants::id].toString();

            if (id.isEmpty()) {
                msg = constructErrorMessage(QtCloudServicesConstants::Requested_object_acl_operation_requires_non_empty_id_value);
                return GetPathReturnValue(Failed);
            }

            result.append('/');
            result.append(id);
            result.append('/');
            result.append(QtCloudServicesConstants::access);
            return GetPathReturnValue(true, QtCloudServicesConstants::access);
        }

        case QtCloudServices::FileOperation: {
            result.append(QtCloudServicesConstants::files);
            // if we have a fileID, it becomes "view", otherwise it is up/download
            QString fileId = object[QtCloudServicesConstants::id].toString();

            if (!fileId.isEmpty()) {
                result.append('/');
                result.append(fileId);
            }

            break;
        }

        case QtCloudServices::FileGetDownloadUrlOperation: {
            result.append(QtCloudServicesConstants::files);
            QString fileId = object[QtCloudServicesConstants::id].toString();

            if (fileId.isEmpty()) {
                msg = constructErrorMessage(QtCloudServicesConstants::Download_operation_requires_non_empty_fileId_value);
                return GetPathReturnValue(Failed);
            }

            result.append(QLatin1Char('/') + fileId + QStringLiteral("/download_url"));
            break;
        }

        case QtCloudServices::FileChunkUploadOperation: {
            const QString fileId = object[QtCloudServicesConstants::id].toString();
            Q_ASSERT(!fileId.isEmpty());
            result.append(QtCloudServicesConstants::files + QLatin1Char('/') + fileId + QStringLiteral("/chunk"));
            break;
        }

        case QtCloudServices::SearchOperation:
            result.append(QtCloudServicesConstants::search);
            break;

        case QtCloudServices::SessionOperation:
            result.append(QtCloudServicesConstants::session);
            break;

        case QtCloudServices::UserOperation:
            result.append(QtCloudServicesConstants::users);
            break;

        case QtCloudServices::UsergroupOperation:
            result.append(QtCloudServicesConstants::usergroups);
            break;

        case QtCloudServices::UsergroupMembersOperation: {
            QString id = object[QtCloudServicesConstants::id].toString();

            if (id.isEmpty()) {
                msg = constructErrorMessage(QtCloudServicesConstants::Requested_usergroup_member_operation_requires_non_empty_id_value);
                return GetPathReturnValue(Failed);
            }

            result.append(QtCloudServicesConstants::usergroups);
            result.append('/');
            result.append(id);
            result.append('/');
            result.append(QtCloudServicesConstants::members);
            return GetPathReturnValue(true, QtCloudServicesConstants::member);
        }
        }

        if (flags & IncludeIdInPath) {
            QString id = object[QtCloudServicesConstants::id].toString();

            if (id.isEmpty()) {
                msg = constructErrorMessage(QtCloudServicesConstants::Requested_operation_requires_non_empty_id_value);
                return GetPathReturnValue(Failed);
            }

            result.append('/');
            result.append(id);
        }

        return GetPathReturnValue(true, QString());
    }

    class CallPrepareSessionToken {
        QEnginioConnectionPrivate *_enginio;
        EnginioIdentity *_identity;

    public:
        CallPrepareSessionToken(QEnginioConnectionPrivate *enginio, EnginioIdentity *identity)
            : _enginio(enginio)
            , _identity(identity)
        {
        }
        void operator ()()
        {
#if 0 /* */

            if (!_enginio->_backendId.isEmpty()) {
                // TODO should we disconnect backendId change singals?
                _identity->prepareSessionToken(_enginio);
            }

#endif
        }
    };

    class IdentityInstanceDestroyed {
        QEnginioConnectionPrivate *_enginio;

    public:
        IdentityInstanceDestroyed(QEnginioConnectionPrivate *enginio)
            : _enginio(enginio)
        {
        }
        void operator ()()
        {
            _enginio->setIdentity(0);
        }
    };
#endif
    QTC_DECLARE_PUBLIC(QEnginioConnection)
protected:
#if 0
    class AuthenticationStateTrackerFunctor {
        QEnginioConnectionPrivate *_enginio;
        QtCloudServices::AuthenticationState _state;
    public:
        AuthenticationStateTrackerFunctor(QEnginioConnectionPrivate *enginio, QtCloudServices::AuthenticationState state = QtCloudServices::NotAuthenticated)
            : _enginio(enginio)
            , _state(state)
        {
        }

        void operator()() const
        {
            _enginio->setAuthenticationState(_state);
        }
    };
#endif
public:
    QEnginioConnectionPrivate();
    QEnginioConnectionPrivate(const QEnginioDataStorage &aEnginioDataStorage);
    ~QEnginioConnectionPrivate();

    bool isValid() const;

    QSharedPointer<QNetworkAccessManager> networkManager() const Q_REQUIRED_RESULT;

    QEnginioOperation customRequest(const QEnginioRequest &aRequest);

    void replyFinished(QNetworkReply *aNetworkReply);
protected:
    void registerReply(QNetworkReply *aNetworkReply, const QEnginioOperation &aOperation);
    void unregisterReply(QNetworkReply *aNetworkReply);
private:
    QNetworkRequest prepareRequest(const QString &aPath,
                                   const QUrlQuery &aQuery,
                                   const QJsonObject &aExtraHeaders);


#if 0

    static QByteArray constructErrorMessage(const QByteArray &msg);

#endif

private:
    Q_DISABLE_COPY(QEnginioConnectionPrivate)
private:
    QEnginioDataStorage iEnginioDataStorage;
    QSharedPointer<QNetworkAccessManager> iNetworkManager;
    QMetaObject::Connection iNetworkManagerConnection;

#if 0
    QByteArray _backendId;
    EnginioIdentity *_identity;

    QLinkedList<QMetaObject::Connection> _connections;
    QVarLengthArray<QMetaObject::Connection, 4> _identityConnections;
    QUrl _serviceUrl;
    QNetworkRequest _request;
#endif
    QMap<QNetworkReply*, QEnginioOperation> iReplyOperationMap;

#if 0
    QMap<QNetworkReply*, QByteArray> _requestData;

    // device and last position
    QMap<QNetworkReply*, QPair<QIODevice*, qint64> > _chunkedUploads;
    qint64 _uploadChunkSize;
    QJsonObject _identityToken;
    QtCloudServices::AuthenticationState _authenticationState;

    QSet<QEnginioOperation*> _delayedReplies; // Used only for testing
#endif
private:
    static QThreadStorage < QWeakPointer<QNetworkAccessManager> > gNetworkManager;

};

#undef CHECK_AND_SET_URL_PATH_IMPL
#undef CHECK_AND_SET_PATH_WITH_ID
#undef CHECK_AND_SET_PATH

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOCONNECTION_P_H */
