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
** Shared Implementation
*/
QEnginioUserObject::QEnginioUserObject(QSharedPointer<QEnginioCollectionObject> aCollection)
    : QEnginioObjectObject(aCollection)
{

}
QEnginioUserObject::~QEnginioUserObject()
{

}

const QString QEnginioUserObject::username() const
{
    return iUsername;
}
const QString QEnginioUserObject::email() const
{
    return iEMail;
}
const QString QEnginioUserObject::firstName() const
{
    return iFirstName;
}
const QString QEnginioUserObject::lastName() const
{
    return iLastName;
}

QSharedPointer<QEnginioUserObject> QEnginioUserObject::get(QSharedPointer<QEnginioCollectionObject> aCollection)
{
    QSharedPointer<QEnginioUserObject> usr;
    usr = QSharedPointer<QEnginioUserObject>(new QEnginioUserObject(aCollection));


    return usr;
}




/*
** Private Implementation
*/

QEnginioUserPrivate::QEnginioUserPrivate()
{

}


const QString QEnginioUserPrivate::username() const
{
    if (iObject) {
        return iObject->username();
    }

    return QString();
}

const QString QEnginioUserPrivate::email() const
{
    if (iObject) {
        return iObject->email();
    }

    return QString();
}

const QString QEnginioUserPrivate::firstName() const
{
    if (iObject) {
        return iObject->firstName();
    }

    return QString();
}

const QString QEnginioUserPrivate::lastName() const
{
    if (iObject) {
        return iObject->lastName();
    }

    return QString();
}

QSharedPointer<QEnginioUserObject> QEnginioUserPrivate::enginioUserObject() const
{
    return iObject;
}

void QEnginioUserPrivate::setEnginioUserObject(QSharedPointer<QEnginioUserObject> aObject)
{
    iObject = aObject;
}


/*
** Public Interface
*/
QEnginioUser::QEnginioUser(QObject *aParent)
    : QEnginioObject(*new QEnginioUserPrivate(), aParent)
{

}
QEnginioUser::QEnginioUser(const QEnginioUser &aOther)
    : QEnginioObject(*new QEnginioUserPrivate())
{
    *this = aOther;
}

QEnginioUser& QEnginioUser::operator=(const QEnginioUser &aOther)
{
    QTC_D(QEnginioUser);
    QEnginioUserPrivate *other;

    other = reinterpret_cast<QEnginioUserPrivate *>(QTC_D_PTR(&aOther));

    if (other) {
        d->setEnginioUserObject(other->enginioUserObject());
    }

    return *this;
}

const QString QEnginioUser::username() const
{
    QTC_D(const QEnginioUser);
    return d->username();
}

const QString QEnginioUser::email() const
{
    QTC_D(const QEnginioUser);
    return d->email();
}

const QString QEnginioUser::firstName() const
{
    QTC_D(const QEnginioUser);
    return d->firstName();
}

const QString QEnginioUser::lastName() const
{
    QTC_D(const QEnginioUser);
    return d->lastName();
}

QT_END_NAMESPACE

