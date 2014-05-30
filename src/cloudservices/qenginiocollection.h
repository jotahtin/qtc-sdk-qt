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

#ifndef QCLOUDSERVICES_QENGINIOCOLLECTION_H
#define QCLOUDSERVICES_QENGINIOCOLLECTION_H

#include <QObject>
#include <QSharedPointer>

#include <QtCloudServices/qcloudservicesobject.h>
#include <QtCloudServices/qenginiooperation.h>
#include <QtCloudServices/qenginioquery.h>

QT_BEGIN_NAMESPACE

class QEnginioDataStorage;
class QEnginioCollectionPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioCollection : public QCloudServicesObject {
    Q_OBJECT
    QTC_DECLARE_PRIVATE(QEnginioCollection)
    friend class QEnginioDataStoragePrivate;
protected:
    // Constructor for valid connection.
    QEnginioCollection(QEnginioCollection::dvar aPIMPL);
    QEnginioCollection(const QEnginioDataStorage &aEnginioDataStorage,
                       const QString &aCollectionName);
public:
    QEnginioCollection(QObject *aParent = 0);
    QEnginioCollection(const QEnginioCollection &aOther, QObject *aParent = 0);
    QEnginioCollection& operator=(const QEnginioCollection &aOther);
public:
    bool operator!() const;

    bool isValid() const;
    QString collectionName() const;

    Q_INVOKABLE QEnginioOperation find(const QEnginioQuery &aQuery,
                                       QEnginioOperation::Callback aCallback);
    Q_INVOKABLE QEnginioOperation findOne(const QString &aObjectId,
                                          QEnginioOperation::Callback aCallback);
    Q_INVOKABLE QEnginioOperation insert(const QEnginioObject &aObject,
                                         QEnginioOperation::Callback aCallback);
    Q_INVOKABLE QEnginioOperation update(const QString &aObjectId,
                                         const QJsonObject &aObject,
                                         QEnginioOperation::Callback aCallback);
    Q_INVOKABLE QEnginioOperation remove(const QString &aObjectId,
                                         QEnginioOperation::Callback aCallback);

    QEnginioObject fromJsonObject(const QJsonObject &aJsonObject);
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOCOLLECTION_H */

