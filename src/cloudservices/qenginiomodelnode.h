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

#ifndef QCLOUDSERVICES_QENGINIOMODEL_NODE_H
#define QCLOUDSERVICES_QENGINIOMODEL_NODE_H

/*
#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qscopedpointer.h>
*/

#include <QtCloudServices/qtcloudservices_global.h>
#include <QtCloudServices/qenginiocollection.h>

QT_BEGIN_NAMESPACE

/*
** Enginio Model Node
*/
class QEnginioModel;
class QEnginioModelNodePrivate;
class QTCLOUDSERVICES_EXPORT QEnginioModelNode : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QEnginioModelNode)
    friend class QEnginioModel;
    friend class QEnginioModelPrivate;
private:
    Q_DISABLE_COPY(QEnginioModelNode)
public:
    explicit QEnginioModelNode(QObject *aParent=0);
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

    virtual Qt::ItemFlags flags() const;
    virtual int columnCount() const;
    virtual QVariant data(const QModelIndex &aIndex, int aRole) const;
    virtual bool setData(const QModelIndex &aIndex, const QVariant &aValue, int aRole);

    QEnginioCollection collection() const Q_REQUIRED_RESULT;
    void setCollection(const QEnginioCollection &aCollection);

    QEnginioQuery query() const Q_REQUIRED_RESULT;
    void setQuery(const QEnginioQuery &aQuery);

    void refresh();

    QEnginioObject enginioObject() const Q_REQUIRED_RESULT;

    QEnginioOperation appendEnginioObject(const QEnginioObject &aObject);
    QEnginioOperation removeEnginioObject(int aIndex);
protected Q_SLOTS:
    void refreshEnginoObjectDisplay();
Q_SIGNALS:
    void nodeModified();
public:
    // virtual bool setupContextMenu(QMenu &aMenu);
};

#if 0
class QTCLOUDSERVICES_EXPORT QEnginioModelNodeLabel : public QEnginioModelNode {
    Q_OBJECT
public:

};

class QTCLOUDSERVICES_EXPORT QEnginioModelNodeCollection : public QEnginioModelNode {
    Q_OBJECT
public:

};

class QTCLOUDSERVICES_EXPORT QEnginioModelNodeObject : public QEnginioModelNode {
    Q_OBJECT
public:

};
#endif

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOMODEL_NODE_H */


