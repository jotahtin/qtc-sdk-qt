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

#if 0
#ifndef ENGINIOMODELBASE_P_H
#define ENGINIOMODELBASE_P_H

#include <QtCore/qdatetime.h>
#include <QtCore/qdebug.h>
#include <QtCore/qhash.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qstring.h>
#include <QtCore/quuid.h>
#include <QtCore/qvector.h>

#include <QtCloudServices/enginiomodel.h>
#include <QtCloudServices/enginiobasemodel.h>

#include <QtCloudServices/private/qcloudservicesobject_p.h>
#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qenginiooperation_p.h>

#include <QtCloudServices/private/enginiofakereply_p.h>
#include <QtCloudServices/private/enginiodummyreply_p.h>
#include <QtCloudServices/private/enginiobackendconnection_p.h>
#include <QtCloudServices/private/enginiobasemodel_p.h>

// #include <QtCore/private/qabstractitemmodel_p.h>

QT_BEGIN_NAMESPACE

enum {
    DeletedRow = -3,
    NoHintRow = -4,
    InvalidRow = NoHintRow
};

struct EnginioModelPrivateAttachedData {
    uint ref;
    int row;
    QString id;
    QEnginioOperation createReply;
    EnginioModelPrivateAttachedData(int initRow = DeletedRow, const QString &initId = QString())
        : ref()
        , row(initRow)
        , id(initId)
        , createReply()
    {}
};
Q_DECLARE_TYPEINFO(EnginioModelPrivateAttachedData, Q_MOVABLE_TYPE);

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const EnginioModelPrivateAttachedData &a);
#endif

class AttachedDataContainer {
    typedef int Row;
    typedef int StorageIndex;
    typedef QString ObjectId;
    typedef QString RequestId;
    typedef EnginioModelPrivateAttachedData AttachedData;

    typedef QHash<Row, StorageIndex> RowIndex;
    RowIndex _rowIndex;

    typedef QHash<ObjectId, StorageIndex> ObjectIdIndex;
    ObjectIdIndex _objectIdIndex;

    typedef QHash<RequestId, QPair<int /*ref*/, StorageIndex> > RequestIdIndex;
    RequestIdIndex _requestIdIndex;

    typedef QHash<StorageIndex, AttachedData> Storage;
    QVector<AttachedData> _storage; // TODO replace by something smarter so we can use pointers instead of index.

    enum { InvalidStorageIndex = InvalidRow };

    StorageIndex append(const AttachedData &data)
    {
        _storage.append(data);
        StorageIndex idx = _storage.count() - 1;
        _rowIndex.insert(data.row, idx);
        _objectIdIndex.insert(data.id, idx);
        return idx;
    }

public:
    bool contains(const ObjectId &id) const
    {
        return _objectIdIndex.contains(id);
    }

    Row rowFromObjectId(const ObjectId &id) const
    {
        Q_ASSERT(contains(id));
        StorageIndex idx = _objectIdIndex.value(id, InvalidStorageIndex);
        return idx == InvalidStorageIndex ? InvalidRow : _storage[idx].row;
    }

    Row rowFromRequestId(const RequestId &id) const
    {
        StorageIndex idx = _requestIdIndex.value(id, qMakePair(0, static_cast<int>(InvalidStorageIndex))).second;
        return idx == InvalidStorageIndex ? InvalidRow : _storage[idx].row;
    }

    bool isSynced(Row row) const
    {
        return _storage[_rowIndex.value(row)].ref == 0;
    }

    void updateAllDataAfterRowRemoval(const int row)
    {
        _rowIndex.clear();
        _rowIndex.reserve(_storage.count());

        for (StorageIndex i = 0; i < _storage.count() ; ++i) {
            AttachedData &data = _storage[i];

            if (data.row > row) {
                --data.row;
            } else if (data.row == row) {
                data.row = DeletedRow;
            }

            _rowIndex.insert(data.row, i);
        }
    }

    AttachedData &ref(const ObjectId &id, Row row)
    {
        StorageIndex idx = _objectIdIndex.value(id, InvalidStorageIndex);

        if (idx == InvalidStorageIndex) {
            AttachedData data(row, id);
            idx = append(data);
        }

        AttachedData &data = _storage[idx];
        ++data.ref;
        Q_ASSERT(_storage[idx].ref == 1 || _storage[idx].row == row);
        data.row = row;
        return data;
    }

