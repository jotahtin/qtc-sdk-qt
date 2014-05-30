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
** QEnginioOperationPrivate
*/
QEnginioOperationPrivate::QEnginioOperationPrivate()
    : iNetworkReply(NULL),
      iDelay(false)
{

}
QEnginioOperationPrivate::QEnginioOperationPrivate(const QEnginioConnection &aEnginioConnection,
        const QEnginioRequest &aEnginioRequest)
    : iEnginioConnection(aEnginioConnection),
      iEnginioRequest(aEnginioRequest),
      iNetworkReply(NULL),
      iDelay(false)
{
}

QEnginioOperationPrivate::~QEnginioOperationPrivate()
{
    setNetworkReply(NULL);
}

bool QEnginioOperationPrivate::isValid() const
{
    if (!iEnginioConnection) {
        return false;
    }

    return true;
}

bool QEnginioOperationPrivate::isError() const
{
    return errorCode() != QNetworkReply::NoError;
}
bool QEnginioOperationPrivate::isFinished() const
{
    return iNetworkReply->isFinished() && Q_LIKELY(!iDelay);
}

QNetworkReply::NetworkError QEnginioOperationPrivate::errorCode() const
{
    return iNetworkReply->error();
}

int QEnginioOperationPrivate::backendStatus() const
{
    return iNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
}

QString QEnginioOperationPrivate::requestId() const
{
    return QString::fromUtf8(iNetworkReply->request().rawHeader(QtCloudServicesConstants::X_Request_Id));
}

QString QEnginioOperationPrivate::errorString() const
{
    if (errorType() == QtCloudServices::BackendError) {
        return QString::fromUtf8(resultBytes());
    }

    return iNetworkReply->errorString();
}

QtCloudServices::ErrorType QEnginioOperationPrivate::errorType() const
{
    if (errorCode() == QNetworkReply::NoError) {
        return QtCloudServices::NoError;
    }

    if (resultBytes().isEmpty()) {
        return QtCloudServices::NetworkError;
    }

    return QtCloudServices::BackendError;
}

QJsonObject QEnginioOperationPrivate::result() const
{
    return iJsonObject;
}

QByteArray QEnginioOperationPrivate::resultBytes() const
{
    if (iData.isEmpty() && iNetworkReply->isFinished()) {
        iData = iNetworkReply->readAll();
    }

    return iData;
}


int QEnginioOperationPrivate::resultObjectCount() const
{
    return iResultObjects.size();
}

QEnginioObject QEnginioOperationPrivate::resultObject() const
{
    if (iResultObjects.size() == 1) {
        iResultObjects.first();
    }

    return QEnginioObject();
}

QList<QEnginioObject> QEnginioOperationPrivate::resultObjects() const
{
    return iResultObjects;
}

QEnginioRequest QEnginioOperationPrivate::enginioRequest() const
{
    return iEnginioRequest;
}

void QEnginioOperationPrivate::dumpDebugInfo() const
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

void QEnginioOperationPrivate::setEnginioRequest(const QEnginioRequest &aEnginioRequest)
{
    iEnginioRequest = aEnginioRequest;
}

