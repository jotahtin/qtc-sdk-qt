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

#ifndef QCLOUDSERVICES_QENGINIOCOLLECTION_SHARED_H
#define QCLOUDSERVICES_QENGINIOCOLLECTION_SHARED_H

#include <QtCloudServices/private/qenginioconnectionshared_p.h>
#include <QtCloudServices/private/qenginiooperationshared_p.h>

QT_BEGIN_NAMESPACE

class QEnginioObjectShared;
class QEnginioQueryShared;
class QTCLOUDSERVICES_EXPORT QEnginioCollectionShared : public QObject {
    Q_OBJECT
private:
    Q_DISABLE_COPY(QEnginioCollectionShared)
public:
    QEnginioCollectionShared(QSharedPointer<QEnginioDataStorageShared> aEnginioDataStorage,
                             QString aCollectionName);
public:
    bool isValid() const;
    QString collectionName() const;

    QSharedPointer<QEnginioOperationShared> find(QSharedPointer<QEnginioCollectionShared> aSelf,
                                                 QSharedPointer<QEnginioQueryShared> aQuery,
                                                 QEnginioOperationShared::Callback aCallback);
    QSharedPointer<QEnginioOperationShared> findOne(QSharedPointer<QEnginioCollectionShared> aSelf,
                                                    const QString &aObjectId,
                                                    QEnginioOperationShared::Callback aCallback);
    QSharedPointer<QEnginioOperationShared> insert(QSharedPointer<QEnginioCollectionShared> aSelf,
                                                   QSharedPointer<QEnginioObjectShared> aObject,
                                                   QEnginioOperationShared::Callback aCallback);
    QSharedPointer<QEnginioOperationShared> update(QSharedPointer<QEnginioCollectionShared> aSelf,
                                                   const QString &aObjectId,
                                                   const QJsonObject &aObject,
                                                   QEnginioOperationShared::Callback aCallback);
    QSharedPointer<QEnginioOperationShared> remove(QSharedPointer<QEnginioCollectionShared> aSelf,
                                                   const QString &aObjectId,
                                                   QEnginioOperationShared::Callback aCallback);

    QSharedPointer<QEnginioObjectShared> fromJsonObject(QSharedPointer<QEnginioCollectionShared> aSelf,
                                                        const QJsonObject &aJsonObject);

public:
    QString resolveBasePath();
    void handleCompletedOperation(QSharedPointer<QRestOperationObject> aOperation,
                                  QSharedPointer<QEnginioConnectionShared> aConnection,
                                  QEnginioOperationShared::Callback aCallback);
private:
    QSharedPointer<QEnginioDataStorageShared> iEnginioDataStorage;
    QString iCollectionName;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOCOLLECTION_SHARED_H */