    AttachedData &ref(Row row)
    {
        StorageIndex idx = _rowIndex.value(row, InvalidStorageIndex);
        Q_ASSERT(idx != InvalidStorageIndex);
        AttachedData &data = _storage[idx];
        ++data.ref;
        return data;
    }

    AttachedData &deref(const ObjectId &id)
    {
        StorageIndex idx = _objectIdIndex.value(id, InvalidStorageIndex);
        Q_ASSERT(idx != InvalidStorageIndex);
        AttachedData &attachedData = _storage[idx];

        if (!--attachedData.ref && id[0] == 't') {
            // TODO it is last ref to a tmp id we should remove it
        }

        return attachedData;
    }

    void insert(const AttachedData &data)
    {
        _storage.append(data);
        StorageIndex idx = _storage.count() - 1;
        _rowIndex.insert(data.row, idx);
        _objectIdIndex.insert(data.id, idx);
    }

    void insertRequestId(const RequestId &id, Row row)
    {
        StorageIndex idx = _rowIndex.value(row, InvalidStorageIndex);
        Q_ASSERT(idx != InvalidStorageIndex);
        _requestIdIndex.insert(id, qMakePair(2, idx));
    }

    /*!
      \internal
      returns true if the request was already handled
    */
    bool markRequestIdAsHandled(const RequestId &id)
    {
        RequestIdIndex::iterator::reference value = _requestIdIndex[id];

        if (value.first) {
            if (--value.first <= 0) {
                _requestIdIndex.remove(id);
                return true;
            }
        } else {
            _requestIdIndex.remove(id);
        }

        return false;
    }

    void initFromArray(const QJsonArray &array)
    {
        const int count = array.count();
        _storage.clear();
        _rowIndex.clear();
        _objectIdIndex.clear();

        _storage.reserve(count);
        _rowIndex.reserve(count);
        _objectIdIndex.reserve(count);

        for (int row = 0; row < count; ++row) {
            QString id = array[row].toObject()[QtCloudServicesConstants::id].toString();
            Q_ASSERT(!id.isEmpty());
            AttachedData data(row, id);
            _storage.append(data);
            _rowIndex.insert(row, row);
            _objectIdIndex.insert(id, row);
        }
    }
};


class QTCLOUDSERVICES_EXPORT EnginioBaseModelPrivate
#if QTCLOUDSERVICES_USE_QOBJECT_PRIVATE
    : public QAbstractItemModelPrivate
