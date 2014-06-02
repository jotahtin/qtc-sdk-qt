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

#include <QtCloudServices/private/qrestconnectionobject_p.h>
#include <QtCloudServices/private/qrestconnectionshared_p.h>
#include <QtCloudServices/private/qrestoperationobject_p.h>
#include <QtCloudServices/private/qrestrequestobject_p.h>
#include <QtCloudServices/private/qrestendpointobject_p.h>



QT_BEGIN_NAMESPACE

/*
** Private
*/

QRestConnectionObjectPrivate::QRestConnectionObjectPrivate() {
}

QRestConnectionObjectPrivate::~QRestConnectionObjectPrivate() {
    if (iShared) {
        deinit();
    }
}

bool QRestConnectionObjectPrivate::isValid() const {
    if (!iShared) {
        return false;
    }

    return iShared->isValid();
}

QSharedPointer<QNetworkAccessManager> QRestConnectionObjectPrivate::networkManager() const {
    return iShared->networkManager();
}

QRestOperationObject *QRestConnectionObjectPrivate::restRequest(const QRestRequestObject *aRequest) {
    QRestOperationObject *op;
    QSharedPointer<QRestOperationShared> opShared;

    opShared = iShared->restRequest(iShared, aRequest->d_func()->sharedInstance());

    op = buildOperationObject();
    op->d_func()->setSharedInstance(opShared);

    return op;
}

void QRestConnectionObjectPrivate::init() {

}

void QRestConnectionObjectPrivate::deinit() {

}

QRestOperationObject* QRestConnectionObjectPrivate::buildOperationObject() const {
    return new QRestOperationObject;
}

QSharedPointer<QRestConnectionShared> QRestConnectionObjectPrivate::sharedInstance() const {
    return iShared;
}
void QRestConnectionObjectPrivate::setSharedInstance(QSharedPointer<QRestConnectionShared> aShared) {
    if (iShared) {
        deinit();
    }

    iShared = aShared;

    if (iShared) {
        init();
    }
}

/*
** Public
*/

QRestConnectionObject::QRestConnectionObject(QRestConnectionObjectPrivate &dd,QObject *aParent)
    : QObject(dd,aParent)
{

}

QRestConnectionObject::QRestConnectionObject(QObject *aParent)
    : QObject(*new QRestConnectionObjectPrivate, aParent)
{

}

bool QRestConnectionObject::isValid() const {
    Q_D(const QRestConnectionObject);
    return d->isValid();
}
QSharedPointer<QNetworkAccessManager> QRestConnectionObject::networkManager() const {
    Q_D(const QRestConnectionObject);
    return d->networkManager();
}

QRestOperationObject *QRestConnectionObject::restRequest(const QRestRequestObject *aRequest) {
    Q_D(QRestConnectionObject);
    return d->restRequest(aRequest);
}

QT_END_NAMESPACE
