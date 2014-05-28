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

#include "QtCloudServices/private/qcloudservicesobject_p.h"

QT_BEGIN_NAMESPACE

QCloudServicesObjectPrivate::QCloudServicesObjectPrivate()
{

}
QCloudServicesObjectPrivate::~QCloudServicesObjectPrivate()
{

}

#if 0
void QCloudServicesObjectPrivate::setPIMPL(QCloudServicesObject::dvar aPIMPL)
{
    Q_ASSERT(iInterface);
//   if (iInterface) {
    aPIMPL->iInterface = iInterface;
    iInterface->iPIMPL = aPIMPL;
    /*
    } else {
    qCritical() << "setPIMPL failed.. no interface";
    }
    */
}
#endif

void QCloudServicesObjectPrivate::addInterface(QCloudServicesObject *aInterface)
{
    QMutexLocker locker(&iLock);
    iInterfaces.append(aInterface);
}
void QCloudServicesObjectPrivate::removeInterface(QCloudServicesObject *aInterface)
{
    QMutexLocker locker(&iLock);
    iInterfaces.removeAll(aInterface);
}

/*
** Public Interface
*/
QCloudServicesObject::QCloudServicesObject(QObject *aParent)
    : QObject(aParent)
{
}

/*
QCloudServicesObject::QCloudServicesObject(QCloudServicesObjectPrivate & dd, QObject * parent)
    : QObject(parent),
      iPIMPL(&dd)
{
    if (iPIMPL) {
        iPIMPL->addInterface(this);
    }
}
*/
QCloudServicesObject::QCloudServicesObject(QCloudServicesObject::dvar aPIMPL, QObject *parent)
    : QObject(parent)
{
    setPIMPL(aPIMPL);
}

QCloudServicesObject::~QCloudServicesObject()
{
    if (iPIMPL) {
        iPIMPL->removeInterface(this);
    }
}

bool QCloudServicesObject::isNull() const
{
    return iPIMPL.get() == NULL;
}
bool QCloudServicesObject::isValid() const
{
    return !isNull();
}

void QCloudServicesObject::setPIMPL(QCloudServicesObject::dvar aPIMPL)
{
    if (iPIMPL) {
        iPIMPL->removeInterface(this);
    }

    iPIMPL = aPIMPL;

    if (iPIMPL) {
        iPIMPL->addInterface(this);
    }
}

QT_END_NAMESPACE
