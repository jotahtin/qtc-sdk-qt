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

#include "QtCloudServices/private/qenginioquery_p.h"

QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/

QEnginioQueryPrivate::QEnginioQueryPrivate()
    : iLimit(100), iOffset(0)
{

}

/*
** Public Interface
*/
QEnginioQuery::QEnginioQuery(QObject *aParent)
    : QCloudServicesObject(*new QEnginioQueryPrivate(), aParent)
{

}
QEnginioQuery::QEnginioQuery(const QJsonObject &aQuery, QObject *aParent)
    : QCloudServicesObject(*new QEnginioQueryPrivate(), aParent)
{
    d<QEnginioQuery>()->iQuery = aQuery;
}
QEnginioQuery::QEnginioQuery(const QEnginioQuery &aOther)
    : QCloudServicesObject(*new QEnginioQueryPrivate())
{
    *this = aOther;
}

QEnginioQuery& QEnginioQuery::operator=(const QEnginioQuery &aOther)
{
    QEnginioQuery::dvar pimpl = d<QEnginioQuery>();
    QEnginioQuery::dvar other;

    other = aOther.d<QEnginioQuery>();

    if (other) {
        pimpl->iQuery = other->iQuery;
        pimpl->iLimit = other->iLimit;
        pimpl->iOffset = other->iOffset;
    }

    return *this;
}

QEnginioQuery &QEnginioQuery::limit(int aLimit)
{
    d<QEnginioQuery>()->iLimit = aLimit;
    return *this;
}

QEnginioQuery &QEnginioQuery::offset(int aOffset)
{
    d<QEnginioQuery>()->iOffset = aOffset;
    return *this;
}

const QJsonObject &QEnginioQuery::query() const
{
    return d<const QEnginioQuery>()->iQuery;
}

int QEnginioQuery::limit() const
{
    return d<const QEnginioQuery>()->iLimit;
}
int QEnginioQuery::offset() const
{
    return d<const QEnginioQuery>()->iOffset;
}

QT_END_NAMESPACE
