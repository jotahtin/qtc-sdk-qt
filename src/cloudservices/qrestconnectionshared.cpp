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

#include <QtCloudServices/private/qrestconnectionshared_p.h>
#include <QtCloudServices/private/qrestendpointshared_p.h>
#include <QtCloudServices/private/qcloudservicesconstants_p.h>

QT_BEGIN_NAMESPACE

QRestConnectionShared::ReplyFinishedFunctor::ReplyFinishedFunctor(QSharedPointer<QRestConnectionShared> aConnection)
    : iConnection(aConnection)
{
}
void QRestConnectionShared::ReplyFinishedFunctor::operator ()(QNetworkReply *aNetworkReply)
{
    QSharedPointer<QRestConnectionShared> connection;

    if (connection = iConnection.toStrongRef()) {
        connection->replyFinished(aNetworkReply);
    }
}

QThreadStorage < QWeakPointer<QNetworkAccessManager> > QRestConnectionShared::gNetworkManager;

QRestConnectionShared::QRestConnectionShared(QSharedPointer<QRestEndpointShared> aRestEndpointShared)
    : iRestEndpoint(aRestEndpointShared)
{
}

void QRestConnectionShared::init(QSharedPointer<QRestConnectionShared> aSelf) {
    iNetworkManager = gNetworkManager.localData().toStrongRef();

    if (!iNetworkManager) {
        iNetworkManager = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager());
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0) && !defined(QT_NO_SSL) && !defined(ENGINIO_VALGRIND_DEBUG)
        iNetworkManager->connectToHostEncrypted(iRestEndpoint->endpointAddress().host());
#else
# error "SSL Connection required."
#endif
        gNetworkManager.setLocalData(iNetworkManager);
    }

    iNetworkManagerConnection = QObject::connect(iNetworkManager.data(),
                                                 &QNetworkAccessManager::finished,
                                                 ReplyFinishedFunctor(aSelf));
}

QRestConnectionShared::~QRestConnectionShared()
{
    if (iNetworkManager) {
        disconnect(iNetworkManagerConnection);
    }
}

bool QRestConnectionShared::isValid() const
{
    if (!iRestEndpoint) {
        return false;
    }

    return true;
}

QSharedPointer<QNetworkAccessManager> QRestConnectionShared::networkManager() const
{
    return iNetworkManager;
}

QSharedPointer<QRestOperationShared> QRestConnectionShared::restRequest(QSharedPointer<QRestConnectionShared> aSelf,
                                                                        QSharedPointer<QRestRequestShared> aRequest)
{
    QBuffer *buffer = 0;
    QNetworkRequest request;
    QSharedPointer<QRestOperationShared> operation;
    QByteArray verb;
    QNetworkReply *reply;

    switch (aRequest->operation()) {
    case QtCloudServices::RESTOperationGet:
        verb = QtCloudServicesConstants::RESTOperationGet;
        break;

    case QtCloudServices::RESTOperationPost:
        verb = QtCloudServicesConstants::RESTOperationPost;
        break;

    case QtCloudServices::RESTOperationPut:
        verb = QtCloudServicesConstants::RESTOperationPut;
        break;

    case QtCloudServices::RESTOperationDelete:
        verb = QtCloudServicesConstants::RESTOperationDelete;
        break;
    }

    if (!prepareRequest(request,
                        aRequest->path(),
                        aRequest->urlQuery(),
                        aRequest->extraHeaders()))
    {
        return operation;
    }

    operation = buildOperationInstance(aSelf,aRequest);

    QJsonObject jsonPayload = aRequest->payload();

    if (!jsonPayload.empty()) {
        QByteArray payload;
        payload = QJsonDocument(jsonPayload).toJson(QJsonDocument::Compact);
        buffer = new QBuffer();
        buffer->setData(payload);
        buffer->open(QIODevice::ReadOnly);
    }

    reply = networkManager()->sendCustomRequest(request, verb, buffer);
    operation->setNetworkReply(operation, reply);

    if (buffer) {
        buffer->setParent(reply);
    }

    return operation;
}

void QRestConnectionShared::replyFinished(QNetworkReply *aNetworkReply)
{    
    QSharedPointer<QRestOperationShared> operation;
    operation = iReplyOperationMap.take(aNetworkReply);

    if (!operation) {
        return;
    }

    operation->operationFinishedPrepare(operation);
}


void QRestConnectionShared::registerReply(QNetworkReply *aNetworkReply,
                                          QSharedPointer<QRestOperationShared> aOperation)
{
    aNetworkReply->setParent(aOperation.data());
    iReplyOperationMap[aNetworkReply] = aOperation;
}

void QRestConnectionShared::unregisterReply(QNetworkReply *aNetworkReply)
{
    iReplyOperationMap.remove(aNetworkReply);
}

QSharedPointer<QRestOperationShared> QRestConnectionShared::buildOperationInstance(QSharedPointer<QRestConnectionShared> aSelf,
                                                                                   QSharedPointer<QRestRequestShared> aRequest)
{
    return QSharedPointer<QRestOperationShared>(new QRestOperationShared(aSelf,aRequest));
}

bool QRestConnectionShared::prepareRequest(QNetworkRequest &aRequest,
                                           const QString &aPath,
                                           const QUrlQuery &aQuery,
                                           const QJsonObject &aExtraHeaders)
{
    QUrl url, relativeUrl;
    QByteArray requestId;

    if (!iRestEndpoint) {
        return false;
    }

    relativeUrl.setPath(aPath);

    url = iRestEndpoint->endpointAddress().resolved(relativeUrl);

    requestId = QUuid::createUuid().toByteArray();
    // Remove unneeded pretty-formatting.
    // before: "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"
    // after:  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    requestId.chop(1);      // }
    requestId.remove(0, 1); // {
    requestId.remove(23, 1);
    requestId.remove(18, 1);
    requestId.remove(13, 1);
    requestId.remove(8, 1);

    if (!aQuery.isEmpty()) {
        url.setQuery(aQuery);
    }

    aRequest.setUrl(url);

    aRequest.setRawHeader(QtCloudServicesConstants::X_Request_Id, requestId);
    aRequest.setRawHeader(QtCloudServicesConstants::Host, url.host().toLatin1());
    aRequest.setRawHeader(QtCloudServicesConstants::Accept_Encoding,
                          QtCloudServicesConstants::Accept_Encoding_Any);
    aRequest.setRawHeader(QtCloudServicesConstants::User_Agent,
                          QtCloudServicesConstants::User_Agent_Default);

    if (!aExtraHeaders.empty()) {
        QJsonObject::const_iterator end = aExtraHeaders.constEnd();

        for (QJsonObject::const_iterator i = aExtraHeaders.constBegin(); i != end; i++) {
            QByteArray headerName = i.key().toUtf8();
            QByteArray headerValue = i.value().toString().toUtf8();
            aRequest.setRawHeader(headerName, headerValue);
        }
    }

    return true;
}

QT_END_NAMESPACE
