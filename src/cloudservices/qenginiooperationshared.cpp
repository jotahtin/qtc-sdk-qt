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

#include <QtCloudServices/private/qenginiooperationshared_p.h>
#include <QtCloudServices/private/qenginiorequestshared_p.h>
#include <QtCloudServices/private/qenginiocollectionshared_p.h>
#include <QtCloudServices/private/qenginioobjectshared_p.h>
#include <QtCloudServices/private/qenginioobjectobject_p.h>
#include <QtCloudServices/private/qcloudservicesconstants_p.h>

QT_BEGIN_NAMESPACE

QEnginioOperationShared::QEnginioOperationShared(QSharedPointer<QEnginioConnectionShared> aEnginioConnection,
                        QSharedPointer<QEnginioRequestShared> aRequest)
 : QRestOperationShared(aEnginioConnection,aRequest)
{

}

int QEnginioOperationShared::resultObjectCount() const
{
    return iResultObjects.size();
}

QEnginioObject QEnginioOperationShared::resultObject() const
{
    if (iResultObjects.size() == 1) {
        iResultObjects.first();
    }

    return QEnginioObject();
}

QList<QEnginioObject> QEnginioOperationShared::resultObjects() const
{
    return iResultObjects;
}

#ifndef QT_NO_DEBUG_STREAM
void QEnginioOperationShared::dumpDebugInfo(QDebug &d) const
{
    QRestOperationShared::dumpDebugInfo(d);
#if 0
    QByteArray json = _client->_requestData.value(_nreply);

    if (!json.isEmpty()) {
        if (request.url().toString(QUrl::None).endsWith(QString::fromUtf8("account/auth/identity"))) {
            qDebug() << "Request Data hidden because it contains password";
        } else {
            qDebug() << "Request Data:" << json;
        }
    }

    if (!pData().isEmpty()) {
        qDebug() << "Reply Data:" << pData();
    }

#endif
}
#endif

void QEnginioOperationShared::operationFinished(QSharedPointer<QRestOperationShared> aSelf)
{    
    QSharedPointer<QEnginioRequestShared> enginioRequest;
    QSharedPointer<QEnginioCollectionShared> enginioCollection;

    enginioRequest=qSharedPointerCast<QEnginioRequestShared>(request());
    enginioCollection = enginioRequest->enginioCollection();

    iResultObjects.clear();

    // TODO: cleanup
    if (iJsonObject.contains(QtCloudServicesConstants::results)) {
        QJsonArray jsonObjects;
        QJsonArray::const_iterator i;
        jsonObjects = iJsonObject.value(QtCloudServicesConstants::results).toArray();

        for (i = jsonObjects.begin(); i != jsonObjects.end(); ++i) {
            if (!(*i).isObject()) {
                continue;
            }

            if (!enginioCollection) {
                QJsonObject  jsonObject = (*i).toObject();
                qDebug() << "---- NEED RESOLVE (PER OBJECT)..." // TODO
                         << jsonObject.value(QtCloudServicesConstants::objectType);

                continue;
            }

            QSharedPointer<QEnginioObjectShared> obj;
            obj = enginioCollection->fromJsonObject(enginioCollection,(*i).toObject());

            QEnginioObject x;
            x.object()->d_func()->setSharedInstance(obj);

            iResultObjects.push_back(x);
        }
    } else if (!iJsonObject.isEmpty()) {
        if (!enginioCollection) {
            qDebug() << "---- NEED RESOLVE (PER OBJECT)..." // TODO
                     << iJsonObject.value(QtCloudServicesConstants::objectType);
        } else {
            //iResultObjects.push_back(enginioCollection.fromJsonObject(iJsonObject));

            QSharedPointer<QEnginioObjectShared> obj;
            obj = enginioCollection->fromJsonObject(enginioCollection,iJsonObject);

            QEnginioObject x;
            x.object()->d_func()->setSharedInstance(obj);

            iResultObjects.push_back(x);
        }
    }

    QRestOperationShared::operationFinished(aSelf);
}

QT_END_NAMESPACE

