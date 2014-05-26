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

#include <QtCloudServices/qenginiomodel.h>
#include <QtCloudServices/QEnginioOperation.h>

#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qenginiomodel_p.h>

#include <QtCloudServices/private/enginiofakereply_p.h>
#include <QtCloudServices/private/enginiodummyreply_p.h>
#include <QtCloudServices/private/enginiobackendconnection_p.h>

#include <QtCore/qobject.h>
#include <QtCore/qvector.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>

QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/

QEnginioModelPrivate::QEnginioModelPrivate() //EnginioBaseModel *q_ptr)
    :
/*
: _enginio(0)
, _operation()
, q(q_ptr)
, _replyConnectionConntext(new QObject())
, _latestRequestedOffset(0)
*/
    iCanFetchMore(false)
/*
    , _rolesCounter(QtCloudServices::SyncedRole)
    */
{
}

QEnginioModelPrivate::~QEnginioModelPrivate()
{
    /*
    foreach (const QMetaObject::Connection & connection, _clientConnections)
    QObject::disconnect(connection);

    delete _replyConnectionConntext;
    */
}

int QEnginioModelPrivate::rowCount() const
{
    return iData.count();
}
QVariant QEnginioModelPrivate::data(unsigned row, int role) const
{
    if (role == QtCloudServices::SyncedRole) {
        Q_ASSERT(false);
        //return _attachedData.isSynced(row);
    }

    const QJsonObject object = iData.at(row).toObject();

    if (!object.isEmpty()) {
        const QString roleName = iRoles.value(role);

        if (!roleName.isEmpty()) {
            return object[roleName];
        } else if (role == Qt::DisplayRole) {
            return iData.at(row);
        }
    }

    return QVariant();
}

bool QEnginioModelPrivate::canFetchMore() const
{
    return iCanFetchMore;
}

void QEnginioModelPrivate::fetchMore(int row)
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



#if 0

const int EnginioBaseModelPrivate::IncrementalModelUpdate = -2;

/*!
  \class EnginioModel
  \since 5.3
  \inmodule enginio-qt
  \ingroup enginio-client
  \target EnginioModelCpp
  \brief EnginioModel represents data from Enginio as a \l QAbstractListModel.

  EnginioModel is a \l QAbstractListModel, together with a view it allows
  to show the result of a query in a convenient way. The model executes query, update
  and create operations asynchronously on an Enginio backend collection, which allows
  not only to read data from the cloud but also modify it.

  The simplest type of query is:
  \code
  { "objectType": "objects.fruits" }
  \endcode

  Assigning such a query to the model results in downloading of all objects from the "objects.fruits"
  collection. It is possible to \l{EnginioModel::append()}{append} new objects, to
  \l{EnginioModel::setData()}{modify} or to \l{EnginioModel::remove()}{remove} them.

  The query has to result in a list of objects, each object becomes an item in the model. Properties
  of the items are used as role names.
  There are a few predefined role names that will always be available (\l{QtCloudServices::Role}{Role}).

  The operations are executed asynchronously, which means that user interface is not
  blocked while the model is initialized and updated. Every modification is divided in
  two steps; request and confirmation. For example when \l{EnginioModel::append()}{append}
  is called EnginioModel returns immediately as if the operation had succeeded. In
  the background it waits for confirmation from the backend and only then the operation is
  really finished. It may happen that operation fails, for example
  because of insufficient access rights, in that case the operation will be reverted.

  There are two, ways of tracking if an item state is the same in the model and backend.
  Each item has a role that returns a boolean \l{QtCloudServices::SyncedRole}{SyncedRole}, role name "_synced" which
  indicates whether the item is successfully updated on the server.
  This role can for example meant to be used for a busy indicator while a property is being updated.
  Alternatively the status of each \l{QEnginioOperation}{QEnginioOperation} returned by EnginioModel can be tracked.
  The operation is confirmed when the reply is \l{QEnginioOperation::isFinished()}{finished} without \l{QEnginioOperation::isError()}{error}.

  When a reply is finished it is the user's responsibility to delete it, it can be done
  by connecting the \l{QEnginioOperation::finished()}{finished} signal to \l{QObject::deleteLater()}{deleteLater}.
  \code
  QObject::connect(reply, &QEnginioOperation::finished, reply, &QEnginioOperation::deleteLater);
  \endcode
  \note it is not safe to use the delete operator directly in \l{QEnginioOperation::finished()}{finished}.

  \note QEnginioConnection emits the finished and error signals for the model, not the model itself.

  The \l{EnginioModel::query}{query} can contain one or more options:
  The "sort" option, to get presorted data:
  \code
  {
    "objectType": "objects.fruits",
    "sort": [{"sortBy":"price", "direction": "asc"}]
  }
  \endcode
  The "query" option is used for filtering:
  \code
  {
    "objectType": "objects.fruits",
    "query": {"name": {"$in": ["apple", "orange", "kiwi"]}}
  }
  \endcode
  The "limit" option to limit the amount of results:
  \code
  {
    "objectType": "objects.fruits",
    "limit": 10
  }
  \endcode
  The "offset" option to skip some results from the beginning of a result set:
  \code
  {
    "objectType": "objects.fruits",
    "offset": 10
  }
  \endcode
  The options are valid only during the initial model population and
  are not enforced in anyway when updating or otherwise modifying the model data.
  \l QSortFilterProxyModel can be used to do more advanced sorting and filtering on the client side.

  EnginioModel can not detect when a property of a result is computed by the backend.
  For example the "include" option to \l{EnginioModel::query}{query} fills in the original creator of
  and object with the full object representing the "creator".
  \code
  {
    "objectType": "objects.fruits",
    "include": {"creator": {}}
  }
  \endcode
  For the model the "creator" property is not longer a reference (as it is on the backend), but a full object.
  But while the full object is accessible, attempts to alter the object's data will fail.
*/


