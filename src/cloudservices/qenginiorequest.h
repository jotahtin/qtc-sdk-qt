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

#ifndef QCLOUDSERVICES_QENGINIOREQUEST_H
#define QCLOUDSERVICES_QENGINIOREQUEST_H

#include <QtCloudServices/qrestrequest.h>

QT_BEGIN_NAMESPACE

class QEnginioOperation;
class QEnginioCollection;
class QEnginioRequestObject;
class QTCLOUDSERVICES_EXPORT QEnginioRequest : public QRestRequest {
    friend class QEnginioOperation;
protected:
    QEnginioRequest(QEnginioRequestObject *aObject);
public:
    QEnginioRequest();
    QEnginioRequest(QtCloudServices::RESTOperation aOperation, QString aPath);
    QEnginioRequest(const QEnginioRequest &aOther);

    QEnginioRequest& operator=(const QEnginioRequest &aOther);

    QEnginioCollection enginioCollection() const Q_REQUIRED_RESULT;

    QEnginioRequest &then(std::function<void(QEnginioOperation)> aCallback);
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOOPERATION_H */
