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

#ifndef QCLOUDSERVICES_QENGINIOOPERATION_P_H
#define QCLOUDSERVICES_QENGINIOOPERATION_P_H

#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsondocument.h>
#include <QtNetwork/qnetworkreply.h>

#include <QtCloudServices/QEnginioOperation.h>

#include <QtCloudServices/private/qenginioconnection_p.h>
#include <QtCloudServices/private/qenginiocollection_p.h>
#include <QtCloudServices/private/qcloudservicesobject_p.h>

QT_BEGIN_NAMESPACE

class QEnginioConnectionObject;
class QEnginioOperationObject : public QObject {
    Q_OBJECT
    friend class QEnginioConnectionObject;
public:
    QEnginioOperationObject(QSharedPointer<QEnginioConnectionObject> aConnection);
    ~QEnginioOperationObject();

    bool isError() const Q_REQUIRED_RESULT;
    bool isFinished() const Q_REQUIRED_RESULT;
    QNetworkReply::NetworkError errorCode() const Q_REQUIRED_RESULT;
    int backendStatus() const Q_REQUIRED_RESULT;
    QString requestId() const Q_REQUIRED_RESULT;
    QString errorString() const Q_REQUIRED_RESULT;
    QtCloudServices::ErrorType errorType() const Q_REQUIRED_RESULT;

    QJsonObject result() const Q_REQUIRED_RESULT;
    QByteArray resultBytes() const Q_REQUIRED_RESULT; // pData

    int resultObjectCount() const Q_REQUIRED_RESULT;
    QEnginioObject resultObject() const Q_REQUIRED_RESULT;
    QList<QEnginioObject> resultObjects() const Q_REQUIRED_RESULT;


    void dumpDebugInfo() const;
protected:
    void setEnginioCollection(QSharedPointer<QEnginioCollectionObject> aEnginioCollection);
    void setEnginioRequest(const QEnginioRequest &aEnginioRequest);
    void setNetworkReply(QSharedPointer<QEnginioOperationObject> aSelf,
                         QNetworkReply *aNetworkReply);

    void operationFinished(QSharedPointer<QEnginioOperationObject> aSelf);
private:
    QSharedPointer<QEnginioConnectionObject> iConnection; // Client
    QSharedPointer<QEnginioCollectionObject> iEnginioCollection;

    QEnginioRequest iEnginioRequest;
    QNetworkReply *iNetworkReply; //  _nreply;
    bool iDelay; //  _delay;
    mutable QByteArray iData; //  _data;

    QJsonObject iJsonObject;
    QList<QEnginioObject> iResultObjects;
};


class QEnginioOperationPrivate : public QCloudServicesObjectPrivate {
    Q_OBJECT
public:
    QEnginioOperationPrivate();
    ~QEnginioOperationPrivate();
public:
    QSharedPointer<QEnginioOperationObject> enginioOperationObject() const;
    void setEnginioOperationObject(QSharedPointer<QEnginioOperationObject> aObject);

    bool isValid() const Q_REQUIRED_RESULT;
    bool isError() const Q_REQUIRED_RESULT;
    bool isFinished() const Q_REQUIRED_RESULT;

    QNetworkReply::NetworkError errorCode() const Q_REQUIRED_RESULT;
    int backendStatus() const Q_REQUIRED_RESULT;
    QString requestId() const Q_REQUIRED_RESULT;
    QString errorString() const Q_REQUIRED_RESULT;
    QtCloudServices::ErrorType errorType() const Q_REQUIRED_RESULT;

    QJsonObject result() const Q_REQUIRED_RESULT;

    int resultObjectCount() const Q_REQUIRED_RESULT;
    QEnginioObject resultObject() const Q_REQUIRED_RESULT;
    QList<QEnginioObject> resultObjects() const Q_REQUIRED_RESULT;

#if 0
    static QEnginioOperationPrivate *get(QEnginioOperation *p)
    {
        return QTC_D_FUNC(p);
    }

    QEnginioOperationPrivate(QEnginioConnectionPrivate *p, QNetworkReply *reply)
        : QCloudServicesObjectPrivate(),
          _client(p)
          , _nreply(reply)
    {

    }
#endif

    void dumpDebugInfo() const;
public:
    QTC_DECLARE_PUBLIC(QEnginioOperation)


    void emitFinished();
// void swapNetworkReply(QEnginioOperationPrivate *other);

private:
    QSharedPointer<QEnginioOperationObject> iObject;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOOPERATION_P_H */
