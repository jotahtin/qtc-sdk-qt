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

#include <QtCloudServices/private/qenginiooperation_p.h>
#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qenginiorequest_p.h>
#include <QtCloudServices/private/enginioobjectadaptor_p.h>

QT_BEGIN_NAMESPACE

/*!
  \class QEnginioOperation
  \since 5.3
  \brief The QEnginioOperation class contains the data from a request to the Enginio database.
  \inmodule enginio-qt
  \ingroup enginio-client

  The reply, when finished, contains information received from the server:
  \list
  \li Data - object, which is a result from an earlier request,
    see the \l {QEnginioOperation::data}{data} function
  \li Network status - in case of a network problem, additional information can
  be accessed through: errorType, errorString, networkError
  \li Backend status - a finished request is always associated with a backend status
  code, which is just an HTTP code, and it can be queried through backendStatus
  \endlist

  The finished signal is emitted when the query is done.

  \sa QEnginioConnection
*/

/*!
  \class QEnginioOperation
  \since 5.3
  \internal
*/

/*!
  \enum QtCloudServices::ErrorType
  Describes the type of error that occured when making a request to the Enginio backend.
  \value NoError The reply returned without errors
  \value NetworkError The error was a networking problem
  \value BackendError The backend did not accept the query
*/

/*!
  \fn QEnginioOperation::finished(QEnginioOperation *reply)
  This signal is emitted when the QEnginioOperation \a reply is finished.
  After the network operation, use the \l{QEnginioOperation::isError()}{isError()} function to check for
  potential problems and then use the \l data property to access the returned data.
*/

/*!
  \fn QEnginioOperation::progress(qint64 bytesSent, qint64 bytesTotal)
  This signal is emitted for file operations, indicating the progress of up or downloads.
  The \a bytesSent is the current progress relative to the total \a bytesTotal.
*/

/*
** QEnginioOperationObject
*/
QEnginioOperationObject::QEnginioOperationObject(QSharedPointer<QEnginioConnectionObject> aConnection)
    : iConnection(aConnection),
      iNetworkReply(NULL),
      iDelay(false)
{
}

QEnginioOperationObject::~QEnginioOperationObject()
{
    setNetworkReply(QSharedPointer<QEnginioOperationObject>(), NULL);
}

bool QEnginioOperationObject::isError() const
{
    return errorCode() != QNetworkReply::NoError;
}
bool QEnginioOperationObject::isFinished() const
{
    return iNetworkReply->isFinished() && Q_LIKELY(!iDelay);
}
QNetworkReply::NetworkError QEnginioOperationObject::errorCode() const
{
    return iNetworkReply->error();
}

int QEnginioOperationObject::backendStatus() const
{
    return iNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
}

QString QEnginioOperationObject::requestId() const
{
    return QString::fromUtf8(iNetworkReply->request().rawHeader(QtCloudServicesConstants::X_Request_Id));
}

QString QEnginioOperationObject::errorString() const
{
    if (errorType() == QtCloudServices::BackendError) {
        return QString::fromUtf8(pData());
    }

    return iNetworkReply->errorString();
}

QtCloudServices::ErrorType QEnginioOperationObject::errorType() const
{
    if (errorCode() == QNetworkReply::NoError) {
        return QtCloudServices::NoError;
    }

    if (pData().isEmpty()) {
        return QtCloudServices::NetworkError;
    }

    return QtCloudServices::BackendError;
}

QJsonObject QEnginioOperationObject::data() const
{
    return QJsonDocument::fromJson(pData()).object();
}

