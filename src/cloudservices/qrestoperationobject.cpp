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

QT_BEGIN_NAMESPACE

/*
** Private
*/

QRestOperationObjectPrivate::QRestOperationObjectPrivate(const QRestConnectionObject *aEnginioConnection,
                            const QRestRequestObject *aEnginioRequest)
{

}

QRestOperationObjectPrivate::~QRestOperationObjectPrivate() {
    if (iShared) {
        deinit();
    }
}

QRestConnectionObject *QRestOperationObjectPrivate::restConnection() const {

}
QRestRequestObject *QRestOperationObjectPrivate::restRequest() const {

}

bool QRestOperationObjectPrivate::isValid() const {
    return iShared->isValid();
}

bool QRestOperationObjectPrivate::isError() const {
    return iShared->isError();
}

bool QRestOperationObjectPrivate::isFinished() const {
    return iShared->isFinished();
}

QtCloudServices::ErrorType QRestOperationObjectPrivate::errorType() const {
    return iShared->errorType();
}
QNetworkReply::NetworkError QRestOperationObjectPrivate::networkError() const {
    return iShared->networkError();
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
void QRestOperationObjectPrivate::dumpDebugInfo() const {
    iShared->dumpDebugInfo(qDebug());
}
#endif

void QRestOperationObjectPrivate::init() {
    Q_Q(QRestOperationObject);
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

void QRestOperationObjectPrivate::deinit() {
    QObject::disconnect(iConnectionDataChanged);
    QObject::disconnect(iConnectionFinished);
    QObject::disconnect(iConnectionProgress);
}

QSharedPointer<QRestOperationShared> QRestOperationObjectPrivate::sharedInstance() const {
    return iShared;
}

void QRestOperationObjectPrivate::setSharedInstance(const QRestOperationObject *aOther) {
    if (iShared) {
        deinit();
    }

    iShared = aOther->d_func()->sharedInstance();

    init();
}

/*
** Public
*/

QRestOperationObject::QRestOperationObject(const QRestConnectionObject *aRestConnection,
                                           const QRestRequestObject *aRestRequest,
                                            QObject *aParent)
    : QObject(*new QRestOperationObjectPrivate(aRestConnection,aRestRequest),aParent)
{

}

QRestConnectionObject *QRestOperationObject::restConnection() const {
    Q_D(const QRestOperationObject);
    return d->restConnection();
}

QRestRequestObject *QRestOperationObject::restRequest() const {
    Q_D(const QRestOperationObject);
    return d->restRequest();
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

QtCloudServices::ErrorType QRestOperationObject::errorType() const {
   Q_D(const QRestOperationObject);
    return d->errorType();
}

QNetworkReply::NetworkError QRestOperationObject::networkError() const {
    Q_D(const QRestOperationObject);
    return d->networkError();
}

QString QRestOperationObject::errorString() const {
    Q_D(const QRestOperationObject);
    return d->errorString();
}

QJsonObject QRestOperationObject::resultJson() const {
    Q_D(const QRestOperationObject);
    return d->resultBytes();
}

QByteArray QRestOperationObject::resultBytes() const {
    Q_D(const QRestOperationObject);
    return d->resultBytes();
}

#ifndef QT_NO_DEBUG_STREAM
void QRestOperationObject::dumpDebugInfo() const {
    Q_D(const QRestOperationObject);
    d->dumpDebugInfo();
}
#endif

QT_END_NAMESPACE
