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

#ifndef QCLOUDSERVICES_QRESTREQUEST_SHARED_P_H
#define QCLOUDSERVICES_QRESTREQUEST_SHARED_P_H

#include <QtCore/qjsonobject.h>
#include <QUrlQuery>

#include <QtCloudServices/qtcloudservices_global.h>
#include <QtCloudServices/qtcloudservices.h>

QT_BEGIN_NAMESPACE

class QRestOperationShared;
class QRestOperationObject;
class QRestRequestShared : public QObject {
    Q_OBJECT
    friend class QRestOperationShared;
private:
    Q_DISABLE_COPY(QRestRequestShared)
public:
    QRestRequestShared();
    QRestRequestShared(QtCloudServices::RESTOperation aOperation, QString aPath);
    virtual ~QRestRequestShared();
public:
    QtCloudServices::RESTOperation operation() const Q_REQUIRED_RESULT;
    QString path() const Q_REQUIRED_RESULT;

    QUrlQuery urlQuery() const Q_REQUIRED_RESULT;
    void setUrlQuery(const QUrlQuery &aUrlQuery);

    QJsonObject payload() const Q_REQUIRED_RESULT;
    void setPayload(const QJsonObject &aPayload);

    QJsonObject extraHeaders() const Q_REQUIRED_RESULT;
    void setExtraHeaders(const QJsonObject &aExtraHeaders);

    void setCallback(std::function<void(QSharedPointer<QRestOperationShared>)> aCallback);
protected:
    void operationFinished(QSharedPointer<QRestOperationShared> aOperation);
Q_SIGNALS:
    void urlQueryChanged(const QUrlQuery &aUrlQuery);
    void payloadChanged(const QJsonObject &aPayload);
    void extraHeadersChanged(const QJsonObject &aExtraHeaders);
private:
    QtCloudServices::RESTOperation iOperation;
    QString iPath;
    QUrlQuery iUrlQuery;
    QJsonObject iPayload;
    QJsonObject iExtraHeaders;

    std::function<void(QSharedPointer<QRestOperationShared> aOperation)> iCallback;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTREQUEST_SHARED_P_H */