void EnginioBaseModelPrivate::receivedNotification(QJsonObject data)
{
    const QJsonObject origin = data[QtCloudServicesConstants::origin].toObject();
    const QString requestId = origin[QtCloudServicesConstants::apiRequestId].toString();

    if (_attachedData.markRequestIdAsHandled(requestId)) {
        return;    // request was handled
    }

    QJsonObject object = data[QtCloudServicesConstants::data].toObject();
    QString event = data[QtCloudServicesConstants::event].toString();

    if (event == QtCloudServicesConstants::update) {
        receivedUpdateNotification(object);
    } else if (event == QtCloudServicesConstants::_delete) {
        receivedRemoveNotification(object);
    } else  if (event == QtCloudServicesConstants::create) {
        const int rowHint = _attachedData.rowFromRequestId(requestId);

        if (rowHint != NoHintRow) {
            receivedUpdateNotification(object, QString(), rowHint);
        } else {
            receivedCreateNotification(object);
        }
    }
}

void EnginioBaseModelPrivate::receivedRemoveNotification(const QJsonObject &object, int rowHint)
{
    int row = rowHint;

    if (rowHint == NoHintRow) {
        QString id = object[QtCloudServicesConstants::id].toString();

        if (Q_UNLIKELY(!_attachedData.contains(id))) {
            // removing not existing object
            return;
        }

        row = _attachedData.rowFromObjectId(id);
    }

    if (Q_UNLIKELY(row == DeletedRow)) {
        return;
    }

    q->beginRemoveRows(QModelIndex(), row, row);
    _data.removeAt(row);
    // we need to updates rows in _attachedData
    _attachedData.updateAllDataAfterRowRemoval(row);
    q->endRemoveRows();
}

void EnginioBaseModelPrivate::receivedUpdateNotification(const QJsonObject &object, const QString &idHint, int row)
{
    // update an existing object
    if (row == NoHintRow) {
        QString id = idHint.isEmpty() ? object[QtCloudServicesConstants::id].toString() : idHint;
        Q_ASSERT(_attachedData.contains(id));
        row = _attachedData.rowFromObjectId(id);
    }

    if (Q_UNLIKELY(row == DeletedRow)) {
        return;
    }

    // FIXME Sometimes it may happen that we get an update about an object that was created just after
    // the full query and before notifications are setup. For now we inore such situation in future
    // we should create a createNotification.
    if (Q_UNLIKELY(row < 0)) {
        return;
    }

    QJsonObject current = _data[row].toObject();
    QDateTime currentUpdateAt = QDateTime::fromString(current[QtCloudServicesConstants::updatedAt].toString(), Qt::ISODate);
    QDateTime newUpdateAt = QDateTime::fromString(object[QtCloudServicesConstants::updatedAt].toString(), Qt::ISODate);

    if (newUpdateAt < currentUpdateAt) {
        // we already have a newer version
        return;
    }

    if (_data[row].toObject()[QtCloudServicesConstants::id].toString().isEmpty()) {
        // Create and update may go through the same code path because
        // the model already have a dummy item. No id means that it
        // is a dummy item.
        const QString newId = object[QtCloudServicesConstants::id].toString();
        AttachedData newData(row, newId);
        _attachedData.insert(newData);
    }

    if (_data.count() == 1) {
        q->beginResetModel();
        _data.replace(row, object);
        syncRoles();
        q->endResetModel();
    } else {
        _data.replace(row, object);
        emit q->dataChanged(q->index(row), q->index(row));
    }
}

