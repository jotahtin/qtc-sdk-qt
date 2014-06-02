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

#ifndef QCLOUDSERVICES_QENGINIOMODEL_H
#define QCLOUDSERVICES_QENGINIOMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qscopedpointer.h>

#include <QtCloudServices/qtcloudservices_global.h>
#include <QtCloudServices/qenginiocollection.h>
#include <QtCloudServices/qenginiomodelnode.h>
#include <QtCloudServices/qenginioquery.h>

QT_BEGIN_NAMESPACE

class QEnginioModelPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioModel : public QAbstractItemModel {
public:
    Q_OBJECT
    // Q_ENUMS(QtCloudServices::Operation) // TODO remove me QTBUG-33577
    // Q_PROPERTY(QtCloudServices::Operation operation READ operation WRITE setOperation NOTIFY operationChanged)
    // Q_PROPERTY(QEnginioConnection *client READ client WRITE setClient NOTIFY clientChanged)
    // Q_PROPERTY(QJsonObject query READ query WRITE setQuery NOTIFY queryChanged)
    Q_DISABLE_COPY(QEnginioModel)
    Q_DECLARE_PRIVATE(QEnginioModel)
    friend class QEnginioModelNodePrivate;
public:
    explicit QEnginioModel(QObject *aParent = 0);
    ~QEnginioModel();

    //! QAbstractItemModel
    virtual Qt::ItemFlags flags(const QModelIndex &aIndex) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int aRow, int aColumn, const QModelIndex &aParent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &aIndex) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &) const Q_DECL_OVERRIDE;

    virtual QVariant data(const QModelIndex &aIndex, int aRole = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &aIndex, const QVariant &aValue, int aRole = Qt::EditRole) Q_DECL_OVERRIDE;

    virtual void fetchMore(const QModelIndex &aParent) Q_DECL_OVERRIDE;
    virtual bool canFetchMore(const QModelIndex &aParent) const Q_DECL_OVERRIDE;

    //! QEnginioModel
    virtual QVariant enginioData(const QEnginioObject &aEnginioObject,
                                 const QModelIndex &aIndex, int aRole = Qt::DisplayRole) const;
    virtual bool setEnginioData(QEnginioObject aEnginioObject,
                                const QModelIndex &aIndex, const QVariant &aValue, int aRole = Qt::EditRole);


#if 0
    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    void disableNotifications();
#endif
#if 0
    Q_INVOKABLE QEnginioOperation setData(int row, const QVariant &value, const QString &role);
    using EnginioBaseModel::setData;
#endif

    QEnginioCollection collection(const QModelIndex &aParent = QModelIndex()) const Q_REQUIRED_RESULT;
    void setCollection(const QEnginioCollection &aCollection,
                       const QModelIndex &aParent = QModelIndex());

    QEnginioQuery query(const QModelIndex &aParent = QModelIndex()) Q_REQUIRED_RESULT;
    void setQuery(const QEnginioQuery &aQuery,
                  const QModelIndex &aParent = QModelIndex());

    void refresh(const QModelIndex &aParent = QModelIndex());

#if 0
    QtCloudServices::Operation operation() const Q_REQUIRED_RESULT;
    void setOperation(QtCloudServices::Operation operation);
#endif
    Q_INVOKABLE QEnginioObject enginioObject(const QModelIndex &aIndex) const Q_REQUIRED_RESULT;

    Q_INVOKABLE QEnginioOperation append(const QEnginioObject &aObject,
                                         const QModelIndex &aParent = QModelIndex());
    Q_INVOKABLE QEnginioOperation remove(const QModelIndex &aIndex);

protected:
    virtual QEnginioModelNode* nodeForEnginioObject(const QEnginioObject &aEnginioObject);
protected:
    QEnginioModelNode *nodeAt(const QModelIndex &aIndex);
    QEnginioModelNode *getNode(const QModelIndex &aIndex) const;
Q_SIGNALS:
    void collectionChanged(const QEnginioCollection &aCollection);
    // void queryChanged(const QJsonObject &query);
    // void operationChanged(QtCloudServices::Operation operation);
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOMODEL_H */
