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

#include <QtCloudServices/qtcloudservices.h>

#include <QtCloudServices/private/qenginioconnectionshared_p.h>
#include <QtCloudServices/private/qenginiooperationshared_p.h>
#include <QtCloudServices/private/qenginiorequestshared_p.h>
#include <QtCloudServices/private/qenginiodatastorageshared_p.h>
#include <QtCloudServices/private/qcloudservicesconstants_p.h>

QT_BEGIN_NAMESPACE

QEnginioConnectionShared::QEnginioConnectionShared(QSharedPointer<QEnginioDataStorageShared> aEnginioDataStorageShared)
    : QRestConnectionShared(aEnginioDataStorageShared),
      iEnginioDataStorageShared(aEnginioDataStorageShared)
{

}

QEnginioConnectionShared::~QEnginioConnectionShared() {

}

QSharedPointer<QEnginioOperationShared>
QEnginioConnectionShared::customRequest(QSharedPointer<QEnginioConnectionShared> aSelf,
                                        QSharedPointer<QEnginioRequestShared> aRequest)
{
    return qSharedPointerCast<QEnginioOperationShared>(restRequest(aSelf,aRequest));
}



QSharedPointer<QRestOperationShared> QEnginioConnectionShared::buildOperationInstance
(QSharedPointer<QRestConnectionShared> aSelf,
 QSharedPointer<QRestRequestShared> aRequest)
{
    QSharedPointer<QEnginioConnectionShared> self;
    QSharedPointer<QEnginioRequestShared> request;

    self=qSharedPointerCast<QEnginioConnectionShared>(aSelf);
    request=qSharedPointerCast<QEnginioRequestShared>(aRequest);

    QSharedPointer<QRestOperationShared> shared(new QEnginioOperationShared(self,request));

    return shared;
}

bool QEnginioConnectionShared::prepareRequest(QNetworkRequest &aRequest,
                                              const QString &aPath,
                                              const QUrlQuery &aQuery,
                                              const QJsonObject &aExtraHeaders)
{
    if (!QRestConnectionShared::prepareRequest(aRequest,aPath,aQuery,aExtraHeaders))
        return false;

    aRequest.setRawHeader(QtCloudServicesConstants::Enginio_Backend_Id,
                          iEnginioDataStorageShared->backendId().toLatin1());

    if (!iEnginioDataStorageShared->secret().isEmpty()) {
        QByteArray bearer = QtCloudServicesConstants::Bearer;
        bearer += iEnginioDataStorageShared->secret().toUtf8();
        aRequest.setRawHeader(QtCloudServicesConstants::Authorization,bearer);
    }


    return true;
}

QT_END_NAMESPACE
