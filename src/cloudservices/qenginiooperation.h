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

#ifndef QCLOUDSERVICES_QENGINIOOPERATION_H
#define QCLOUDSERVICES_QENGINIOOPERATION_H

#include <QtCloudServices/qrestoperation.h>
#include <QtCloudServices/qenginiorequest.h>
#include <QtCloudServices/qenginioobject.h>
#include <QtCloudServices/qenginioquery.h>

QT_BEGIN_NAMESPACE

class QEnginioConnection;
class QEnginioOperationObject;
class QTCLOUDSERVICES_EXPORT QEnginioOperation : public QRestOperation {
    friend class QEnginioCollection;
    friend class QEnginioConnection;
public:
    typedef std::function<void(QEnginioOperation aEnginioOperation)> Callback;
public:
    explicit QEnginioOperation(QEnginioOperationObject *aEnginioOperationObject);
public:
    // Constructors
    QEnginioOperation();
    QEnginioOperation(const QEnginioOperation &aOther);

    // Assignment
    QEnginioOperation& operator=(const QEnginioOperation &aOther);

    // connection & request
    QEnginioConnection connection() const Q_REQUIRED_RESULT;
    QEnginioRequest request() const Q_REQUIRED_RESULT;

    int resultObjectCount() const Q_REQUIRED_RESULT;
    QEnginioObject resultObject() const Q_REQUIRED_RESULT;
    QList<QEnginioObject> resultObjects() const Q_REQUIRED_RESULT;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOOPERATION_H */
