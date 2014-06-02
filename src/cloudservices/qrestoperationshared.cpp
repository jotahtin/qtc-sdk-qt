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

#include <QtCloudServices/private/qrestoperationshared_p.h>
#include <QtCloudServices/private/qrestconnectionshared_p.h>
#include <QtCloudServices/private/qcloudservicesconstants_p.h>

QT_BEGIN_NAMESPACE


/*
** QRestOperationShared
*/
QRestOperationShared::QRestOperationShared(QSharedPointer<QRestConnectionShared> aConnection,
                                QSharedPointer<QRestRequestShared> aRequest)
    : iConnection(aConnection),
      iRequest(aRequest),
      iNetworkReply(NULL)
{
}

QRestOperationShared::~QRestOperationShared()
{
    setNetworkReply(QSharedPointer<QRestOperationShared>(),NULL);
}

bool QRestOperationShared::isValid() const
{
    if (!iConnection || !iRequest) {
        return false;
    }

    return true;
}

bool QRestOperationShared::isError() const
{
    return errorCode() != QNetworkReply::NoError;
}
bool QRestOperationShared::isFinished() const
{
    return iNetworkReply->isFinished();
}

QSharedPointer<QRestConnectionShared> QRestOperationShared::connection() const {
    return iConnection;
}

QSharedPointer<QRestRequestShared> QRestOperationShared::request() const {
    return iRequest;
}

int QRestOperationShared::backendStatus() const
{
    return iNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
}

QString QRestOperationShared::requestId() const
{
    return QString::fromUtf8(iNetworkReply->request().rawHeader(QtCloudServicesConstants::X_Request_Id));
}

QtCloudServices::ErrorType QRestOperationShared::errorType() const
{
    if (errorCode() == QNetworkReply::NoError) {
        return QtCloudServices::NoError;
    }

    if (resultBytes().isEmpty()) {
        return QtCloudServices::NetworkError;
    }

    return QtCloudServices::BackendError;
}

QNetworkReply::NetworkError QRestOperationShared::errorCode() const
{
    return iNetworkReply->error();
}

QString QRestOperationShared::errorString() const
{
    if (errorType() == QtCloudServices::BackendError) {
        return QString::fromUtf8(resultBytes());
    }

    return iNetworkReply->errorString();
}

QJsonObject QRestOperationShared::resultJson() const
{
    return iJsonObject;
}

QByteArray QRestOperationShared::resultBytes() const
{
    if (iData.isEmpty() && iNetworkReply->isFinished()) {
        iData = iNetworkReply->readAll();
    }

    return iData;
}

#ifndef QT_NO_DEBUG_STREAM
void QRestOperationShared::dumpDebugInfo(QDebug &d) const
{
    static QHash<QNetworkAccessManager::Operation, QByteArray> operationNames;
    operationNames[QNetworkAccessManager::GetOperation] = "GET";
    operationNames[QNetworkAccessManager::PutOperation] = "PUT";
    operationNames[QNetworkAccessManager::PostOperation] = "POST";
    operationNames[QNetworkAccessManager::DeleteOperation] = "DELETE";
    operationNames[QNetworkAccessManager::CustomOperation] = "CUSTOM";

    QNetworkRequest request = iNetworkReply->request();
    d << "NetworkReply:" << iNetworkReply;
    d << "  Request URL:" << request.url().toString(/*FormattingOptions*/ QUrl::None);
    d << "  Operation:" << operationNames[iNetworkReply->operation()];
    d << "  HTTP return code:" << backendStatus();
    d << "  Headers[Content-Type]:" << request.header(QNetworkRequest::ContentTypeHeader);
    d << "  Raw headers:" << request.rawHeaderList();
    d << "  RawHeaders[Accept]:" << request.rawHeader(QtCloudServicesConstants::Accept);
    d << "  RawHeaders[Authorization]:" << request.rawHeader(QtCloudServicesConstants::Authorization);
    d << "  RawHeaders[Content-Type]:" << request.rawHeader(QtCloudServicesConstants::Content_Type);
    d << "  RawHeaders[X_Request_Id]:" << request.rawHeader(QtCloudServicesConstants::X_Request_Id);

    if (!iData.isEmpty()) {
        qDebug() << "Reply Data:" << iData;
    }
}
#endif

void QRestOperationShared::setNetworkReply(QSharedPointer<QRestOperationShared> aSelf,
                                           QNetworkReply *aNetworkReply)
{
    if (iNetworkReply != NULL) {
        iConnection->unregisterReply(iNetworkReply);

        if (iNetworkReply->isFinished()) {
            iNetworkReply->deleteLater();
        } else {
            iNetworkReply->setParent(iNetworkReply->manager());
            QObject::connect(iNetworkReply, &QNetworkReply::finished,
                             iNetworkReply, &QNetworkReply::deleteLater);
            iNetworkReply->abort();
        }
    }

    iNetworkReply = aNetworkReply;
    iData = QByteArray();

    if (!iNetworkReply) {
        return;
    }

    iConnection->registerReply(aNetworkReply, aSelf);
}


void QRestOperationShared::operationFinishedPrepare(QSharedPointer<QRestOperationShared> aSelf) {
    iJsonObject = QJsonDocument::fromJson(resultBytes()).object();

    operationFinished(aSelf);
}

void QRestOperationShared::operationFinished(QSharedPointer<QRestOperationShared> aSelf)
{

    iRequest->operationFinished(aSelf);
    emit finished();
}

QT_END_NAMESPACE
