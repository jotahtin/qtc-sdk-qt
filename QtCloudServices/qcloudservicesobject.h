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

#ifndef QCLOUDSERVICES_QCLOUDSERVICES_OBJECT_H
#define QCLOUDSERVICES_QCLOUDSERVICES_OBJECT_H

#include <memory>

#include <QObject>

#include <QtCloudServices/qtcloudservices_global.h>

#define QTC_DECLARE_PRIVATE(Class) \
	public: \
	typedef std::shared_ptr<Class##Private> dvar; \
	typedef std::weak_ptr<Class##Private> wvar; \
	typedef Class##Private private_type; \
	friend class Class##Private;


#define QTC_DECLARE_PUBLIC(Class) \
	public: \
	typedef Class qvar; \
	friend class Class;

QT_BEGIN_NAMESPACE

class QCloudServicesObjectPrivate;
class QTCLOUDSERVICES_EXPORT QCloudServicesObject : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(QCloudServicesObject)
    QTC_DECLARE_PRIVATE(QCloudServicesObject)
protected:
    QCloudServicesObject(QObject *aParent = 0);
    // QCloudServicesObject(QCloudServicesObjectPrivate &dd, QObject *aParent = 0); // DEPRECATE
    QCloudServicesObject(QCloudServicesObject::dvar aPIMPL, QObject *aParent = 0);
public:
    virtual ~QCloudServicesObject();

    bool isNull() const Q_REQUIRED_RESULT;
    virtual bool isValid() const Q_REQUIRED_RESULT;
public:
    template<class T>
    std::shared_ptr<typename T::private_type> d()
    {
        Q_ASSERT(iPIMPL);
        return std::dynamic_pointer_cast <typename T::private_type>(iPIMPL);
    }
    template<class T>
    const std::shared_ptr<typename T::private_type> d() const
    {
        Q_ASSERT(iPIMPL);
        return std::dynamic_pointer_cast <typename T::private_type>(iPIMPL);
    }
protected:
    virtual void setPIMPL(QCloudServicesObject::dvar aPIMPL);
private:
    QCloudServicesObject::dvar iPIMPL;
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QCLOUDSERVICES_OBJECT_H */