void EnginioBaseModelPrivate::fullQueryReset(const QJsonArray &data)
{
    delete _replyConnectionConntext;
    _replyConnectionConntext = new QObject();
    q->beginResetModel();
    _data = data;
    _attachedData.initFromArray(_data);
    syncRoles();
    _canFetchMore = _canFetchMore && _data.count() && (queryData(QtCloudServicesConstants::limit).toDouble() <= _data.count());
    q->endResetModel();
}

void EnginioBaseModelPrivate::receivedCreateNotification(const QJsonObject &object)
{
    // create a new object
    QString id = object[QtCloudServicesConstants::id].toString();
    Q_ASSERT(!_attachedData.contains(id));
    AttachedData data;
    data.row = _data.count();
    data.id = id;
    q->beginInsertRows(QModelIndex(), _data.count(), _data.count());
    _attachedData.insert(data);
    _data.append(object);
    q->endInsertRows();
}

void EnginioBaseModelPrivate::syncRoles()
{
    QJsonObject firstObject(_data.first().toObject());

    if (!_roles.count()) {
        _roles.reserve(firstObject.count());
        _roles[QtCloudServices::SyncedRole] = QtCloudServicesConstants::_synced; // TODO Use a proper name, can we make it an attached property in qml? Does it make sense to try?
        _roles[QtCloudServices::CreatedAtRole] = QtCloudServicesConstants::createdAt;
        _roles[QtCloudServices::UpdatedAtRole] = QtCloudServicesConstants::updatedAt;
        _roles[QtCloudServices::IdRole] = QtCloudServicesConstants::id;
        _roles[QtCloudServices::ObjectTypeRole] = QtCloudServicesConstants::objectType;
        _rolesCounter = QtCloudServices::CustomPropertyRole;
    }

    // check if someone does not use custom roles
    QHash<int, QByteArray> predefinedRoles = q->roleNames();
    foreach (int i, predefinedRoles.keys()) {
        if (i < QtCloudServices::CustomPropertyRole && i >= QtCloudServices::SyncedRole && predefinedRoles[i] != _roles[i].toUtf8()) {
            qWarning("Can not use custom role index lower then QtCloudServices::CustomPropertyRole, but '%i' was used for '%s'", i, predefinedRoles[i].constData());
            continue;
        }

        _roles[i] = QString::fromUtf8(predefinedRoles[i].constData());
    }

    // estimate additional dynamic roles:
    QSet<QString> definedRoles = _roles.values().toSet();
    QSet<int> definedRolesIndexes = predefinedRoles.keys().toSet();

    for (QJsonObject::const_iterator i = firstObject.constBegin(); i != firstObject.constEnd(); ++i) {
        const QString key = i.key();

        if (definedRoles.contains(key)) {
            // we skip predefined keys so we can keep constant id for them
            if (Q_UNLIKELY(key == QtCloudServicesConstants::_synced)) {
                qWarning("EnginioModel can not be used with objects having \"_synced\" property. The property will be overriden.");
            }
        } else {
            while (definedRolesIndexes.contains(_rolesCounter)) {
                ++_rolesCounter;
            }

            _roles[_rolesCounter++] = i.key();
        }
    }
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const EnginioModelPrivateAttachedData &a)
{
    dbg.nospace() << "EnginioModelPrivateAttachedData(ref:";
    dbg.nospace() << a.ref << ", row: " << a.row << ", synced: " << (a.ref == 0) << ", id: " << a.id;
    dbg.nospace() << ')';
    return dbg.space();
}
#endif

namespace  {

    struct Types {
        typedef QEnginioOperation Reply;
        typedef EnginioModel Public;
        typedef QEnginioConnection Client;
        typedef QEnginioConnectionPrivate ClientPrivate;
        typedef QJsonObject Data;
    };

} // namespace

class EnginioModelPrivate: public EnginioModelPrivateT<EnginioModelPrivate, Types> {
public:
    typedef EnginioModelPrivateT<EnginioModelPrivate, Types> Base;

