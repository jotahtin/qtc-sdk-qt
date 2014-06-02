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

#include <QtCloudServices/qenginiooperation.h>
#include <QtCloudServices/qenginiocollectionobject.h>

QT_BEGIN_NAMESPACE

class QEnginioQuery;
class QEnginioDataStorage;
class QEnginioCollectionObject;
class QTCLOUDSERVICES_EXPORT QEnginioCollection {
    friend class QEnginioDataStorage;
    friend class QEnginioRequest;
protected:
    // Constructor for valid connection.
    explicit QEnginioCollection(QEnginioCollectionObject *aEnginioCollectionObject);
public:
    QEnginioCollection();
    QEnginioCollection(const QEnginioCollection &aOther);
    virtual ~QEnginioCollection();

    // Assignment
    QEnginioCollection& operator=(const QEnginioCollection &aOther);

    // IsValid
    bool operator!() const;
    bool isValid() const;

    QString collectionName() const;

    QEnginioOperation find(const QEnginioQuery &aQuery,
                           QEnginioOperation::Callback aCallback);
    QEnginioOperation findOne(const QString &aObjectId,
                           QEnginioOperation::Callback aCallback);
    QEnginioOperation insert(const QEnginioObject &aObject,
                           QEnginioOperation::Callback aCallback);
    QEnginioOperation update(const QString &aObjectId,
                                         const QJsonObject &aObject,
                                         QEnginioOperation::Callback aCallback);
    QEnginioOperation remove(const QString &aObjectId,
                                         QEnginioOperation::Callback aCallback);

    QEnginioObject fromJsonObject(const QJsonObject &aJsonObject);
public:
    // Get implementation object
    const QEnginioCollectionObject* object() const;
private:
    QEnginioCollectionObject *iObject;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QEnginioCollection)

#endif /* QCLOUDSERVICES_QENGINIOCOLLECTION_H */

