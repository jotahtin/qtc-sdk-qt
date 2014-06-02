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

#include "QtCloudServices/qenginiouser.h"
#include "QtCloudServices/private/qenginiouserobject_p.h"

QT_BEGIN_NAMESPACE

QEnginioUser::QEnginioUser(QEnginioUserObject *aObject)
    : QEnginioObject(aObject)
{

}

QEnginioUser::QEnginioUser()
    : QEnginioObject(new QEnginioUserObject)
{

}

QEnginioUser::QEnginioUser(const QJsonObject &aJsonObject)
    : QEnginioObject(new QEnginioUserObject(aJsonObject))
{

}

QEnginioUser::QEnginioUser(const QEnginioUser &aOther)
    : QEnginioObject(new QEnginioUserObject)
{
    object()->setSharedInstanceFrom(aOther.object());
}

QEnginioUser& QEnginioUser::operator=(const QEnginioUser &aOther) {
    object()->setSharedInstanceFrom(aOther.object());
    return *this;
}

QString QEnginioUser::username() const {
    const QEnginioUserObject *obj;
    obj = reinterpret_cast<const QEnginioUserObject *>(object());
    return obj->username();
}

QString QEnginioUser::email() const {
    const QEnginioUserObject *obj;
    obj = reinterpret_cast<const QEnginioUserObject *>(object());
    return obj->email();
}

QString QEnginioUser::firstName() const {
    const QEnginioUserObject *obj;
    obj = reinterpret_cast<const QEnginioUserObject *>(object());
    return obj->firstName();
}

QString QEnginioUser::lastName() const {
    const QEnginioUserObject *obj;
    obj = reinterpret_cast<const QEnginioUserObject *>(object());
    return obj->lastName();
}

QT_END_NAMESPACE

