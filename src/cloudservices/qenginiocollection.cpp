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

#include "stdafx.h"

#include <QtCloudServices/qenginiocollection.h>
#include <QtCloudServices/qenginiooperation.h>
#include <QtCloudServices/qenginioquery.h>
#include <QtCloudServices/private/qenginiocollectionobject_p.h>

QT_BEGIN_NAMESPACE

QEnginioCollection::QEnginioCollection(QEnginioCollectionObject *aEnginioCollectionObject)
    : iObject(aEnginioCollectionObject)
{
}

QEnginioCollection::QEnginioCollection()
    : iObject(new QEnginioCollectionObject)
{
}

QEnginioCollection::QEnginioCollection(const QEnginioCollection &aOther)
    : iObject(new QEnginioCollectionObject)
{
    iObject->setSharedInstanceFrom(aOther.object());
}

QEnginioCollection::~QEnginioCollection() {
    if (iObject) {
        delete iObject;
    }
}

QEnginioCollection & QEnginioCollection::operator=(const QEnginioCollection &aOther)
{
    iObject->setSharedInstanceFrom(aOther.object());
    return *this;
}

bool QEnginioCollection::operator!() const
{
    return !isValid();
}

bool QEnginioCollection::isValid() const
{
    if (!iObject) {
        return false;
    }

    return iObject->isValid();
}

QString QEnginioCollection::collectionName() const
{
    return iObject->collectionName();
}

QEnginioOperation QEnginioCollection::find(const QEnginioQuery &aQuery,
        QEnginioOperation::Callback aCallback)
{
    return QEnginioOperation(iObject->find(aQuery.object(),
                  [=](QEnginioOperationObject *aObject) {
                    aCallback(QEnginioOperation(aObject));
                    }));
}

QEnginioOperation QEnginioCollection::findOne(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    return QEnginioOperation(iObject->findOne(aObjectId,
                  [=](QEnginioOperationObject *aObject) {
                    if (aObject) aCallback(QEnginioOperation(aObject));
                    }));
}

QEnginioOperation QEnginioCollection::insert(const QEnginioObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    return QEnginioOperation(iObject->insert(aObject.object(),
                  [=](QEnginioOperationObject *aObject) {
                    if (aObject) aCallback(QEnginioOperation(aObject));
                    }));
}
QEnginioOperation QEnginioCollection::update(const QString &aObjectId,
        const QJsonObject &aObject,
        QEnginioOperation::Callback aCallback)
{
    return QEnginioOperation(iObject->update(aObjectId,aObject,
                  [=](QEnginioOperationObject *aObject) {
                    if (aObject) aCallback(QEnginioOperation(aObject));
                    }));
}

QEnginioOperation QEnginioCollection::remove(const QString &aObjectId,
        QEnginioOperation::Callback aCallback)
{
    return QEnginioOperation(iObject->remove(aObjectId,
                  [=](QEnginioOperationObject *aObject) {
                    if (aObject) aCallback(QEnginioOperation(aObject));
                    }));
}

QEnginioObject QEnginioCollection::fromJsonObject(const QJsonObject &aJsonObject)
{
    return QEnginioObject(iObject->fromJsonObject(aJsonObject));
}

const QEnginioCollectionObject* QEnginioCollection::object() const {
    return iObject;
}

QT_END_NAMESPACE
