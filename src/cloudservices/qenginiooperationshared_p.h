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

#ifndef QCLOUDSERVICES_QENGINIOOPERATION_SHARED_P_H
#define QCLOUDSERVICES_QENGINIOOPERATION_SHARED_P_H

#include <QtCloudServices/private/qrestoperationshared_p.h>

#include <QtCloudServices/private/qenginiocollectionshared_p.h>

QT_BEGIN_NAMESPACE

class QEnginioOperationShared : public QRestOperationShared {
    Q_OBJECT
    Q_DISABLE_COPY(QEnginioOperationShared)
public:
    QEnginioOperationPrivate();
    QEnginioOperationPrivate(const QEnginioConnection &aEnginioConnection,
                             const QEnginioRequest &aRequest);
    ~QEnginioOperationPrivate();
public:
    QSharedPointer<QEnginioRequestShared> enginioRequest() const Q_REQUIRED_RESULT;

    int backendStatus() const Q_REQUIRED_RESULT;
    QString requestId() const Q_REQUIRED_RESULT;
    QString errorString() const Q_REQUIRED_RESULT;
    QtCloudServices::ErrorType errorType() const Q_REQUIRED_RESULT;

    int resultObjectCount() const Q_REQUIRED_RESULT;
    QEnginioObject resultObject() const Q_REQUIRED_RESULT;
    QList<QEnginioObject> resultObjects() const Q_REQUIRED_RESULT;

#ifndef QT_NO_DEBUG_STREAM
    virtual void dumpDebugInfo(QDebug &d) const;
#endif
protected:
    void setEnginioRequest(const QEnginioRequest &aEnginioRequest);
    virtual void operationFinished();
private:
    QList<QEnginioObject> iResultObjects;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOOPERATION_SHARED_P_H */
