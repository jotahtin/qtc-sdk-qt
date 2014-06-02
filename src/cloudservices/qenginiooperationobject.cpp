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

#include <QtCore/qstring.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsondocument.h>
#include <QtNetwork/qnetworkreply.h>

#include <QtCloudServices/private/qenginiooperationobject_p.h>
#include <QtCloudServices/private/qenginiooperationshared_p.h>
#include <QtCloudServices/private/qenginioconnectionobject_p.h>
#include <QtCloudServices/private/qenginiorequestobject_p.h>

QT_BEGIN_NAMESPACE

/*
** Private
*/

QEnginioOperationObjectPrivate::QEnginioOperationObjectPrivate() {

}

int QEnginioOperationObjectPrivate::resultObjectCount() const {
    QSharedPointer<const QEnginioOperationShared> shared;
    shared = qSharedPointerCast<const QEnginioOperationShared>(sharedInstance());
    return shared->resultObjectCount();
}
QEnginioObject QEnginioOperationObjectPrivate::resultObject() const {
    QSharedPointer<const QEnginioOperationShared> shared;
    shared = qSharedPointerCast<const QEnginioOperationShared>(sharedInstance());
    return shared->resultObject();
}
QList<QEnginioObject> QEnginioOperationObjectPrivate::resultObjects() const {
    QSharedPointer<const QEnginioOperationShared> shared;
    shared = qSharedPointerCast<const QEnginioOperationShared>(sharedInstance());
    return shared->resultObjects();
}

QRestConnectionObject *QEnginioOperationObjectPrivate::buildConnectionObject() const {
    return new QEnginioConnectionObject;
}
QRestRequestObject *QEnginioOperationObjectPrivate::buildRequestObject() const {
    return new QEnginioRequestObject;
}

/*
** Public
*/

QEnginioOperationObject::QEnginioOperationObject(QObject *aParent)
    : QRestOperationObject(*new QEnginioOperationObjectPrivate,aParent)
{
    qDebug() << "QEnginioOperationObject::Constructor::" << this;
}
QEnginioOperationObject::~QEnginioOperationObject() {
    qDebug() << "QEnginioOperationObject::Destructor::" << this;
}

int QEnginioOperationObject::resultObjectCount() const {
    Q_D(const QEnginioOperationObject);
    return d->resultObjectCount();
}
QEnginioObject QEnginioOperationObject::resultObject() const {
    Q_D(const QEnginioOperationObject);
    return d->resultObject();
}
QList<QEnginioObject> QEnginioOperationObject::resultObjects() const {
    Q_D(const QEnginioOperationObject);
    return d->resultObjects();
}

QT_END_NAMESPACE
