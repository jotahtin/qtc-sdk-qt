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

#ifndef QCLOUDSERVICES_QENGINIOOBJECT_H
#define QCLOUDSERVICES_QENGINIOOBJECT_H

#include <QTime>
#include <QJsonValueRef>

#include <QtCloudServices/qtcloudservices.h>
#include <QtCloudServices/qcloudservicesobject.h>

QT_BEGIN_NAMESPACE

class QEnginioUser;
class QEnginioObjectPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioObject : public QCloudServicesObject {
    Q_OBJECT
    Q_PROPERTY(QString objectId READ objectId NOTIFY objectChanged)
    QTC_DECLARE_PRIVATE(QEnginioObject)
protected:
    QEnginioObject(QEnginioObject::dvar aPIMPL, QObject *aParent = 0);
public:
    QEnginioObject(QObject *aParent = 0);
    QEnginioObject(const QEnginioObject &aOther);
    QEnginioObject(const QJsonObject &aJsonObject);

    QEnginioObject& operator=(const QEnginioObject &aOther);

    virtual bool isValid() const Q_REQUIRED_RESULT;
    bool isPersistent() const Q_REQUIRED_RESULT;
    bool isModified() const Q_REQUIRED_RESULT;

    QEnginioObject& insert(const QString &aKey, const QJsonValue &aValue);
    QEnginioObject& remove(const QString &aKey);
    bool contains(const QString &aKey) const Q_REQUIRED_RESULT;
    QJsonValue value(const QString &aKey) const Q_REQUIRED_RESULT;
    QJsonValue operator[](const QString &aKey) const Q_REQUIRED_RESULT;
    QJsonValueRef operator[](const QString &aKey) Q_REQUIRED_RESULT;

    const QJsonObject jsonObject() const Q_REQUIRED_RESULT;

    const QString objectId() const Q_REQUIRED_RESULT;
    const QTime createAt() const Q_REQUIRED_RESULT;
    const QEnginioUser creator() const Q_REQUIRED_RESULT;
    const QString objectType() const Q_REQUIRED_RESULT;
    const QTime updatedAt() const Q_REQUIRED_RESULT;
    const QEnginioUser updater() const Q_REQUIRED_RESULT;

    void save();
protected:
    virtual void lazyInitialization();
    virtual void setPIMPL(QCloudServicesObject::dvar aPIMPL);
Q_SIGNALS:
    void objectChanged();
    void operationFailed(QString);
};

Q_DECLARE_TYPEINFO(QEnginioObject, Q_COMPLEX_TYPE);

#ifndef QT_NO_DEBUG_STREAM
QTCLOUDSERVICES_EXPORT QDebug operator<<(QDebug d, QEnginioObject aObject);
#endif

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QEnginioObject);

#endif /* QCLOUDSERVICES_QENGINIOOBJECT_H */
