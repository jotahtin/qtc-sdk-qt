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

#ifndef QCLOUDSERVICES_QENGINIOOBJECT_P_H
#define QCLOUDSERVICES_QENGINIOOBJECT_P_H

#include <QtCore/qjsonobject.h>

#include "QtCloudServices/qenginioobject.h"
#include "QtCloudServices/qenginiouser.h"
#include "QtCloudServices/private/qcloudservicesobject_p.h"
#include "QtCloudServices/private/qenginiocollection_p.h"

QT_BEGIN_NAMESPACE

/*
** QEnginioObjectObject
*/
class QEnginioCollectionObject;
class QEnginioObjectObject : public QObject {
    Q_OBJECT
    friend class QEnginioCollectionObject;
public:
    QEnginioObjectObject(QSharedPointer<QEnginioCollectionObject> aCollection);
    ~QEnginioObjectObject();

    bool isValid() const Q_REQUIRED_RESULT;

    const QJsonObject jsonObject() const Q_REQUIRED_RESULT;

    const QString objectId() const Q_REQUIRED_RESULT;
    const QTime createAt() const Q_REQUIRED_RESULT;
    const QEnginioUser creator() const Q_REQUIRED_RESULT;
    const QString objectType() const Q_REQUIRED_RESULT;
    const QTime updatedAt() const Q_REQUIRED_RESULT;
    const QEnginioUser updater() const Q_REQUIRED_RESULT;
private:
    Q_DISABLE_COPY(QEnginioObjectObject)
public:
    static QSharedPointer<QEnginioObjectObject> get(QSharedPointer<QEnginioCollectionObject> aCollection);
private:
    QSharedPointer<QEnginioCollectionObject> iCollection;

    // Reference Object
    QJsonObject iJsonObject;

    // Common Fields
    QString iObjectId;
    QTime iCreatedAt;
    QEnginioUser iCreator;
    QString iObjectType;
    QTime iUpdatedAt;
    QEnginioUser iUpdater;

};

/*
** QEnginioObjectPrivate
*/
class QEnginioObjectPrivate : public QCloudServicesObjectPrivate {
    QTC_DECLARE_PUBLIC(QEnginioObject)
public:
    QEnginioObjectPrivate();

    bool isValid() const Q_REQUIRED_RESULT;

    const QJsonObject jsonObject() const Q_REQUIRED_RESULT;

    const QString objectId() const Q_REQUIRED_RESULT;
    const QTime createAt() const Q_REQUIRED_RESULT;
    const QEnginioUser creator() const Q_REQUIRED_RESULT;
    const QString objectType() const Q_REQUIRED_RESULT;
    const QTime updatedAt() const Q_REQUIRED_RESULT;
    const QEnginioUser updater() const Q_REQUIRED_RESULT;
public:
    QSharedPointer<QEnginioObjectObject> enginioObjectObject() const;
    void setEnginioObjectObject(QSharedPointer<QEnginioObjectObject> aObject);
private:
    QSharedPointer<QEnginioObjectObject> iObject;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOOBJECT_P_H */