QByteArray QEnginioOperationObject::pData() const
{
    if (iData.isEmpty() && iNetworkReply->isFinished()) {
        iData = iNetworkReply->readAll();
    }

    return iData;
}
void QEnginioOperationObject::dumpDebugInfo() const
{
    static QHash<QNetworkAccessManager::Operation, QByteArray> operationNames;
    operationNames[QNetworkAccessManager::GetOperation] = "GET";
    operationNames[QNetworkAccessManager::PutOperation] = "PUT";
    operationNames[QNetworkAccessManager::PostOperation] = "POST";
    operationNames[QNetworkAccessManager::DeleteOperation] = "DELETE";
    operationNames[QNetworkAccessManager::CustomOperation] = "CUSTOM";

    QNetworkRequest request = iNetworkReply->request();
    qDebug() << "NetworkReply:" << iNetworkReply;
    qDebug() << "  Request URL:" << request.url().toString(/*FormattingOptions*/ QUrl::None);
    qDebug() << "  Operation:" << operationNames[iNetworkReply->operation()];
    qDebug() << "  HTTP return code:" << backendStatus();
    qDebug() << "  Headers[Content-Type]:" << request.header(QNetworkRequest::ContentTypeHeader);
    qDebug() << "  Raw headers:" << request.rawHeaderList();
    qDebug() << "  RawHeaders[Accept]:" << request.rawHeader(QtCloudServicesConstants::Accept);
    qDebug() << "  RawHeaders[Authorization]:" << request.rawHeader(QtCloudServicesConstants::Authorization);
    qDebug() << "  RawHeaders[Content-Type]:" << request.rawHeader(QtCloudServicesConstants::Content_Type);
    qDebug() << "  RawHeaders[X_Request_Id]:" << request.rawHeader(QtCloudServicesConstants::X_Request_Id);

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

void QEnginioOperationObject::setEnginioRequest(const QEnginioRequest &aEnginioRequest)
{
    iEnginioRequest = aEnginioRequest;
}

void QEnginioOperationObject::setNetworkReply(QSharedPointer<QEnginioOperationObject> aSelf,
        QNetworkReply *aNetworkReply)
{
    if (iNetworkReply != NULL) {
        iConnection->unregisterReply(iNetworkReply);

        if (iNetworkReply->isFinished()) {
            iNetworkReply->deleteLater();
        } else {
            iNetworkReply->setParent(iNetworkReply->manager());
            QObject::connect(iNetworkReply, &QNetworkReply::finished, iNetworkReply, &QNetworkReply::deleteLater);
            iNetworkReply->abort();
        }
    }

    iNetworkReply = aNetworkReply;
    iData = QByteArray();

    if (iNetworkReply == NULL || !aSelf) {
        return;
    }

    iConnection->registerReply(aNetworkReply, aSelf);
}

void QEnginioOperationObject::operationFinished(QSharedPointer<QEnginioOperationObject> aSelf)
{
    QEnginioRequestPrivate *enginioRequest;
    enginioRequest = reinterpret_cast<QEnginioRequestPrivate *>(QTC_D_PTR(&iEnginioRequest));

    if (!enginioRequest) {
        return;
    }

    if (enginioRequest->iCallback) {
        QEnginioOperation operation;
        QEnginioOperationPrivate *operationPrivate;
        operationPrivate = reinterpret_cast<QEnginioOperationPrivate *>(QTC_D_PTR(&operation));

        if (operationPrivate) {
            operationPrivate->setEnginioOperationObject(aSelf);
        }

        enginioRequest->iCallback(operation);
    }
}

/*
** QEnginioOperationPrivate
*/
QEnginioOperationPrivate::QEnginioOperationPrivate()
{

}
QEnginioOperationPrivate::~QEnginioOperationPrivate()
{
}

QSharedPointer<QEnginioOperationObject> QEnginioOperationPrivate::enginioOperationObject() const
{
    return iObject;
}
void QEnginioOperationPrivate::setEnginioOperationObject(QSharedPointer<QEnginioOperationObject> aObject)
{
    iObject = aObject;
}

bool QEnginioOperationPrivate::isValid() const
{
    if (iObject) {
        return true; //  return iObject->isValid();
    }

    return false;
}

bool QEnginioOperationPrivate::isError() const
{
    if (iObject) {
        return iObject->isError();
    }

    return false;
}
bool QEnginioOperationPrivate::isFinished() const
{
    if (iObject) {
        return iObject->isFinished();
    }

    return false;
}

QNetworkReply::NetworkError QEnginioOperationPrivate::errorCode() const
{
    if (iObject) {
        return iObject->errorCode();
    }

    return QNetworkReply::NetworkError::NoError;
}

int QEnginioOperationPrivate::backendStatus() const
{
    if (iObject) {
        return iObject->backendStatus();
    }

    return 0;
}

QString QEnginioOperationPrivate::requestId() const
{
    if (iObject) {
        return iObject->requestId();
    }

    return QString();
}
QString QEnginioOperationPrivate::errorString() const
{
    if (iObject) {
        return iObject->errorString();
    }

    return QString();
}

QtCloudServices::ErrorType QEnginioOperationPrivate::errorType() const
{
    if (iObject) {
        return iObject->errorType();
    }

    return QtCloudServices::ErrorType::NoError;
}

QJsonObject QEnginioOperationPrivate::result() const
{
    if (iObject) {
        return iObject->data();
    }

    return QJsonObject();
}

void QEnginioOperationPrivate::dumpDebugInfo() const
{
    if (iObject) {
        return iObject->dumpDebugInfo();
    }
}

#if 0
class QEnginioOperationPrivate: public QEnginioOperationPrivate {
    QTC_DECLARE_PUBLIC(QEnginioOperation)
public:
    QEnginioOperationPrivate(QEnginioConnectionPrivate *p, QNetworkReply *reply)
        : QEnginioOperationPrivate(p, reply)
    {}
    void emitFinished() Q_DECL_OVERRIDE;
};
#endif
/*!
  \internal
*/
#if 0
QEnginioOperation::QEnginioOperation(QEnginioConnectionPrivate *p, QNetworkReply *reply)
    : QEnginioOperation(p, reply, new QEnginioOperationPrivate(p, reply))
{
    QObject::connect(this, &QEnginioOperation::dataChanged, this, &QEnginioOperation::dataChanged);
}
#endif

#if 0

/*!
  \brief Destroys the QEnginioOperation.

  The reply needs to be deleted after the finished signal is emitted.
*/
QEnginioOperation::~QEnginioOperation()
{}

/*!
  \internal
*/
void QEnginioOperationPrivate::emitFinished()
{
    QTC_Q(QEnginioOperation);
    emit q->finished(q);
}

/*!
  \internal
*/
void QEnginioOperation::swapNetworkReply(QEnginioOperation *other)
{
    QTC_D(QEnginioOperation);
    d->swapNetworkReply(QTC_D_FUNC(other));
}

void QEnginioOperationPrivate::swapNetworkReply(QEnginioOperationPrivate *other)
{
#if 0
    QTC_Q(QEnginioOperation);
    Q_ASSERT(other->_client == _client);
    _client->unregisterReply(_nreply);
    _client->unregisterReply(other->_nreply);

    qSwap(_nreply, other->_nreply);
    _data = other->_data = QByteArray();

    _client->registerReply(_nreply, q);
    _client->registerReply(other->_nreply, QTC_Q_FUNC(other));
#endif
}

/*!
  \internal
  Mark this QEnginioOperation as not finished, the finished signal
  will be delayed until delayFinishedSignal() is returning true.

  \note The feature can be used only with one QEnginioConnection
*/
void QEnginioOperation::setDelayFinishedSignal(bool delay)
{
#if 0
    QTC_D(QEnginioOperation);
    d->_delay = delay;
    d->_client->finishDelayedReplies();
#endif
}

/*!
  \internal
  Returns true if signal should be delayed
 */
bool QEnginioOperation::delayFinishedSignal()
{
    QTC_D(QEnginioOperation);
    return d->_delay;
}

#endif

#if 0
QEnginioOperation::QEnginioOperation(QEnginioConnectionPrivate *parent, QNetworkReply *reply,
                                     QEnginioOperationPrivate *priv)
    : QCloudServicesObject(*priv, QTC_Q_PTR(parent))
{
#if 0
    parent->registerReply(reply, this);
#endif
}

QEnginioOperation::~QEnginioOperation()
{
#if 0
    QTC_D(QEnginioOperation);
    Q_ASSERT(d->_nreply->parent() == this);

    if (Q_UNLIKELY(!d->isFinished())) {
        QObject::connect(d->_nreply, &QNetworkReply::finished, d->_nreply, &QNetworkReply::deleteLater);
        d->_client->unregisterReply(d->_nreply);
        d->_nreply->setParent(d->_nreply->manager());
        d->_nreply->abort();
    }

#endif
}

#endif


/*
** QEnginioOperation
*/

QEnginioOperation::QEnginioOperation()
    : QCloudServicesObject(*new QEnginioOperationPrivate(), NULL)
{

}
QEnginioOperation::QEnginioOperation(const QEnginioOperation &aOther)
    : QCloudServicesObject(*new QEnginioOperationPrivate(), NULL)
{
    *this = aOther;
}

QEnginioOperation::~QEnginioOperation()
{

}

QEnginioOperation& QEnginioOperation::operator=(const QEnginioOperation &aOther)
{
    QTC_D(QEnginioOperation);
    QEnginioOperationPrivate *other;

    other = reinterpret_cast<QEnginioOperationPrivate *>(QTC_D_PTR(&aOther));

    if (other) {
        d->setEnginioOperationObject(other->enginioOperationObject());
    }

    return *this;
}

bool QEnginioOperation::operator!() const
{
    return !isValid() || isError();
}

bool QEnginioOperation::isValid() const
{
    QTC_D(const QEnginioOperation);
    return d->isValid();
}

/*!
\fn bool QEnginioOperation::isError() const
\brief QEnginioOperation::isError returns whether this reply was unsuccessful
\return true if the reply did not succeed
*/

bool QEnginioOperation::isError() const
{
    QTC_D(const QEnginioOperation);
    return d->isError();
}

/*!
\fn bool QEnginioOperation::isFinished() const
\brief Returns whether this reply was finished or not
\return true if the reply was finished, false otherwise.
*/

bool QEnginioOperation::isFinished() const
{
    QTC_D(const QEnginioOperation);
    return d->isFinished();
}

/*!
\property QEnginioOperation::data
\brief The data returned from the backend
This property holds the JSON data returned by the server after a successful request.
*/
QJsonObject QEnginioOperation::result() const
{
    QTC_D(const QEnginioOperation);
    return d->result();
}

/*!
\property QEnginioOperation::errorType
\return the type of the error
\sa QtCloudServices::ErrorType
*/

QtCloudServices::ErrorType QEnginioOperation::errorType() const
{
    QTC_D(const QEnginioOperation);
    return d->errorType();
}

/*!
\property QEnginioOperation::networkError
This property holds the network error for the request.
*/

QNetworkReply::NetworkError QEnginioOperation::networkError() const
{
    QTC_D(const QEnginioOperation);
    return d->errorCode();
}

/*!
\property QEnginioOperation::errorString
This property holds the error for the request as human readable string.
Check \l{QEnginioOperation::isError()}{isError()} first to check if the reply is an error.
*/

QString QEnginioOperation::errorString() const
{
    QTC_D(const QEnginioOperation);
    return d->errorString();
}

/*!
\property QEnginioOperation::requestId
This property holds the API request ID for the request.
The request ID is useful for end-to-end tracking of requests and to identify
the origin of notifications.
\internal
*/

/*!
\internal
*/
QString QEnginioOperation::requestId() const
{
    QTC_D(const QEnginioOperation);
    return d->requestId();
}

/*!
\property QEnginioOperation::backendStatus
\return the backend return status for this reply.
\sa QtCloudServices::ErrorType
*/
int QEnginioOperation::backendStatus() const
{
    QTC_D(const QEnginioOperation);
    return d->backendStatus();
}


/*!
\internal
*/
void QEnginioOperation::dumpDebugInfo() const
{
    QTC_D(const QEnginioOperation);
    d->dumpDebugInfo();
}



#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, QEnginioOperation aReply)
{
    if (!aReply.isValid()) {
        d << "QEnginioOperation(null)";
        return d;
    }

    d.nospace();
    // d << "QEnginioOperation(" << hex << (void *)aReply << dec;

    if (!aReply.isError()) {
        d << " success data=" << aReply.result();
    } else {
        d << " errorCode=" << aReply.networkError() << " ";
        d << " errorString=" << aReply.errorString() << " ";
        d << " errorResult=" << aReply.result() << " ";
    }

    d << "backendStatus=" << aReply.backendStatus();
    d << ")";

    return d.space();
}

#endif /* QT_NO_DEBUG_STREAM */

QT_END_NAMESPACE
