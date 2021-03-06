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

#ifndef QCLOUDSERVICES_QENGINIOOPERATION_OBJECT_H
#define QCLOUDSERVICES_QENGINIOOPERATION_OBJECT_H

#include <QList>
#include <QJsonObject>

#include <QtCloudServices/qrestoperationobject.h>
#include <QtCloudServices/qenginiorequestobject.h>
#include <QtCloudServices/qenginioobject.h>

QT_BEGIN_NAMESPACE

class QEnginioConnectionObject;
class QEnginioOperationObjectPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioOperationObject : public QRestOperationObject {
    Q_OBJECT
public:
    Q_DECLARE_PRIVATE(QEnginioOperationObject)
    Q_PROPERTY(int backendStatus READ backendStatus NOTIFY dataChanged)
    Q_PROPERTY(QString requestId READ requestId CONSTANT)
    Q_PROPERTY(int resultObjectCount READ resultObjectCount NOTIFY dataChanged)
private:
    Q_DISABLE_COPY(QEnginioOperationObject)
public:
    typedef std::function<void(QEnginioOperationObject *aOperation)> Callback;
public:
    QEnginioOperationObject(QObject *aParent=0);
    ~QEnginioOperationObject();

    int resultObjectCount() const Q_REQUIRED_RESULT;    
    QEnginioObject resultObject() const Q_REQUIRED_RESULT;
    QList<QEnginioObject> resultObjects() const Q_REQUIRED_RESULT;
Q_SIGNALS:
    void dataChanged();
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOOPERATION_OBJECT_H */
