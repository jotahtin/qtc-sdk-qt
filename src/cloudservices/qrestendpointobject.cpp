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

#include "QtCloudServices/private/qrestendpointobject_p.h"
#include "QtCloudServices/private/qrestendpointshared_p.h"
#include "QtCloudServices/private/qrestconnectionobject_p.h"

QT_BEGIN_NAMESPACE

/*
** Private
*/

QRestEndpointObjectPrivate::QRestEndpointObjectPrivate(QSharedPointer<QRestEndpointShared> aShared)
    : iShared(aShared)
{

}

QRestEndpointObjectPrivate::QRestEndpointObjectPrivate()
    : iShared(new QRestEndpointShared)
{
}

QRestEndpointObjectPrivate::QRestEndpointObjectPrivate(const QUrl &aEndpointAddress)
    : iShared(new QRestEndpointShared(aEndpointAddress))
{
}

QRestEndpointObjectPrivate::~QRestEndpointObjectPrivate()
{
    if (iShared) {
        deinit();
    }
}

bool QRestEndpointObjectPrivate::isValid() const
{
    if (iShared.isNull()) {
        return false;
    }

    return iShared->isValid();
}

QUrl QRestEndpointObjectPrivate::endpointAddress() const
{
    return iShared->endpointAddress();
}

void QRestEndpointObjectPrivate::setEndpointAddress(const QUrl &aEndpointAddress) {
    Q_Q(QRestEndpointObject);
    setSharedInstance(new QRestEndpointShared(aEndpointAddress));

    emit q->endpointAddressChanged(aEndpointAddress);
}

QRestConnectionObject *QRestEndpointObjectPrivate::reserveConnection()
{
    QRestConnectionObject *obj;
    QRestConnectionObjectPrivate *objPrv;
    QSharedPointer<QRestConnectionShared> conShared;

    conShared=iShared->reserveConnection(iShared);
    if (!conShred) {
        return NULL;
    }

    obj = new QRestConnectionObject();
    objPrv = reinterpret_cast<QRestConnectionObjectPrivate *>(obj->d_ptr);

    objPrv->setSharedInstance(conShared);

    return obj;
}

void QRestEndpointObjectPrivate::releaseConnection(const QRestConnectionObject *aConnection)
{
    QRestConnectionObjectPrivate *objPrv;
    if (!aConnection) {
        return;
    }

    objPrv = reinterpret_cast<QRestConnectionObjectPrivate *>(aConnection->d_ptr);

    iShared->releaseConnection(objPrv->sharedInstance());
}

void QRestEndpointObjectPrivate::init() {
}

void QRestEndpointObjectPrivate::deinit() {
}

QSharedPointer<QEnginioDataStorageShared> QRestEndpointObjectPrivate::sharedInstance() const {
    return iShared;
}

void QRestEndpointObjectPrivate::setSharedInstance(QSharedPointer<QRestEndpointShared> aShared) {
   if (iShared) {
       deinit();
   }

   iShared = aShared;

   init();
}

/*
** Public
*/

QRestEndpointObject::QRestEndpointObject(QRestEndpointObjectPrivate &dd,QObject *aParent)
    : QObject(dd,aParent)
{

}

QRestEndpointObject::QRestEndpointObject(QObject *aParent)
 : QObject(*new QRestEndpointObjectPrivate(),aParent)
{

}

QRestEndpointObject::QRestEndpointObject(const QUrl &aEndpointAddress, QObject *aParent)
    : QObject(*new QRestEndpointObjectPrivate(aEndpointAddress),aParent)
{

}

bool QRestEndpointObject::isValid() const {
    Q_D(const QRestEndpointObject);
    return d->isValid();
}

QUrl QRestEndpointObject::endpointAddress() const {
    Q_D(const QRestEndpointObject);
    return d->endpointAddress();
}
void QRestEndpointObject::setEndpointAddress(const QUrl &aEndpointAddress) {
    Q_D(QRestEndpointObject);
    d->setEndpointAddress(aEndpointAddress);
}

QRestConnectionObject *QRestEndpointObject::reserveConnection() {
    Q_D(QRestEndpointObject);
    return d->reserveConnection();
}

void QRestEndpointObject::releaseConnection(QRestConnectionObject *aConnection) {
    Q_D(QRestEndpointObject);
    d->releaseConnection(aConnection);
}

void QRestEndpointObject::setSharedInstanceFrom(const QRestEndpointObject *aOther) {
    Q_D(QRestEndpointObject);
    QRestEndpointObjectPrivate *otherPrv;
    otherPrv=reinterpret_cast<QRestEndpointObjectPrivate *>(aOther->d_ptr);
    d->setSharedInstance(otherPrv->sharedInstance());
}

QT_END_NAMESPACE
