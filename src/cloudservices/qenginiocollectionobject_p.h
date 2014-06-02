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

#ifndef QCLOUDSERVICES_QENGINIOCOLLECTION_OBJECT_P_H
#define QCLOUDSERVICES_QENGINIOCOLLECTION_OBJECT_P_H

#include <QtCore/private/qobject_p.h>

#include <QtCloudServices/qenginiocollectionobject.h>

QT_BEGIN_NAMESPACE

class QEnginioQueryObject;
class QEnginioOperationObject;
class QEnginioOperationShared;
class QEnginioCollectionShared;
class QTCLOUDSERVICES_EXPORT QEnginioCollectionObjectPrivate : public QObjectPrivate {
    Q_DECLARE_PUBLIC(QEnginioCollectionObject)
private:
    Q_DISABLE_COPY(QEnginioCollectionObjectPrivate)
public:
    QEnginioCollectionObjectPrivate();

    bool isValid() const;
    QString collectionName() const;

    QEnginioOperationObject *find(const QEnginioQueryObject *aQuery,
                                  QEnginioOperationObject::Callback aCallback);
    QEnginioOperationObject *findOne(const QString &aObjectId,
                                     QEnginioOperationObject::Callback aCallback);
    QEnginioOperationObject *insert(const QEnginioObjectObject *aObject,
                                    QEnginioOperationObject::Callback aCallback);
    QEnginioOperationObject *update(const QString &aObjectId,
                                    const QJsonObject &aObject,
                                    QEnginioOperationObject::Callback aCallback);
    QEnginioOperationObject *remove(const QString &aObjectId,
                                    QEnginioOperationObject::Callback aCallback);

    QEnginioObjectObject *fromJsonObject(const QJsonObject &aJsonObject);

protected:
    virtual void init();
    virtual void deinit();
public:
    QSharedPointer<QEnginioCollectionShared> sharedInstance() const;
    void setSharedInstance(QSharedPointer<QEnginioCollectionShared> aShared);
public:
    static void handleCompletedOperation(QSharedPointer<QEnginioOperationShared> aOperation,
                                         QEnginioOperationObject::Callback aCallback);

private:
    QSharedPointer<QEnginioCollectionShared> iShared;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOCOLLECTION_OBJECT_P_H */

