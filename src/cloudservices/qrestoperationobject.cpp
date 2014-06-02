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

#include <QtCloudServices/private/qrestoperationobject_p.h>
#include <QtCloudServices/private/qrestoperationshared_p.h>
#include <QtCloudServices/private/qrestconnectionobject_p.h>
#include <QtCloudServices/private/qrestrequestobject_p.h>

QT_BEGIN_NAMESPACE

/*
** Private
*/

QRestOperationObjectPrivate::QRestOperationObjectPrivate()
{

}

QRestOperationObjectPrivate::~QRestOperationObjectPrivate() {
    if (iShared) {
        deinit();
    }
}

QRestConnectionObject *QRestOperationObjectPrivate::connection() const {
    QRestConnectionObject *obj;

    obj=buildConnectionObject();
    obj->d_func()->setSharedInstance(iShared->connection());

    return obj;
}
QRestRequestObject *QRestOperationObjectPrivate::request() const {
    QRestRequestObject *obj;

    obj=buildRequestObject();
    obj->d_func()->setSharedInstance(iShared->request());

    return obj;
}

bool QRestOperationObjectPrivate::isValid() const {
    if (!iShared) {
        return false;
    }
    return iShared->isValid();
}

bool QRestOperationObjectPrivate::isError() const {
    return iShared->isError();
}

bool QRestOperationObjectPrivate::isFinished() const {
    return iShared->isFinished();
}

int QRestOperationObjectPrivate::backendStatus() const {
    return iShared->backendStatus();
}
QString QRestOperationObjectPrivate::requestId() const {
    return iShared->requestId();
}

QtCloudServices::ErrorType QRestOperationObjectPrivate::errorType() const {
    return iShared->errorType();
}
QNetworkReply::NetworkError QRestOperationObjectPrivate::errorCode() const {
    return iShared->errorCode();
}
QString QRestOperationObjectPrivate::errorString() const {
    return iShared->errorString();
}

QJsonObject QRestOperationObjectPrivate::resultJson() const {
    return iShared->resultJson();
}
QByteArray QRestOperationObjectPrivate::resultBytes() const {
    return iShared->resultBytes();
}

#ifndef QT_NO_DEBUG_STREAM
void QRestOperationObjectPrivate::dumpDebugInfo(QDebug &d) const {
    iShared->dumpDebugInfo(d);
}
#endif

void QRestOperationObjectPrivate::init() {
    /*
    Q_Q(QRestOperationObject);
    iConnectionFinished
    = QObject::connect(iShared.data(), &QRestOperationShared::finished,
              q, &QRestOperationObject::finished);
    */

    /*
    iConnectionDataChanged
            = QObject::connect(iShared.data(), &QRestOperationShared::dataChanged,
                      q, &QRestOperationObject::dataChanged);
    iConnectionProgress
    = QObject::connect(iShared.data(), &QRestOperationShared::progress,
              q, &QRestOperationObject::progress);
    */
}

void QRestOperationObjectPrivate::deinit() {
    QObject::disconnect(iConnectionFinished);
    /*
    QObject::disconnect(iConnectionDataChanged);
    QObject::disconnect(iConnectionProgress);
    */
}

QSharedPointer<QRestOperationShared> QRestOperationObjectPrivate::sharedInstance() const {
    return iShared;
}

void QRestOperationObjectPrivate::setSharedInstance(QSharedPointer<QRestOperationShared> aShared) {
    if (iShared) {
        deinit();
    }

    iShared = aShared;

    if (iShared) {
        init();
    }
}

QRestConnectionObject *QRestOperationObjectPrivate::buildConnectionObject() const {
    return new QRestConnectionObject;
}
QRestRequestObject *QRestOperationObjectPrivate::buildRequestObject() const {
    return new QRestRequestObject;
}


/*
** Public
*/

QRestOperationObject::QRestOperationObject(QRestOperationObjectPrivate &dd,QObject *aParent)
    : QObject(dd,aParent)
{}

QRestOperationObject::QRestOperationObject(QObject *aParent)
    : QObject(*new QRestOperationObjectPrivate,aParent)
{

}

QRestConnectionObject *QRestOperationObject::connection() const {
    Q_D(const QRestOperationObject);
    return d->connection();
}

QRestRequestObject *QRestOperationObject::request() const {
    Q_D(const QRestOperationObject);
    return d->request();
}

bool QRestOperationObject::isValid() const {
    Q_D(const QRestOperationObject);
    return d->isValid();
}

bool QRestOperationObject::isError() const {
    Q_D(const QRestOperationObject);
    return d->isError();
}

bool QRestOperationObject::isFinished() const {
    Q_D(const QRestOperationObject);
    return d->isFinished();
}

int QRestOperationObject::backendStatus() const {
    Q_D(const QRestOperationObject);
    return d->backendStatus();
}
QString QRestOperationObject::requestId() const {
    Q_D(const QRestOperationObject);
    return d->requestId();
}

QtCloudServices::ErrorType QRestOperationObject::errorType() const {
   Q_D(const QRestOperationObject);
    return d->errorType();
}

QNetworkReply::NetworkError QRestOperationObject::errorCode() const {
    Q_D(const QRestOperationObject);
    return d->errorCode();
}

QString QRestOperationObject::errorString() const {
    Q_D(const QRestOperationObject);
    return d->errorString();
}

QJsonObject QRestOperationObject::resultJson() const {
    Q_D(const QRestOperationObject);
    return d->resultJson();
}

QByteArray QRestOperationObject::resultBytes() const {
    Q_D(const QRestOperationObject);
    return d->resultBytes();
}

#ifndef QT_NO_DEBUG_STREAM
void QRestOperationObject::dumpDebugInfo(QDebug &dd) const {
    Q_D(const QRestOperationObject);
    d->dumpDebugInfo(dd);
}
#endif

void QRestOperationObject::setSharedInstanceFrom(const QRestOperationObject *aOther) {
    Q_D(QRestOperationObject);
    d->setSharedInstance(aOther->d_func()->sharedInstance());
}

QT_END_NAMESPACE
