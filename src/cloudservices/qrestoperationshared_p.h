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

#ifndef QCLOUDSERVICES_QRESTOPERATION_SHARED_P_H
#define QCLOUDSERVICES_QRESTOPERATION_SHARED_P_H

#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsondocument.h>
#include <QtNetwork/qnetworkreply.h>

#include <QtCloudServices/qtcloudservices_global.h>
#include <QtCloudServices/qtcloudservices.h>

QT_BEGIN_NAMESPACE

class QRestRequestShared;
class QRestConnectionShared;
class QRestOperationShared : public QObject {
    Q_OBJECT
    friend class QRestConnectionShared;
private:
    Q_DISABLE_COPY(QRestOperationShared)
public:
    QRestOperationShared(QSharedPointer<QRestConnectionShared> aConnection,
                         QSharedPointer<QRestRequestShared> aRequest);
    ~QRestOperationShared();
public:
    virtual bool isValid() const Q_REQUIRED_RESULT;
    virtual bool isError() const Q_REQUIRED_RESULT;
    virtual bool isFinished() const Q_REQUIRED_RESULT;

    QSharedPointer<QRestConnectionShared> connection() const Q_REQUIRED_RESULT;
    QSharedPointer<QRestRequestShared> request() const Q_REQUIRED_RESULT;

    // void setConnection(QSharedPointer<QRestConnectionShared> aConnection);
    // void setRequest(QSharedPointer<QRestRequestShared> aRequest);

    int backendStatus() const Q_REQUIRED_RESULT;
    QString requestId() const Q_REQUIRED_RESULT;

    virtual QtCloudServices::ErrorType errorType() const Q_REQUIRED_RESULT;
    virtual QNetworkReply::NetworkError errorCode() const Q_REQUIRED_RESULT;
    virtual QString errorString() const Q_REQUIRED_RESULT;

    QJsonObject resultJson() const Q_REQUIRED_RESULT;
    QByteArray resultBytes() const Q_REQUIRED_RESULT;

#ifndef QT_NO_DEBUG_STREAM
    virtual void dumpDebugInfo(QDebug &d) const;
#endif
protected:
    // void setRestRequest(const QEnginioRequest &aEnginioRequest);
    void setNetworkReply(QSharedPointer<QRestOperationShared> aSelf,
                         QNetworkReply *aNetworkReply);
    void operationFinishedPrepare(QSharedPointer<QRestOperationShared> aSelf);
    virtual void operationFinished(QSharedPointer<QRestOperationShared> aSelf);
Q_SIGNALS:
    void finished();
private:
    QSharedPointer<QRestConnectionShared> iConnection;
    QSharedPointer<QRestRequestShared> iRequest;
    QNetworkReply *iNetworkReply;
protected:
    mutable QByteArray iData;    
    QJsonObject iJsonObject;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTOPERATION_SHARED_P_H */