    EnginioModelPrivate(EnginioBaseModel *pub)
        : Base(pub)
    {}

    virtual QJsonObject replyData(const QEnginioOperation aReply) const Q_DECL_OVERRIDE
    {
#if 0
        return static_cast<const QEnginioOperation*>(reply)->data();
#endif
        return QJsonObject();
    }

    virtual QJsonValue queryData(const QString &name) Q_DECL_OVERRIDE {
        return _query[name];
    }

    virtual QJsonObject queryAsJson() const Q_DECL_OVERRIDE
    {
        return _query;
    }
};


/*!
  \enum QtCloudServices::Role

  EnginioModel defines roles which represent data used by every object
  stored in the Enginio backend

  \value CreatedAtRole \c When an item was created
  \value UpdatedAtRole \c When an item was updated last time
  \value IdRole \c What is the id of an item
  \value ObjectTypeRole \c What is item type
  \value SyncedRole \c Mark if an item is in sync with the backend
  \value CustomPropertyRole \c The first role id that may be used for dynamically created roles.
  \omitvalue InvalidRole

  Additionally EnginioModel supports dynamic roles which are mapped
  directly from received data. EnginioModel is mapping an item's properties
  to role names.

  \note Some objects may not contain value for a static role, it may happen
  for example when an item is not in sync with the backend.

  \sa EnginioBaseModel::roleNames()
*/

/*!
  \property EnginioModel::client
  \brief The QEnginioConnection used by the model.

  \sa QEnginioConnection
*/
QEnginioConnection *EnginioModel::client() const
{
    QTC_D(const EnginioModel);
    return d->enginio();
}

void EnginioModel::setClient(const QEnginioConnection *client)
{
    QTC_D(EnginioModel);

    if (client == d->enginio()) {
        return;
    }

    d->setClient(client);
}

/*!
  \property EnginioModel::query

  \include model-query.qdocinc 0
  \l {QEnginioConnection::query()}
  \include model-query.qdocinc 1

  \sa QEnginioConnection::query()
*/
QJsonObject EnginioModel::query()
{
    QTC_D(EnginioModel);
    return d->query();
}

void EnginioModel::setQuery(const QJsonObject &query)
{
    QTC_D(EnginioModel);

    if (d->query() == query) {
        return;
    }

    return d->setQuery(query);
}

/*!
  \property EnginioModel::operation
  \brief The operation type of the query
  \sa QtCloudServices::Operation, query()
*/
QtCloudServices::Operation EnginioModel::operation() const
{
    QTC_D(const EnginioModel);
    return d->operation();
}

void EnginioModel::setOperation(QtCloudServices::Operation operation)
{
    QTC_D(EnginioModel);

    if (operation == d->operation()) {
        return;
    }

    d->setOperation(operation);
    emit operationChanged(operation);
}

/*!
  \include model-append.qdocinc
  \sa QEnginioConnection::create()
*/
QEnginioOperation *EnginioModel::append(const QJsonObject &object)
{
    QTC_D(EnginioModel);

    if (Q_UNLIKELY(!d->enginio())) {
        qWarning("EnginioModel::append(): Enginio client is not set");
        return 0;
    }

    return d->append(object);
}

/*!
  \include model-remove.qdocinc
  \sa QEnginioConnection::remove()
*/
QEnginioOperation *EnginioModel::remove(int row)
{
    QTC_D(EnginioModel);

    if (Q_UNLIKELY(!d->enginio())) {
        qWarning("EnginioModel::remove(): Enginio client is not set");
        return 0;
    }

#if 0

    if (unsigned(row) >= unsigned(d->rowCount())) {
        QEnginioConnectionPrivate *client = QEnginioConnectionPrivate::get(d->enginio());
        QNetworkReply *nreply = new EnginioFakeReply(client, QEnginioConnectionPrivate::constructErrorMessage(QtCloudServicesConstants::EnginioModel_remove_row_is_out_of_range));
        QEnginioOperation *ereply = new QEnginioOperation(client, nreply);
        return ereply;
    }

#endif
    return d->remove(row);
}

