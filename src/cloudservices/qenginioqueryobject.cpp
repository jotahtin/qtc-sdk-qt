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
#include <QSharedPointer>

#include <QtCloudServices/private/qenginioqueryobject_p.h>
#include <QtCloudServices/private/qenginioqueryshared_p.h>

QT_BEGIN_NAMESPACE

/*
** Private
*/

QEnginioQueryObjectPrivate::QEnginioQueryObjectPrivate() {

}

QEnginioQueryObjectPrivate::~QEnginioQueryObjectPrivate() {
    if (iShared) {
        deinit();
    }
}

QJsonObject QEnginioQueryObjectPrivate::query() const {
    return iShared->query();
}
void QEnginioQueryObjectPrivate::setQuery(QJsonObject aQuery) {
    iShared->setQuery(aQuery);
}

int QEnginioQueryObjectPrivate::limit() const {
    return iShared->limit();
}
void QEnginioQueryObjectPrivate::setLimit(int aLimit) {
    iShared->setLimit(aLimit);
}

int QEnginioQueryObjectPrivate::offset() const {
    return iShared->offset();
}
void QEnginioQueryObjectPrivate::setOffset(int aOffset) {
    iShared->setOffset(aOffset);
}

void QEnginioQueryObjectPrivate::init() {
    Q_Q(QEnginioQueryObject);

    iConnectionQueryChanged
            = QObject::connect(iShared.data(), &QEnginioQueryShared::queryChanged,
                               q, &QEnginioQueryObject::queryChanged);
    iConnectionLimitChanged
            = QObject::connect(iShared.data(), &QEnginioQueryShared::limitChanged,
                               q, &QEnginioQueryObject::limitChanged);
    iConnectionOffsetChanged
            = QObject::connect(iShared.data(), &QEnginioQueryShared::offsetChanged,
                               q, &QEnginioQueryObject::offsetChanged);
}

void QEnginioQueryObjectPrivate::deinit() {
    QObject::disconnect(iConnectionQueryChanged);
    QObject::disconnect(iConnectionLimitChanged);
    QObject::disconnect(iConnectionOffsetChanged);
}

QSharedPointer<QEnginioQueryShared> QEnginioQueryObjectPrivate::sharedInstance() const {
    return iShared;
}
void QEnginioQueryObjectPrivate::setSharedInstance(QSharedPointer<QEnginioQueryShared> aShared) {
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
QEnginioQueryObject::QEnginioQueryObject(QObject *aParent)
    : QObject(*new QEnginioQueryObjectPrivate,aParent)
{

}

QJsonObject QEnginioQueryObject::query() const {
    Q_D(const QEnginioQueryObject);
    return d->query();
}

void QEnginioQueryObject::setQuery(QJsonObject aQuery) {
    Q_D(QEnginioQueryObject);
    d->setQuery(aQuery);
}

int QEnginioQueryObject::limit() const {
    Q_D(const QEnginioQueryObject);
    return d->limit();
}

void QEnginioQueryObject::setLimit(int aLimit) {
    Q_D(QEnginioQueryObject);
    d->setLimit(aLimit);
}

int QEnginioQueryObject::offset() const {
    Q_D(const QEnginioQueryObject);
    return d->offset();
}

void QEnginioQueryObject::setOffset(int aOffset) {
    Q_D(QEnginioQueryObject);
    d->setOffset(aOffset);
}

QT_END_NAMESPACE
