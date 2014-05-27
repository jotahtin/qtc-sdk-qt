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

#include <QtCore/qobject.h>
#include <QtCore/qvector.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>

#include <QtCloudServices/qenginiomodel.h>
#include <QtCloudServices/QEnginioOperation.h>

#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qenginiomodel_p.h>
#include <QtCloudServices/private/qenginiomodelnode_p.h>

QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/

QEnginioModelNodePrivate::QEnginioModelNodePrivate()
    : iModel(NULL), iParentNode(NULL),
      iCanFetchMore(false)
{
}

QEnginioModelNodePrivate::~QEnginioModelNodePrivate()
{
    qDeleteAll(iChildNodes);
}

QEnginioModelPrivate *QEnginioModelNodePrivate::model()
{
    return iModel;
}
const QEnginioModelPrivate *QEnginioModelNodePrivate::model() const
{
    return iModel;
}

QList<QEnginioModelNode *>& QEnginioModelNodePrivate::childNodes()
{
    return iChildNodes;
}

QEnginioModelNode *QEnginioModelNodePrivate::parentNode()
{
    return reinterpret_cast<QEnginioModelNode *>(QTC_Q_PTR(iParentNode));
}

void QEnginioModelNodePrivate::append(QEnginioModelNode *aNode)
{
    if (aNode == nullptr) {
        return;
    }

    reinterpret_cast<QEnginioModelNodePrivate *>(QTC_D_PTR(aNode))->setModel(model());

    reinterpret_cast<QEnginioModelNodePrivate *>(QTC_D_PTR(aNode))->setParentNode(this);

    int n = iChildNodes.count();

    QTC_Q_PTR(model())->beginInsertRows(index(), n, n);

    iChildNodes.append(aNode);

    QTC_Q_PTR(model())->endInsertRows();

    QTC_Q(QEnginioModelNode);

    q->nodeModified();
}

void QEnginioModelNodePrivate::insert(int aIndex, QEnginioModelNode *aNode)
{
    if (aNode == nullptr) {
        return;
    }

    reinterpret_cast<QEnginioModelNodePrivate *>(QTC_D_PTR(aNode))->setModel(model());

    reinterpret_cast<QEnginioModelNodePrivate *>(QTC_D_PTR(aNode))->setParentNode(this);

    QTC_Q_PTR(model())->beginInsertRows(index(), aIndex, aIndex);

    iChildNodes.insert(aIndex, aNode);

    QTC_Q_PTR(model())->endInsertRows();

    QTC_Q(QEnginioModelNode);

    q->nodeModified();
}

QEnginioModelNode *QEnginioModelNodePrivate::remove(int aIndex)
{
    QEnginioModelNode *node;

    if ((node = child(aIndex)) == nullptr) {
        return nullptr;
    }

    QTC_Q_PTR(model())->beginRemoveRows(index(), aIndex, aIndex);
    iChildNodes.removeAt(aIndex);
    QTC_Q_PTR(model())->endRemoveRows();

    QTC_Q(QEnginioModelNode);
    q->nodeModified();
    return node;
}
void QEnginioModelNodePrivate::removeAndDelete(int aIndex)
{
    QEnginioModelNode *node = remove(aIndex);

    if (node != nullptr) {
        node->deleteLater();
    }
}

QEnginioModelNode *QEnginioModelNodePrivate::child(int aIndex)
{
    if (aIndex >= iChildNodes.count()) {
        return nullptr;
    }

    return iChildNodes.value(aIndex);
}

int QEnginioModelNodePrivate::childCount() const
{
    return iChildNodes.count();
}

int QEnginioModelNodePrivate::childNumber() const
{
    QTC_Q(const QEnginioModelNode);

    if (iParentNode) {
        QEnginioModelNode *self = const_cast<QEnginioModelNode*>(q);
        return iParentNode->childNodes().indexOf(self);
    }

    return 0;
}
QModelIndex QEnginioModelNodePrivate::index()
{
    if (parentNode() != nullptr) {
        return QTC_Q_PTR(model())->createIndex(childNumber(), 0, this);
    }

    return QModelIndex();
}
QModelIndex QEnginioModelNodePrivate::parentIndex()
{
    if (parentNode() != nullptr) {
        return parentNode()->index();
    }

    return QModelIndex();
}

QEnginioCollection QEnginioModelNodePrivate::collection() const
{
    return iCollection;
}
void QEnginioModelNodePrivate::setCollection(const QEnginioCollection &aCollection)
{
    iCollection = aCollection;
}

QEnginioQuery QEnginioModelNodePrivate::query() const
{
    return iQuery; // return _query;
}
void QEnginioModelNodePrivate::setQuery(const QEnginioQuery &aQuery)
{
    iQuery = aQuery;

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
#if 0
    emit q()->queryChanged(query);
#endif
}

void QEnginioModelNodePrivate::refresh()
{
    qDebug() << "Refresh Node....";
}