#endif
{
protected:
    QEnginioConnectionPrivate *_enginio;
    QtCloudServices::Operation _operation;
    EnginioBaseModel *q;
    QVector<QMetaObject::Connection> _clientConnections;
    QObject *_replyConnectionConntext;

    const static int IncrementalModelUpdate;
    typedef EnginioModelPrivateAttachedData AttachedData;
    AttachedDataContainer _attachedData;
    int _latestRequestedOffset;
    bool _canFetchMore;

    unsigned _rolesCounter;
    QHash<int, QString> _roles;

    QJsonArray _data;

    class NotificationObject {
        // connection object it can be:
        // - null if not yet created
        // - -1 if notifications where disabled with EnginioModel::disableNotifications()
        // - valid pointer to connection object
        EnginioBackendConnection *_connection;

        struct NotificationReceived {
            EnginioBaseModelPrivate *model;

            void operator ()(QJsonObject data)
            {
                model->receivedNotification(data);
            }
        };
        void removeConnection()
        {
            if (*this) {
                _connection->close();
                delete _connection;
            }
        }

    public:
        NotificationObject()
            : _connection()
        {}

        ~NotificationObject()
        {
            removeConnection();
        }

        operator EnginioBackendConnection*()
        {
            return qintptr(_connection) != -1 ? _connection : 0;
        }

        void disable()
        {
            removeConnection();
            _connection = (EnginioBackendConnection*) - 1;
        }

        void connectToBackend(EnginioBaseModelPrivate *model, QEnginioConnectionPrivate *enginio, const QJsonObject &filter)
        {
#if 0

            if (qintptr(_connection) == -1) {
                return;
            }

            Q_ASSERT(model && enginio);

            if (enginio->_serviceUrl != QtCloudServicesConstants::stagingEnginIo) {
                return;    // TODO it allows to use notification only on staging
            }

            removeConnection(); // TODO reuse the connecton object
            _connection = new EnginioBackendConnection;
            NotificationReceived receiver = { model };
            QObject::connect(_connection, &EnginioBackendConnection::dataReceived, receiver);
            _connection->connectToBackend(enginio, filter);
#endif
        }
    } _notifications;

    struct FinishedRemoveRequest {
        EnginioBaseModelPrivate *model;
        const QString id;
        QEnginioOperation reply;
        void operator ()()
        {
            model->finishedRemoveRequest(reply, id);
        }
    };

    struct FinishedUpdateRequest {
        EnginioBaseModelPrivate *model;
        const QString id;
        const QJsonObject oldValue;
        QEnginioOperation reply;
        void operator ()()
        {
            model->finishedUpdateRequest(reply, id, oldValue);
        }
    };

    struct FinishedCreateRequest {
        EnginioBaseModelPrivate *model;
        const QString tmpId;
        QEnginioOperation reply;
        void operator ()()
        {
            model->finishedCreateRequest(reply, tmpId);
        }
    };

    struct FinishedFullQueryRequest {
        EnginioBaseModelPrivate *model;
        QEnginioOperation reply;
        void operator ()()
        {
            model->finishedFullQueryRequest(reply);
        }
    };

    struct FinishedIncrementalUpdateRequest {
        EnginioBaseModelPrivate *model;
        const QJsonObject query;
        QEnginioOperation reply;
        void operator ()()
        {
            model->finishedIncrementalUpdateRequest(reply, query);
        }
    };

    class QueryChanged {
        EnginioBaseModelPrivate *model;
    public:
        QueryChanged(EnginioBaseModelPrivate *m)
            : model(m)
        {
            Q_ASSERT(m);
        }

        void operator ()()
        {
            model->execute();
        }
    };

public:
    EnginioBaseModelPrivate(EnginioBaseModel *q_ptr)
        : _enginio(0)
        , _operation()
        , q(q_ptr)
        , _replyConnectionConntext(new QObject())
        , _latestRequestedOffset(0)
        , _canFetchMore(false)
        , _rolesCounter(QtCloudServices::SyncedRole)
    {
    }

    virtual ~EnginioBaseModelPrivate();

    void disableNotifications()
    {
        _notifications.disable();
    }

    void receivedNotification(QJsonObject data);
    void receivedRemoveNotification(const QJsonObject &object, int rowHint = NoHintRow);
    void receivedUpdateNotification(const QJsonObject &object, const QString &idHint = QString(), int row = NoHintRow);
    void receivedCreateNotification(const QJsonObject &object);

    QEnginioOperation append(const QJsonObject &value)
    {
#if 0
        QJsonObject object(value);
        QString temporaryId = QString::fromLatin1("tmp") + QUuid::createUuid().toString();
        object[QtCloudServicesConstants::objectType] = queryData(QtCloudServicesConstants::objectType); // TODO think about it, it means that not all queries are valid
        ObjectAdaptor<QJsonObject> aObject(object);
        QNetworkReply *nreply = _enginio->create(aObject, _operation);
        QEnginioOperation ereply = _enginio->createReply(nreply);
        FinishedCreateRequest finishedRequest = { this, temporaryId, ereply };
        QObject::connect(ereply, &QEnginioOperation::dataChanged, _replyConnectionConntext, finishedRequest);
        object[QtCloudServicesConstants::id] = temporaryId;
        const int row = _data.count();
        AttachedData data(row, temporaryId);
        data.ref = 1;
        data.createReply = ereply;

        if (!row) { // the first item need to update roles
            q->beginResetModel();
            _attachedData.insert(data);
            _data.append(value);
            syncRoles();
            q->endResetModel();
        } else {
            q->beginInsertRows(QModelIndex(), _data.count(), _data.count());
            _attachedData.insert(data);
            _data.append(value);
            q->endInsertRows();
        }

        _attachedData.insertRequestId(ereply->requestId(), row);
        return ereply;
#endif
        return NULL;
    }

    struct SwapNetworkReplyBase {
        QEnginioOperation _reply;
        EnginioBaseModelPrivate *_model;
        QJsonObject _object;
        QString _tmpId;
        QPointer<EnginioBaseModel> _modelGuard;

        void markAsError(QByteArray msg)
        {
#if 0
            EnginioFakeReply *nreply = new EnginioFakeReply(_reply, QEnginioConnectionPrivate::constructErrorMessage(msg));
            _reply->setNetworkReply(nreply);
#endif
        }

        QPair<QString, int> getAndSetCurrentIdRow(QEnginioOperation finishedCreateReply)
        {
            QString id = _model->replyData(finishedCreateReply)[QtCloudServicesConstants::id].toString();
            Q_ASSERT(!id.isEmpty());
            _object[QtCloudServicesConstants::id] = id;
            int row = InvalidRow;

            if (Q_LIKELY(_model->_attachedData.contains(_tmpId))) {
                row = _model->_attachedData.deref(_tmpId).row;
            } else if (Q_LIKELY(_model->_attachedData.contains(id))) {
                // model reset happend in a mean while
                row = _model->_attachedData.rowFromObjectId(id);
            } else {
                // the model was reset, probably with a different query, beacause
                // we have no sign of the id.
            }

            return qMakePair(id, row);
        }

        void swapNetworkReply(QEnginioOperation ereply)
        {
#if 0
            _reply->swapNetworkReply(ereply);
            ereply->deleteLater();
#endif
        }
    };

    struct SwapNetworkReplyForRemove {
        SwapNetworkReplyBase d;
        QEnginioOperation finishedCreateReply;
        void operator ()()
        {
#if 0

            if (finishedCreateReply->isError()) {
                d.markAsError(QtCloudServicesConstants::Dependent_create_query_failed_so_object_could_not_be_removed);
            } else if (Q_UNLIKELY(!d._modelGuard)) {
                d.markAsError(QtCloudServicesConstants::EnginioModel_was_removed_before_this_request_was_prepared);
            } else {
                QPair<QString, int> tmp = d.getAndSetCurrentIdRow(finishedCreateReply);
                const int row = tmp.second;

                if (Q_UNLIKELY(row == InvalidRow)) {
                    d.markAsError(QtCloudServicesConstants::EnginioModel_The_query_was_changed_before_the_request_could_be_sent);
                    return;
                }

                QString id = tmp.first;
                FinishedRemoveRequest finishedRequest = { d._model, id, d._reply };
                QObject::connect(d._reply, &QEnginioOperation::dataChanged, d._model->_replyConnectionConntext, finishedRequest);
                QEnginioOperation ereply = d._model->removeNow(row, d._object, id);
                d.swapNetworkReply(ereply);
            }

#endif
        }
    };

    QEnginioOperation remove(int row)
    {
        QJsonObject oldObject = _data.at(row).toObject();
        QString id = oldObject[QtCloudServicesConstants::id].toString();

        if (id.isEmpty()) {
            return removeDelayed(row, oldObject);
        }

        return removeNow(row, oldObject, id);
    }

    QEnginioOperation removeDelayed(int row, const QJsonObject &oldObject)
    {
#if 0
        // We are about to remove a not synced new item. The item do not have id yet,
        // so we can not make a request now, we need to wait for finished signal.
        QEnginioOperation ereply, createReply;
        QString tmpId;
        Q_ASSERT(oldObject[QtCloudServicesConstants::id].toString().isEmpty());
        delayedOperation(row, &ereply, &tmpId, &createReply);
        SwapNetworkReplyForRemove swapNetworkReply = {{ereply, this, oldObject, tmpId, q}, createReply};
        QObject::connect(createReply, &QEnginioOperation::dataChanged, swapNetworkReply);
        return ereply;
#endif
        return NULL;
    }

    QEnginioOperation removeNow(int row, const QJsonObject &oldObject, const QString &id)
    {
#if 0
        Q_ASSERT(!id.isEmpty());
        _attachedData.ref(id, row); // TODO if refcount is > 1 then do not emit dataChanged
        ObjectAdaptor<QJsonObject> aOldObject(oldObject);
        QNetworkReply *nreply = _enginio->remove(aOldObject, _operation);
        QEnginioOperation ereply = _enginio->createReply(nreply);
        FinishedRemoveRequest finishedRequest = { this, id, ereply };
        QObject::connect(ereply, &QEnginioOperation::dataChanged, _replyConnectionConntext, finishedRequest);
        _attachedData.insertRequestId(ereply->requestId(), row);
        QVector<int> roles(1);
        roles.append(QtCloudServices::SyncedRole);
        emit q->dataChanged(q->index(row), q->index(row) , roles);
        return ereply;
#endif
        return NULL;
    }

    QEnginioOperation setValue(int row, const QString &role, const QVariant &value)
    {
        int key = _roles.key(role, QtCloudServices::InvalidRole);
        return setData(row, value, key);
    }

    QtCloudServices::Operation operation() const Q_REQUIRED_RESULT
    {
        return _operation;
    }

    void setOperation(const int operation)
    {
        Q_ASSERT_X(operation >= QtCloudServices::ObjectOperation, "setOperation", "Invalid operation specified.");
        _operation = static_cast<QtCloudServices::Operation>(operation);
    }

    void execute()
    {
#if 0

        if (!_enginio || _enginio->_backendId.isEmpty()) {
            return;
        }

        if (!queryIsEmpty()) {
            // setup notifications
            QJsonObject filter;
            QJsonObject objectType;
            objectType.insert(QtCloudServicesConstants::objectType, queryData(QtCloudServicesConstants::objectType));
            filter.insert(QtCloudServicesConstants::data, objectType);
            _notifications.connectToBackend(this, _enginio, filter);

            // send full query
            QJsonObject query = queryAsJson();
            ObjectAdaptor<QJsonObject> aQuery(query);
            QNetworkReply *nreply = _enginio->query(aQuery, static_cast<QtCloudServices::Operation>(_operation));
            QEnginioOperation ereply = _enginio->createReply(nreply);

            if (_canFetchMore) {
                _latestRequestedOffset = query[QtCloudServicesConstants::limit].toDouble();
            }

            FinishedFullQueryRequest finshedRequest = { this, ereply };
            QObject::connect(ereply, &QEnginioOperation::dataChanged, _replyConnectionConntext, finshedRequest);
            QObject::connect(ereply, &QEnginioOperation::dataChanged, ereply, &QEnginioOperation::deleteLater);
        } else {
            fullQueryReset(QJsonArray());
        }

#endif
    }

    void finishedIncrementalUpdateRequest(const QEnginioOperation reply, const QJsonObject &query)
    {
#if 0
        Q_ASSERT(_canFetchMore);
        QJsonArray data(replyData(reply)[QtCloudServicesConstants::results].toArray());
        int offset = query[QtCloudServicesConstants::offset].toDouble();
        int limit = query[QtCloudServicesConstants::limit].toDouble();
        int dataCount = data.count();

        int startingOffset = qMax(offset, _data.count());

        q->beginInsertRows(QModelIndex(), startingOffset, startingOffset + dataCount - 1);

        for (int i = 0; i < dataCount; ++i) {
            _data.append(data[i]);
        }

        _canFetchMore = limit <= dataCount;
        q->endInsertRows();
#endif
    }

    void finishedFullQueryRequest(const QEnginioOperation reply)
    {
#if 0
        delete _replyConnectionConntext;
        _replyConnectionConntext = new QObject();
        fullQueryReset(replyData(reply)[QtCloudServicesConstants::results].toArray());
#endif
    }

    void fullQueryReset(const QJsonArray &data);

    void finishedCreateRequest(const QEnginioOperation reply, const QString &tmpId)
    {
#if 0

        if (_attachedData.markRequestIdAsHandled(reply->requestId())) {
            return;    // request was handled
        }

        int row;

        if (_attachedData.contains(tmpId))
            // this is a common path, we got result of our create request and we still have a dummy
            // item that we want to update.
        {
            row = _attachedData.deref(tmpId).row;
        } else {
            // the dummy object doesn't exist anymore, probably it was removed by a full reset
            // or by an initial query.
            QString id = replyData(reply)[QtCloudServicesConstants::id].toString();

            if (_attachedData.contains(id)) {
                // The reset removed the dummy value but it contained the newly created (initial reset
                // and append were reordered)
                row = _attachedData.rowFromObjectId(id);
            } else {
                // we created the item but there is no sign of it. We need to check if we have more or
                // less the same query, there is a chance that the value was lost in race between the
                // reset and create. This is possible scenario that:
                // send create -> send full query -> do query -> do create -> got query -> got create
                if (queryData(QtCloudServicesConstants::objectType) == replyData(reply)[QtCloudServicesConstants::objectType]) {
                    // the type is the same so we can re-add it
                    receivedCreateNotification(replyData(reply));
                }

                // query was changed too much we are done.
                return;
            }
        }

        if (reply->networkError() != QNetworkReply::NoError) {
            // We tried to create something and we failed, we need to remove tmp
            // item

            // TODO add a signal here so a developer can ask an user for a conflict
            // resolution.
            receivedRemoveNotification(_data[row].toObject(), row);
            return;
        }

        const QJsonObject object = replyData(reply);
        receivedUpdateNotification(object, tmpId, row);
#endif
    }

    void finishedRemoveRequest(const QEnginioOperation response, const QString &id)
    {
#if 0

        if (!_attachedData.contains(id)) {
            return;    // we do not know the object anymore, we are not interested in it's delete event
        }

        AttachedData &data = _attachedData.deref(id);

        if (_attachedData.markRequestIdAsHandled(response->requestId())) {
            return;    // request was handled
        }


        int row = data.row;

        if (row == DeletedRow || (response->networkError() != QNetworkReply::NoError && response->backendStatus() != 404)) {
            if (!data.ref) {
                // The item was not removed, because of an error. We assume that the
                // item is in sync
                emit q->dataChanged(q->index(row), q->index(row));
            }

            return;
        }

        receivedRemoveNotification(_data[row].toObject(), row);
#endif
    }

    void finishedUpdateRequest(const QEnginioOperation reply, const QString &id, const QJsonObject &oldValue)
    {
#if 0
        AttachedData &data = _attachedData.deref(id);

        if (_attachedData.markRequestIdAsHandled(reply->requestId())) {
            return;    // request was handled
        }

        int row = data.row;

        if (row == DeletedRow) {
            // We tried to update something that we already deleted
            // everything should be handled
            return;
        }

        if (reply->networkError() != QNetworkReply::NoError) {
            if (reply->backendStatus() == 404) {
                // We tried to update something that got deleted in between, probably on
                // the server side. Changing operation type to remove, so the cache
                // can be in sync with the server again.

                // TODO add a signal here so a developer can ask an user for a conflict
                // resolution.
                receivedRemoveNotification(_data[row].toObject(), row);
            } else {
                // Try to rollback the change.
                // TODO it is not perfect https://github.com/enginio/enginio-qt/issues/200
                _data.replace(row, oldValue);
                emit q->dataChanged(q->index(row), q->index(row));
            }

            return;
        }

        receivedUpdateNotification(replyData(reply), id, row);
#endif
    }

    struct SwapNetworkReplyForSetData {
        SwapNetworkReplyBase d;
        QVariant _value;
        int _role;
        QEnginioOperation finishedCreateReply;

        void operator ()()
        {
#if 0

            if (finishedCreateReply->isError()) {
                d.markAsError(QtCloudServicesConstants::Dependent_create_query_failed_so_object_could_not_be_updated);
            } else if (Q_UNLIKELY(!d._modelGuard)) {
                d.markAsError(QtCloudServicesConstants::EnginioModel_was_removed_before_this_request_was_prepared);
            } else {
                QPair<QString, int> tmp = d.getAndSetCurrentIdRow(finishedCreateReply);
                const int row = tmp.second;

                if (Q_UNLIKELY(row == InvalidRow)) {
                    d.markAsError(QtCloudServicesConstants::EnginioModel_The_query_was_changed_before_the_request_could_be_sent);
                    return;
                }

                QString id = tmp.first;
                FinishedUpdateRequest finished = { d._model, id, d._object, d._reply };
                QObject::connect(d._reply, &QEnginioOperation::dataChanged, d._model->_replyConnectionConntext, finished);
                QEnginioOperation ereply = d._model->setDataNow(row, _value, _role, d._object, id);
                d.swapNetworkReply(ereply);
            }

#endif
        }
    };

    QEnginioOperation setData(const int row, const QVariant &value, int role)
    {
#if 0

        if (role != QtCloudServices::InvalidRole) {
            QJsonObject oldObject = _data.at(row).toObject();
            QString id = oldObject[QtCloudServicesConstants::id].toString();

            if (id.isEmpty()) {
                return setDataDelyed(row, value, role, oldObject);
            }

            return setDataNow(row, value, role, oldObject, id);
        }

        QNetworkReply *nreply = new EnginioFakeReply(_enginio, QEnginioConnectionPrivate::constructErrorMessage(QtCloudServicesConstants::EnginioModel_Trying_to_update_an_object_with_unknown_role));
        QEnginioOperation ereply = _enginio->createReply(nreply);
        return ereply;
#endif
        return NULL;
    }

    void delayedOperation(int row, QEnginioOperation **newReply, QString *tmpId, QEnginioOperation **createReply)
    {
#if 0
        Q_ASSERT(!_attachedData.isSynced(row));
        AttachedData data = _attachedData.ref(row);
        *createReply = data.createReply;
        Q_ASSERT(*createReply);
        *tmpId = data.id;
        Q_ASSERT(tmpId->startsWith('t'));
        EnginioDummyReply *nreply = new EnginioDummyReply(*createReply);
        *newReply = _enginio->createReply(nreply);
#endif
    }

    QEnginioOperation setDataDelyed(int row, const QVariant &value, int role, const QJsonObject &oldObject)
    {
#if 0
        // We are about to update a not synced new item. The item do not have id yet,
        // so we can not make a request now, we need to wait for finished signal.
        Q_ASSERT(role > QtCloudServices::SyncedRole);
        QEnginioOperation *ereply, *createReply;
        QString tmpId;
        Q_ASSERT(oldObject[QtCloudServicesConstants::id].toString().isEmpty());
        delayedOperation(row, &ereply, &tmpId, &createReply);
        SwapNetworkReplyForSetData swapNetworkReply = {{ereply, this, oldObject, tmpId, q}, value, role, createReply};
        QObject::connect(createReply, &QEnginioOperation::dataChanged, swapNetworkReply);
        return ereply;
#endif
        return NULL;
    }

    QEnginioOperation setDataNow(const int row, const QVariant &value, int role, const QJsonObject &oldObject, const QString &id)
    {
#if 0
        Q_ASSERT(!id.isEmpty());
        const QString roleName(_roles.value(role));
        Q_ASSERT(!roleName.isEmpty());
        QJsonObject deltaObject;
        QJsonObject newObject = oldObject;
        deltaObject[roleName] = newObject[roleName] = QJsonValue::fromVariant(value);
        deltaObject[QtCloudServicesConstants::id] = id;
        deltaObject[QtCloudServicesConstants::objectType] = newObject[QtCloudServicesConstants::objectType];
        ObjectAdaptor<QJsonObject> aDeltaObject(deltaObject);
        QNetworkReply *nreply = _enginio->update(aDeltaObject, _operation);
        QEnginioOperation ereply = _enginio->createReply(nreply);
        FinishedUpdateRequest finished = { this, id, oldObject, ereply };
        QObject::connect(ereply, &QEnginioOperation::dataChanged, _replyConnectionConntext, finished);
        _attachedData.ref(id, row);
        _data.replace(row, newObject);
        _attachedData.insertRequestId(ereply->requestId(), row);
        emit q->dataChanged(q->index(row), q->index(row));
        return ereply;
#endif
        return NULL;
    }

    void syncRoles();

    QHash<int, QByteArray> roleNames() const Q_REQUIRED_RESULT
    {
        QHash<int, QByteArray> roles;
        roles.reserve(_roles.count());

        for (QHash<int, QString>::const_iterator i = _roles.constBegin();
                i != _roles.constEnd();
                ++i) {
            roles.insert(i.key(), i.value().toUtf8());
        }

        return roles;
    }

    int rowCount() const Q_REQUIRED_RESULT
    {
        return _data.count();
    }

    QVariant data(unsigned row, int role) const Q_REQUIRED_RESULT
    {
        if (role == QtCloudServices::SyncedRole) {
            return _attachedData.isSynced(row);
        }

        const QJsonObject object = _data.at(row).toObject();

        if (!object.isEmpty()) {
            const QString roleName = _roles.value(role);

            if (!roleName.isEmpty()) {
                return object[roleName];
            } else if (role == Qt::DisplayRole) {
                return _data.at(row);
            }
        }

        return QVariant();
    }

    bool canFetchMore() const Q_REQUIRED_RESULT
    {
        return _canFetchMore;
    }

    void fetchMore(int row)
    {
#if 0
        int currentOffset = _data.count();

        if (!_canFetchMore || currentOffset < _latestRequestedOffset) {
            return;    // we do not want to spam the server, lets wait for the last fetch
        }

        QJsonObject query(queryAsJson());

        int limit = query[QtCloudServicesConstants::limit].toDouble();
        limit = qMax(row - currentOffset, limit); // check if default limit is not too small

        query[QtCloudServicesConstants::offset] = currentOffset;
        query[QtCloudServicesConstants::limit] = limit;

        qDebug() << Q_FUNC_INFO << query;
        _latestRequestedOffset += limit;
        ObjectAdaptor<QJsonObject> aQuery(query);
        QNetworkReply *nreply = _enginio->query(aQuery, static_cast<QtCloudServices::Operation>(_operation));
        QEnginioOperation ereply = _enginio->createReply(nreply);
        QObject::connect(ereply, &QEnginioOperation::dataChanged, ereply, &QEnginioOperation::deleteLater);
        FinishedIncrementalUpdateRequest finishedRequest = { this, query, ereply };
        QObject::connect(ereply, &QEnginioOperation::dataChanged, _replyConnectionConntext, finishedRequest);
#endif
    }

    virtual QJsonObject replyData(const QEnginioOperation reply) const = 0;
    virtual QJsonValue queryData(const QString &name) = 0;
    virtual bool queryIsEmpty() const = 0;
    virtual QJsonObject queryAsJson() const = 0;

public:
#if !QTCLOUDSERVICES_USE_QOBJECT_PRIVATE
    EnginioBaseModel *iInterface;
#endif

};


