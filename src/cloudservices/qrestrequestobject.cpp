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

#include <QDebug>

#include <QtCloudServices/private/qrestrequestobject_p.h>
#include <QtCloudServices/private/qrestrequestshared_p.h>

QT_BEGIN_NAMESPACE

/*
** Private
*/
QRestRequestObjectPrivate::QRestRequestObjectPrivate(QSharedPointer<QRestRequestShared> aShared)
    : iShared(aShared)
{

}
QRestRequestObjectPrivate::QRestRequestObjectPrivate(QtCloudServices::RESTOperation aOperation, QString aPath)
    : iShared(new QRestRequestShared(aOperation,aPath))
{

}

QtCloudServices::RESTOperation QRestRequestObjectPrivate::operation() const {
    return iShared->operation();
}

QString QRestRequestObjectPrivate::path() const {
    return iShared->path();
}

QUrlQuery QRestRequestObjectPrivate::urlQuery() const {
    return iShared->urlQuery();
}
void QRestRequestObjectPrivate::setUrlQuery(const QUrlQuery &aUrlQuery) {
    iShared->setUrlQuery(aUrlQuery);
}

QJsonObject QRestRequestObjectPrivate::payload() const {
    return iShared->payload();
}
void QRestRequestObjectPrivate::setPayload(const QJsonObject &aPayload) {
    iShared->setPayload(aPayload);
}

QJsonObject QRestRequestObjectPrivate::extraHeaders() const {

}
void QRestRequestObjectPrivate::setExtraHeaders(const QJsonObject &aExtraHeaders) {
    iShared->setExtraHeaders(aExtraHeaders);
}

void QRestRequestObjectPrivate::setRestCallback(std::function<void(QRestOperationObject *)> aCallback) {
    iShared->setRestCallback(aCallback);
}

void QRestRequestObjectPrivate::init() {
    Q_Q(QRestRequestObject);
    iConnectionDataChanged
            = QObject::connect(iShared.data(), &QRestOperationObjectShared::dataChanged,
                      q, &QRestOperationObject::dataChanged);
    iConnectionFinished
    = QObject::connect(iShared.data(), &QRestOperationObjectShared::finished,
              q, &QRestOperationObject::finished);
    iConnectionProgress
    = QObject::connect(iShared.data(), &QRestOperationObjectShared::progress,
              q, &QRestOperationObject::progress);

}

void QRestRequestObjectPrivate::deinit() {
    QObject::disconnect(iConnectionDataChanged);
    QObject::disconnect(iConnectionFinished);
    QObject::disconnect(iConnectionProgress);
}

QSharedPointer<QRestOperationShared> QRestRequestObjectPrivate::sharedInstance() const {
    return iShared;
}

void QRestRequestObjectPrivate::setSharedInstance(const QRestOperationObject *aOther) {
    if (iShared) {
        deinit();
    }

    iShared = aOther->d_func()->sharedInstance();

    init();
}

/*
** Public
*/

QRestRequestObject::QRestRequestObject(QRestRequestObjectPrivate &dd,QObject *aParent)
    : QObject(dd,aParent)
{

}

QRestRequestObject::QRestRequestObject(QtCloudServices::RESTOperation aOperation, QString aPath, QObject *aParent)
    : QObject(*new QRestRequestObjectPrivate(aOperation,aPath),aParent)
{

}

QtCloudServices::RESTOperation QRestRequestObject::operation() const {
    Q_D(const QRestRequestObject);
    return d->operation();
}
QString QRestRequestObject::path() const {
    Q_D(const QRestRequestObject);
    return d->path();
}

QUrlQuery QRestRequestObject::urlQuery() const {
    Q_D(const QRestRequestObject);
    return d->urlQuery();
}
void QRestRequestObject::setUrlQuery(const QUrlQuery &aUrlQuery) {
    Q_D(QRestRequestObject);
    d->setUrlQuery(aUrlQuery);
}

QJsonObject QRestRequestObject::payload() const {
    Q_D(const QRestRequestObject);
    return d->payload();
}
void QRestRequestObject::setPayload(const QJsonObject &aPayload) {
    Q_D(QRestRequestObject);
    d->setPayload(aPayload);
}

QJsonObject QRestRequestObject::extraHeaders() const {
    Q_D(const QRestRequestObject);
    return d->extraHeaders();
}
void QRestRequestObject::setExtraHeaders(const QJsonObject &aExtraHeaders) {
    Q_D(QRestRequestObject);
    d->setExtraHeaders(aExtraHeaders);
}

void QRestRequestObject::setRestCallback(std::function<void(QRestOperationObject *)> aCallback) {
    Q_D(QRestRequestObject);
    d->setRestCallback(aCallback);
}

void QRestRequestObject::setSharedInstanceFrom(const QRestRequestObject *aOther) {
    const QRestRequestObjectPrivate *otherPrivate;
    Q_D(QRestRequestObject);

    otherPrivate=reinterpret_cast<QRestRequestObjectPrivate *>(aOther->d_ptr);
    if (otherPrivate) {
        d->setSharedInstance(otherPrivate->sharedInstance());
    }
}

QT_END_NAMESPACE