/*
int QEnginioModelNodePrivate::rowCount() const
{
    return iData.count();
}
QVariant QEnginioModelNodePrivate::data(unsigned row, int role) const
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
*/

bool QEnginioModelNodePrivate::canFetchMore() const
{
    return iCanFetchMore;
}

void QEnginioModelNodePrivate::fetchMore(int row)
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

QEnginioOperation QEnginioModelNodePrivate::append(const QEnginioObject &aObject)
{
    QEnginioOperation op;

    if (!iCollection) {
        return op;
    }

    op = iCollection.insert(aObject,
    [ = ](QEnginioOperation & op) {
        this->handleOperationReply(HandleOperationInsert, op);
    });

#if 0
    QJsonObject object(value);
    QString temporaryId = QString::fromLatin1("tmp") + QUuid::createUuid().toString();
    object[QtCloudServicesConstants::objectType] = queryData(QtCloudServicesConstants::objectType);
    // TODO think about it, it means that not all queries are valid

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

    return op;
}

void QEnginioModelNodePrivate::handleOperationReply(HandleOperationType aType,
        QEnginioOperation aOperation)
{
    if (!aOperation) {
        // Signal error
        return;
    }

    qDebug() << "Got reply";
}


void QEnginioModelNodePrivate::setModel(QEnginioModelPrivate *aModel)
{
    iModel = aModel;

}
void QEnginioModelNodePrivate::setParentNode(QEnginioModelNodePrivate *aParentNode)
{
    iParentNode = aParentNode;
}

/*
** Public Implementation
*/

/*!
Constructs a new model with \a parent as QObject aParent.
*/
QEnginioModelNode::QEnginioModelNode()
    : QCloudServicesObject(*new QEnginioModelNodePrivate(), NULL)
{
}

/*!
Destroys the model.
*/
QEnginioModelNode::~QEnginioModelNode()
{
}

QEnginioModel *QEnginioModelNode::model()
{
    QTC_D(QEnginioModelNode);
    return QTC_Q_PTR(d->model());
}
const QEnginioModel *QEnginioModelNode::model() const
{
    QTC_D(const QEnginioModelNode);
    return QTC_Q_PTR(d->model());
}

QList<QEnginioModelNode *>& QEnginioModelNode::childNodes()
{
    QTC_D(QEnginioModelNode);
    return d->childNodes();
}
QEnginioModelNode *QEnginioModelNode::parentNode()
{
    QTC_D(QEnginioModelNode);
    return d->parentNode();
}

void QEnginioModelNode::append(QEnginioModelNode *aNode)
{
    QTC_D(QEnginioModelNode);
    d->append(aNode);
}
void QEnginioModelNode::insert(int aIndex, QEnginioModelNode *aNode)
{
    QTC_D(QEnginioModelNode);
    d->insert(aIndex, aNode);
}
QEnginioModelNode *QEnginioModelNode::remove(int aIndex)
{
    QTC_D(QEnginioModelNode);
    return d->remove(aIndex);
}
void QEnginioModelNode::removeAndDelete(int aIndex)
{
    QTC_D(QEnginioModelNode);
    d->removeAndDelete(aIndex);
}

QEnginioModelNode *QEnginioModelNode::child(int aIndex)
{
    QTC_D(QEnginioModelNode);
    return d->child(aIndex);
}
int QEnginioModelNode::childCount() const
{
    QTC_D(const QEnginioModelNode);
    return d->childCount();
}
int QEnginioModelNode::childNumber() const
{
    QTC_D(const QEnginioModelNode);
    return d->childNumber();
}
QModelIndex QEnginioModelNode::index()
{
    QTC_D(QEnginioModelNode);
    return d->index();
}
QModelIndex QEnginioModelNode::parentIndex()
{
    QTC_D(QEnginioModelNode);
    return d->parentIndex();
}



Qt::ItemFlags QEnginioModelNode::flags() const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

int QEnginioModelNode::columnCount() const
{
    return 1;
}

QVariant QEnginioModelNode::data(int /*aColumn*/, int /*aRole*/) const
{
    return QVariant();
}

bool QEnginioModelNode::setData(int /*aColumn*/,
                                const QVariant &/*aValue*/,
                                int /*aRole*/)
{
    return false;
}

QEnginioCollection QEnginioModelNode::collection() const
{
    QTC_D(const QEnginioModelNode);
    return d->collection();
}
void QEnginioModelNode::setCollection(const QEnginioCollection &aCollection)
{
    QTC_D(QEnginioModelNode);
    return d->setCollection(aCollection);
}

QEnginioQuery QEnginioModelNode::query() const
{
    QTC_D(const QEnginioModelNode);
    return d->query();
}

void QEnginioModelNode::setQuery(const QEnginioQuery &aQuery)
{
    QTC_D(QEnginioModelNode);
    return d->setQuery(aQuery);
}

void QEnginioModelNode::refresh()
{
    QTC_D(QEnginioModelNode);
    return d->refresh();
}


QT_END_NAMESPACE

