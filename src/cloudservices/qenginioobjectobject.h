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

#ifndef QCLOUDSERVICES_QENGINIOOBJECT_OBJECT_H
#define QCLOUDSERVICES_QENGINIOOBJECT_OBJECT_H

#include <QTime>
#include <QJsonValueRef>

#include <QtCloudServices/qtcloudservices_global.h>

QT_BEGIN_NAMESPACE

class QEnginioUserObject;
class QEnginioOperationObject;
class QEnginioObjectObjectPrivate;
class QTCLOUDSERVICES_EXPORT QEnginioObjectObject : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QEnginioObjectObject)

    Q_PROPERTY(QString objectId READ objectId NOTIFY dataChanged)
    Q_PROPERTY(QString objectType READ objectType NOTIFY dataChanged)
    Q_PROPERTY(QTime createAt READ createAt NOTIFY dataChanged)
    Q_PROPERTY(QEnginioUser creator READ creator NOTIFY dataChanged)
    Q_PROPERTY(QTime updatedAt READ objectId NOTIFY dataChanged)
    Q_PROPERTY(QEnginioUser updater READ objectId NOTIFY dataChanged)
private:
    Q_DISABLE_COPY(QEnginioObjectObject)
public:
    QEnginioObjectObject(QObject *aParent = 0);
    QEnginioObjectObject(const QJsonObject &aJsonObject,QObject *aParent = 0);

    bool isValid() const Q_REQUIRED_RESULT;
    bool isPersistent() const Q_REQUIRED_RESULT;
    bool isModified() const Q_REQUIRED_RESULT;

    void insert(const QString &aKey, const QJsonValue &aValue);
    void remove(const QString &aKey);
    bool contains(const QString &aKey) const Q_REQUIRED_RESULT;
    QJsonValue value(const QString &aKey) const Q_REQUIRED_RESULT;
    QJsonValueRef valueRef(const QString &aKey) Q_REQUIRED_RESULT;

    QJsonObject jsonObject() const Q_REQUIRED_RESULT;

    QString objectId() const Q_REQUIRED_RESULT;
    QString objectType() const Q_REQUIRED_RESULT;
    QTime createAt() const Q_REQUIRED_RESULT;
    QEnginioUserObject *creator() const Q_REQUIRED_RESULT;
    QTime updatedAt() const Q_REQUIRED_RESULT;
    QEnginioUserObject *updater() const Q_REQUIRED_RESULT;

    QEnginioOperationObject *save();
public:
    void setSharedInstanceFrom(const QEnginioObjectObject *aOther);
Q_SIGNALS:
    void dataChanged();
    void operationFailed(QString);
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QENGINIOOBJECT_OBJECT_H */