template<typename Derived, typename Types>
struct EnginioModelPrivateT : public EnginioBaseModelPrivate {
    typedef EnginioBaseModelPrivate Base;
    typedef typename Types::Reply Reply;
    typedef typename Types::Public Public;
    typedef typename Types::Client Client;
    typedef typename Types::ClientPrivate ClientPrivate;
    typedef typename Types::Data Data;

    Data _query;

    inline Public *q() const
    {
        return static_cast<Public*>(Base::q);
    }

    class EnginioDestroyed {
        EnginioModelPrivateT *model;
    public:
        EnginioDestroyed(EnginioModelPrivateT *m)
            : model(m)
        {
            Q_ASSERT(m);
        }
        void operator ()()
        {
            model->setClient(0);
        }
    };

    EnginioModelPrivateT(EnginioBaseModel *pub)
        : Base(pub)
    {}

    void init()
    {
        QObject::connect(q(), &Public::queryChanged, QueryChanged(this));
        QObject::connect(q(), &Public::clientChanged, QueryChanged(this));
        QObject::connect(q(), &Public::operationChanged, QueryChanged(this));
    }

    Client *enginio() const Q_REQUIRED_RESULT
    {
#if 0
        return _enginio ? ClientPrivate::get(_enginio) : 0;
#endif
        return 0;
    }

