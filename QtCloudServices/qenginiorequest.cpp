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

#include "QtCloudServices/private/qenginiorequest_p.h"

QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/

QEnginioRequestPrivate::QEnginioRequestPrivate()
{

}

/*
** Public Interface
*/
QEnginioRequest::QEnginioRequest(QObject *aParent)
    : QCloudServicesObject(*new QEnginioRequestPrivate(), aParent)
{

}
QEnginioRequest::QEnginioRequest(QtCloudServices::RESTOperation aOperation, QString aPath, QObject *aParent)
    : QCloudServicesObject(*new QEnginioRequestPrivate(), aParent)
{
    QTC_D(QEnginioRequest);
    d->iOperation = aOperation;
    d->iPath = aPath;
}
QEnginioRequest::QEnginioRequest(const QEnginioRequest &aOther)
    : QCloudServicesObject(*new QEnginioRequestPrivate())
{
    *this = aOther;
}

QEnginioRequest& QEnginioRequest::operator=(const QEnginioRequest &aOther)
{
    QTC_D(QEnginioRequest);
    QEnginioRequestPrivate *other;

    other = reinterpret_cast<QEnginioRequestPrivate *>(QTC_D_PTR(&aOther));

    if (other) {
        d->iOperation = other->iOperation;
        d->iPath = other->iPath;
        d->iUrlQuery = other->iUrlQuery;
        d->iPayload = other->iPayload;
        d->iExtraHeaders = other->iExtraHeaders;
        d->iCallback = other->iCallback;
    }

    return *this;
}

QEnginioRequest &QEnginioRequest::urlQuery(const QUrlQuery &aUrlQuery)
{
    QTC_D(QEnginioRequest);
    d->iUrlQuery = aUrlQuery;
    return *this;
}
QEnginioRequest& QEnginioRequest::payload(const QJsonObject &aPayload)
{
    QTC_D(QEnginioRequest);
    d->iPayload = aPayload;
    return *this;
}

QEnginioRequest& QEnginioRequest::headers(const QJsonObject &aExtraHeaders)
{
    QTC_D(QEnginioRequest);
    d->iExtraHeaders = aExtraHeaders;
    return *this;
}

QEnginioRequest& QEnginioRequest::then(std::function<void(QEnginioOperation &)> aCallback)
{
    QTC_D(QEnginioRequest);
    d->iCallback = aCallback;
    return *this;
}

QT_END_NAMESPACE