void QEnginioOperationPrivate::setNetworkReply(QNetworkReply *aNetworkReply)
{
    if (iNetworkReply != NULL) {
        iEnginioConnection.d<QEnginioConnection>()->unregisterReply(iNetworkReply);

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

    iEnginioConnection.d<QEnginioConnection>()->registerReply(aNetworkReply, *q<QEnginioOperation>());
}

void QEnginioOperationPrivate::operationFinished()
{
    QEnginioCollection enginioCollection;
    enginioCollection = iEnginioRequest.enginioCollection();

    iJsonObject = QJsonDocument::fromJson(resultBytes()).object();

    iResultObjects.clear();

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
                qDebug() << "---- NEED RESOLVE (PER OBJECT)..."
                         << jsonObject.value(QtCloudServicesConstants::objectType);

                continue;
            }

            iResultObjects.push_back(enginioCollection.fromJsonObject((*i).toObject()));
        }
    } else if (!iJsonObject.isEmpty()) {
        if (!enginioCollection) {
            qDebug() << "---- NEED RESOLVE (PER OBJECT)..."
                     << iJsonObject.value(QtCloudServicesConstants::objectType);
        } else {
            iResultObjects.push_back(enginioCollection.fromJsonObject(iJsonObject));
        }
    }

    QEnginioRequest::dvar enginioRequest;
    enginioRequest = iEnginioRequest.d<QEnginioRequest>();

    if (!!enginioRequest && enginioRequest->iCallback) {
        enginioRequest->iCallback(*q<QEnginioOperation>());
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

QEnginioOperation::QEnginioOperation(const QEnginioConnection &aEnginioConnection,
                                     const QEnginioRequest &aRequest)
    : QCloudServicesObject(QEnginioOperation::dvar(new QEnginioOperationPrivate(aEnginioConnection, aRequest)))
{

}
QEnginioOperation::QEnginioOperation()
    : QCloudServicesObject(QEnginioOperation::dvar(new QEnginioOperationPrivate()))
{

}
QEnginioOperation::QEnginioOperation(const QEnginioOperation &aOther)
    : QCloudServicesObject(aOther.d<QEnginioOperation>())
{
}

QEnginioOperation::~QEnginioOperation()
{

}

QEnginioOperation & QEnginioOperation::operator=(const QEnginioOperation &aOther)
{
    setPIMPL(aOther.d<QEnginioOperation>());
    return *this;
}

bool QEnginioOperation::operator!() const
{
    return !isValid() || isError();
}

bool QEnginioOperation::isValid() const
{
    return d<const QEnginioOperation>()->isValid();
}

/*!
\fn bool QEnginioOperation::isError() const
\brief QEnginioOperation::isError returns whether this reply was unsuccessful
\return true if the reply did not succeed
*/

bool QEnginioOperation::isError() const
{
    return d<const QEnginioOperation>()->isError();
}

/*!
\fn bool QEnginioOperation::isFinished() const
\brief Returns whether this reply was finished or not
\return true if the reply was finished, false otherwise.
*/

bool QEnginioOperation::isFinished() const
{
    return d<const QEnginioOperation>()->isFinished();
}

/*!
\property QEnginioOperation::data
\brief The data returned from the backend
This property holds the JSON data returned by the server after a successful request.
*/
QJsonObject QEnginioOperation::result() const
{
    return d<const QEnginioOperation>()->result();
}

int QEnginioOperation::resultObjectCount() const
{
    return d<const QEnginioOperation>()->resultObjectCount();
}
QEnginioObject QEnginioOperation::resultObject() const
{
    return d<const QEnginioOperation>()->resultObject();
}
QList<QEnginioObject> QEnginioOperation::resultObjects() const
{
    return d<const QEnginioOperation>()->resultObjects();
}


/*!
\property QEnginioOperation::errorType
\return the type of the error
\sa QtCloudServices::ErrorType
*/

QtCloudServices::ErrorType QEnginioOperation::errorType() const
{
    return d<const QEnginioOperation>()->errorType();
}

/*!
\property QEnginioOperation::networkError
This property holds the network error for the request.
*/

QNetworkReply::NetworkError QEnginioOperation::networkError() const
{
    return d<const QEnginioOperation>()->errorCode();
}

/*!
\property QEnginioOperation::errorString
This property holds the error for the request as human readable string.
Check \l{QEnginioOperation::isError()}{isError()} first to check if the reply is an error.
*/

QString QEnginioOperation::errorString() const
{
    return d<const QEnginioOperation>()->errorString();
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
    return d<const QEnginioOperation>()->requestId();
}

/*!
\property QEnginioOperation::backendStatus
\return the backend return status for this reply.
\sa QtCloudServices::ErrorType
*/
int QEnginioOperation::backendStatus() const
{
    return d<const QEnginioOperation>()->backendStatus();
}

QEnginioRequest QEnginioOperation::enginioRequest() const
{
    return d<const QEnginioOperation>()->enginioRequest();
}

/*!
\internal
*/
void QEnginioOperation::dumpDebugInfo() const
{
    d<const QEnginioOperation>()->dumpDebugInfo();
}



#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, const QEnginioOperation &aReply)
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
