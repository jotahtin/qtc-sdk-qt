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

#ifndef QCLOUDSERVICES_QRESTOPERATION_OBJECT_P_H
#define QCLOUDSERVICES_QRESTOPERATION_OBJECT_P_H

#include <QtCore/private/qobject_p.h>

#include <QtCloudServices/qrestoperationobject.h>

/*
#include <functional>
#include <QList>

#include <QtCore/qjsonobject.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qtypeinfo.h>
#include <QtCore/qmetatype.h>
#include <QtNetwork/qnetworkreply.h>

#include <QtCloudServices/qtcloudservices.h>
#include <QtCloudServices/qenginioobject.h>
*/

QT_BEGIN_NAMESPACE

class QRestConnectionShared;
class QRestRequestShared;
class QRestOperationShared;
class QTCLOUDSERVICES_EXPORT QRestOperationObjectPrivate : public QObjectPrivate {
    Q_DECLARE_PUBLIC(QRestOperationObject)
private:
    Q_DISABLE_COPY(QRestOperationObjectPrivate)
public:
    QRestOperationObjectPrivate();
    ~QRestOperationObjectPrivate();

    virtual QRestConnectionObject *restConnection() const Q_REQUIRED_RESULT;
    virtual QRestRequestObject *restRequest() const Q_REQUIRED_RESULT;

    bool isValid() const Q_REQUIRED_RESULT;
    bool isError() const Q_REQUIRED_RESULT;
    bool isFinished() const Q_REQUIRED_RESULT;

    QtCloudServices::ErrorType errorType() const Q_REQUIRED_RESULT;
    QNetworkReply::NetworkError networkError() const Q_REQUIRED_RESULT;
    QString errorString() const Q_REQUIRED_RESULT;

    QJsonObject resultJson() const Q_REQUIRED_RESULT;
    QByteArray resultBytes() const Q_REQUIRED_RESULT;

#ifndef QT_NO_DEBUG_STREAM
    void dumpDebugInfo() const;
#endif
protected:
    virtual void init();
    virtual void deinit();
public:
    QSharedPointer<QRestOperationShared> sharedInstance() const;
    void setSharedInstance(QSharedPointer<QRestOperationShared> aShared);
private:
    QSharedPointer<QRestOperationShared> iShared;

    QMetaObject::Connection iConnectionDataChanged;
    QMetaObject::Connection iConnectionFinished;
    QMetaObject::Connection iConnectionProgress;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTOPERATION_OBJECT_P_H */
