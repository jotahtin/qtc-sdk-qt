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

#ifndef QCLOUDSERVICES_QRESTCONNECTION_SHARED_P_H
#define QCLOUDSERVICES_QRESTCONNECTION_SHARED_P_H

#include <QtCore/qthreadstorage.h>
#include <QtCore/qpointer.h>
#include <QtCore/qurl.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qurlquery.h>
#include <QtCore/qfile.h>
#include <QtCore/qmimedatabase.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qbuffer.h>
#include <QtCore/qlinkedlist.h>
#include <QtCore/quuid.h>

#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qhttpmultipart.h>

#include <QtCloudServices/private/qrestrequestshared_p.h>
#include <QtCloudServices/private/qrestoperationshared_p.h>

QT_BEGIN_NAMESPACE

class QRestEndpointShared;
class QRestConnectionShared : public QObject {
    Q_OBJECT
    friend class QRestOperationShared;
private:
    Q_DISABLE_COPY(QRestConnectionShared)
public:
    class ReplyFinishedFunctor {
    public:
        ReplyFinishedFunctor(QSharedPointer<QRestConnectionShared> aConnection);
        void operator ()(QNetworkReply *aNetworkReply);
    private:
        QWeakPointer<QRestConnectionShared> iConnection;
    };
public:
    QRestConnectionShared(QSharedPointer<QRestEndpointShared> aRestEndpointShared);
    virtual ~QRestConnectionShared();

    virtual bool isValid() const;

    QSharedPointer<QNetworkAccessManager> networkManager() const Q_REQUIRED_RESULT;

    QSharedPointer<QRestOperationShared> restRequest(QSharedPointer<QRestConnectionShared> aSelf,
                                                     QSharedPointer<QRestRequestShared> aRequest);

    void replyFinished(QNetworkReply *aNetworkReply);
protected:
    void registerReply(QNetworkReply *aNetworkReply,
                       QSharedPointer<QRestOperationShared> aOperation);
    void unregisterReply(QNetworkReply *aNetworkReply);
protected:
    virtual QSharedPointer<QRestOperationShared> buildOperationInstance(QSharedPointer<QRestConnectionShared> aSelf,
                                                                        QSharedPointer<QRestRequestShared> aRequest);

    virtual bool prepareRequest(QNetworkRequest &aRequest,
                                const QString &aPath,
                                const QUrlQuery &aQuery,
                                const QJsonObject &aExtraHeaders);

private:
    QSharedPointer<QRestEndpointShared> iRestEndpoint;
    QSharedPointer<QNetworkAccessManager> iNetworkManager;
    QMetaObject::Connection iNetworkManagerConnection;
    QMap<QNetworkReply*, QSharedPointer<QRestOperationShared> > iReplyOperationMap;
private:
    static QThreadStorage < QWeakPointer<QNetworkAccessManager> > gNetworkManager;

};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QRESTCONNECTION_SHARED_P_H */