/*!
  Update a value on \a row of this model's local cache
  and send an update request to the Enginio backend.

  The \a role is the property of the object that will be updated to be the new \a value.

  \return reply from backend.
  \sa QEnginioConnection::update()
*/
QEnginioOperation *EnginioModel::setData(int row, const QVariant &value, const QString &role)
{
    QTC_D(EnginioModel);

    if (Q_UNLIKELY(!d->enginio())) {
        qWarning("EnginioModel::setData(): Enginio client is not set");
        return 0;
    }

#if 0

    if (unsigned(row) >= unsigned(d->rowCount())) {
        QEnginioConnectionPrivate *client = QEnginioConnectionPrivate::get(d->enginio());
        QNetworkReply *nreply = new EnginioFakeReply(client, QEnginioConnectionPrivate::constructErrorMessage(QtCloudServicesConstants::EnginioModel_setProperty_row_is_out_of_range));
        QEnginioOperation *ereply = new QEnginioOperation(client, nreply);
        return ereply;
    }

#endif
    return d->setValue(row, role, value);
}

/*!
    \overload
    Returns the mapping of the model's roles to names. Use this function to map
    the object property names to the role integers.

    EnginioModel uses heuristics to assign the properties of the objects in the \l query()
    to roles (greater than \l Qt::UserRole). Sometimes if the objects do not share
    the same structure, if for example a property is missing, it may happen that
    a role is missing. In such cases we recommend to overload this method to
    enforce existence of all required roles.

    \note when reimplementating this function, you need to call the base class implementation first and
    take the result into account as shown in the {todos-cpp}{Todos Example}
    \note custom role indexes have to not overlap with \l QtCloudServices::Role
*/
QHash<int, QByteArray> EnginioBaseModel::roleNames() const
{
    QTC_D(const EnginioBaseModel);
    return d->roleNames();
}

/*!
    \internal
    Allows to disable notifications for autotests.
*/
void EnginioBaseModel::disableNotifications()
{
    QTC_D(EnginioBaseModel);
    d->disableNotifications();
}

#endif

/*
** Public Implementation
*/


/*!
Constructs a new model with \a parent as QObject parent.
*/
QEnginioModel::QEnginioModel(QObject *aParent)
#if QTCLOUDSERVICES_USE_QOBJECT_PRIVATE
    : QAbstractListModel(*new QEnginioModelPrivate(this), aParent)
#else
    : QAbstractListModel(aParent), iPIMPL(new QEnginioModelPrivate())
#endif
{
#if !QTCLOUDSERVICES_USE_QOBJECT_PRIVATE
    iPIMPL->iInterface = this;
#endif
    QTC_D(QEnginioModel);
    // d->init();

    qRegisterMetaType<QtCloudServices::Role>();
}

/*!
Destroys the model.
*/
QEnginioModel::~QEnginioModel()
{
#if !QTCLOUDSERVICES_USE_QOBJECT_PRIVATE

    if (iPIMPL) {
        delete iPIMPL;
    }

#endif
}

Qt::ItemFlags QEnginioModel::flags(const QModelIndex &aIndex) const
{
    return QAbstractListModel::flags(aIndex) | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

/*!
\overload
Use this function to access the model data at \a index.
With the \l roleNames() function the mapping of JSON property names to data roles used as \a role is available.
The data returned will be JSON (for example a string for simple objects, or a JSON Object).
*/
QVariant QEnginioModel::data(const QModelIndex &index, int role) const
{
    QTC_D(const QEnginioModel);

    if (!index.isValid() || index.row() < 0 || index.row() >= d->rowCount()) {
        return QVariant();
    }

    return d->data(index.row(), role);
}

/*!
\overload
\internal
*/
int QEnginioModel::rowCount(const QModelIndex &parent) const
{
    QTC_D(const QEnginioModel);
    Q_UNUSED(parent);
    return d->rowCount();
}

/*!
\overload
\internal
*/
bool QEnginioModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QTC_D(QEnginioModel);

    if (unsigned(index.row()) >= unsigned(d->rowCount())) {
        return false;
    }

#if 0
    QEnginioOperation *reply = d->setData(index.row(), value, role);
    QObject::connect(reply, &QEnginioOperation::dataChanged, reply, &QEnginioOperation::deleteLater);
#endif
    return true;
}

/*!
\overload
\internal
*/
void QEnginioModel::fetchMore(const QModelIndex &parent)
{
    QTC_D(QEnginioModel);
    d->fetchMore(parent.row());
}

/*!
\overload
\internal
*/
bool QEnginioModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    QTC_D(const QEnginioModel);
    return d->canFetchMore();
}

void QEnginioModel::setCollection(const QEnginioCollection &aCollection,
                                  const QEnginioQuery &aQuery)
{

}


QT_END_NAMESPACE

