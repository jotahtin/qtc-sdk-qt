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

#include "QtCloudServices/private/qenginiouser_p.h"

QT_BEGIN_NAMESPACE

/*
** Private Implementation
*/
QEnginioUserPrivate::QEnginioUserPrivate()
{

}
QEnginioUserPrivate::QEnginioUserPrivate(const QJsonObject &aJsonObject)
    : QEnginioObjectPrivate(aJsonObject)
{

}


const QString QEnginioUserPrivate::username() const
{
    return iUsername;
}

const QString QEnginioUserPrivate::email() const
{
    return iEMail;
}

const QString QEnginioUserPrivate::firstName() const
{
    return iFirstName;
}

const QString QEnginioUserPrivate::lastName() const
{
    return iLastName;
}

/*
** Public Interface
*/
QEnginioUser::QEnginioUser(QObject *aParent)
    : QEnginioObject(aParent)
{

}
QEnginioUser::QEnginioUser(const QEnginioUser &aOther)
    : QEnginioObject(aOther.d<QEnginioUser>())
{
}

QEnginioUser::QEnginioUser(const QJsonObject &aJsonObject)
    : QEnginioObject(QEnginioUser::dvar(new QEnginioUserPrivate(aJsonObject)))
{

}

QEnginioUser& QEnginioUser::operator=(const QEnginioUser &aOther)
{
    setPIMPL(aOther.d<QEnginioUser>());
    return *this;
}

const QString QEnginioUser::username() const
{
    if (isNull()) {
        return QString();
    }

    return d<const QEnginioUser>()->username();
}

const QString QEnginioUser::email() const
{
    if (isNull()) {
        return QString();
    }

    return d<const QEnginioUser>()->email();
}

const QString QEnginioUser::firstName() const
{
    if (isNull()) {
        return QString();
    }

    return d<const QEnginioUser>()->firstName();
}

const QString QEnginioUser::lastName() const
{
    if (isNull()) {
        return QString();
    }

    return d<const QEnginioUser>()->lastName();
}

void QEnginioUser::lazyInitialization()
{
    if (isNull()) {
        setPIMPL(QEnginioUser::dvar(new QEnginioUserPrivate));
    }
}

QT_END_NAMESPACE

