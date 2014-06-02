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

#ifndef QCLOUDSERVICES_QENGINIOMODEL_NODE_P_H
#define QCLOUDSERVICES_QENGINIOMODEL_NODE_P_H

#include <QtCore/private/qobject_p.h>

#include <QtCloudServices/qenginiomodelnode.h>

/*
#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qenginiooperation_p.h>

#include <QtCloudServices/private/enginiofakereply_p.h>
#include <QtCloudServices/private/enginiodummyreply_p.h>
#include <QtCloudServices/private/enginiobackendconnection_p.h>
*/

QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/
class QEnginioModelPrivate;
class QEnginioModelNodePrivate : public QObjectPrivate {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QEnginioModelNode)
    friend class QEnginioModelPrivate;
public:
    enum HandleOperationType {
        HandleOperationRefresh,
        HandleOperationInsert,
        HandleOperationRemove
    };
public:
    QEnginioModelNodePrivate();
    ~QEnginioModelNodePrivate();
public:
    QEnginioModel *model();
    const QEnginioModel *model() const;

    QList<QEnginioModelNode *>& childNodes();
    QEnginioModelNode *parentNode();

    void append(QEnginioModelNode *aNode);
    void insert(int aIndex, QEnginioModelNode *aNode);
    QEnginioModelNode *remove(int aIndex);
    void removeAndDelete(int aIndex);

    QEnginioModelNode *child(int aIndex);
    int childCount() const;
    int childNumber() const;
    QModelIndex index();
    QModelIndex parentIndex();

    QEnginioCollection collection() const Q_REQUIRED_RESULT;
    void setCollection(const QEnginioCollection &aCollection);

    QEnginioQuery query() const Q_REQUIRED_RESULT;
    void setQuery(const QEnginioQuery &aQuery);

    void refresh();

    // --
    // int rowCount() const Q_REQUIRED_RESULT;
    // QVariant data(unsigned row, int role) const Q_REQUIRED_RESULT;

    bool canFetchMore() const Q_REQUIRED_RESULT;
    void fetchMore(int row);

    QEnginioObject enginioObject() const Q_REQUIRED_RESULT;

    QEnginioOperation appendEnginioObject(const QEnginioObject &aObject);
    QEnginioOperation removeEnginioObject(int aIndex);
public:
    void handleOperationReply(HandleOperationType aType,
                              QEnginioOperation aOperation,
                              const QString &aObjectId = QString());
private:
    void updateOrAppendObjectNode(const QEnginioObject &aObject);
    void removeObjectNode(const QString &aObjectId);
protected:
    void setModel(QEnginioModel *aModel);
    void setParentNode(QEnginioModelNode *aParentNode);
    void setEnginioObject(const QEnginioObject &aEnginoObject);
public:
    void refreshEnginoObjectDisplay();
private:
    QEnginioModel *iModel;
    QList<QEnginioModelNode *> iChildNodes;
    QEnginioModelNode *iParentNode;

    QEnginioObject iEnginoObject;
    QMetaObject::Connection iConnectionForObjectChange;

    QEnginioCollection iCollection; // QEnginioConnectionPrivate *_enginio;
    QEnginioQuery iQuery;

    bool iCanFetchMore;
    // QJsonArray iData;
    // QHash<int, QString> iRoles;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOMODEL_NODE_P_H */