    void setClient(const QEnginioConnection *enginio)
    {
#if 0

        if (_enginio) {
            foreach (const QMetaObject::Connection & connection, _clientConnections)
            QObject::disconnect(connection);
            _clientConnections.clear();
        }

        if (enginio) {
            _enginio = QEnginioConnectionPrivate::get(const_cast<QEnginioConnection*>(enginio));
            _clientConnections.append(QObject::connect(enginio, &QObject::destroyed, EnginioDestroyed(this)));

            //WILL NOT CHANGE
            //_clientConnections.append(QObject::connect(enginio, &QEnginioConnection::backendIdChanged, QueryChanged(this)));
        } else {
            _enginio = 0;
        }

        q()->clientChanged(static_cast<Client*>(const_cast<QEnginioConnection*>(enginio)));
#endif
    }

    Data query() Q_REQUIRED_RESULT {
        return _query;
    }

    void setQuery(const Data &query)
    {
        _query = query;

        // TODO Enable together with pageing support
//        if (_query.contains(QtCloudServicesConstants::pageSize)) {
//            const int pageSize = _query[QtCloudServicesConstants::pageSize].toDouble();
//            const QString limitString(QtCloudServicesConstants::limit);
//            const QString offsetString(QtCloudServicesConstants::offset);
//            const unsigned limit = _query[limitString].toDouble();
//            const unsigned offset = _query[offsetString].toDouble();
//            if (limit)
//                qWarning() << "EnginioModel::setQuery()" << "'limit' parameter can not be used together with model pagining feature, the value will be ignored";

//            if (offset) {
//                qWarning() << "EnginioModel::setQuery()" << "'offset' parameter can not be used together with model pagining feature, the value will be ignored";
//                _query.remove(offsetString);
//            }
//            _query[limitString] = pageSize;
//            _canFetchMore = true;
//        } else {
//            _canFetchMore = false;
//        }
        emit q()->queryChanged(query);
    }

    Reply *append(const QJsonObject &value)
    {
#if 0
        return static_cast<Reply*>(Base::append(value));
#endif
        return NULL;
    }
    Reply *remove(int row)
    {
#if 0
        return static_cast<Reply*>(Base::remove(row));
#endif
        return 0;
    }
    Reply *setValue(int row, const QString &role, const QVariant &value)
    {
#if 0
        return static_cast<Reply*>(Base::setValue(row, role, value));
#endif
        return 0;
    }

    bool queryIsEmpty() const Q_DECL_OVERRIDE
    {
        return ObjectAdaptor<Data>(_query, static_cast<ClientPrivate*>(_enginio)).isEmpty();
    }
};

QT_END_NAMESPACE
#endif // ENGINIOMODELBASE_P_H
#endif
