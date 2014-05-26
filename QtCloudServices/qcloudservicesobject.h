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

#include <QObject>

#include <QtCloudServices/qtcloudservices_global.h>

#if QTCLOUDSERVICES_USE_QOBJECT_PRIVATE
# define QTC_DECLARE_PRIVATE(Class) Q_DECLARE_PRIVATE(Class)
# define QTC_DECLARE_PUBLIC(Class) Q_DECLARE_PUBLIC(Class)
# define QTC_D(Class) Q_D(Class)
# define QTC_Q(Class) Q_Q(Class)
# define QTC_Q_PTR(INSTANCE) (INSTANCE)->q_ptr
# define QTC_D_PTR(INSTANCE) (INSTANCE)->d_ptr
# define QTC_Q_FUNC(INSTANCE) (INSTANCE)->q_func()
# define QTC_D_FUNC(INSTANCE) (INSTANCE)->d_func()
#else
# define QTC_DECLARE_PRIVATE(Class) \
                inline Class##Private* priv_func() { \
					return reinterpret_cast<Class##Private *>(iPIMPL); \
				} \
                inline const Class##Private* priv_func() const { \
					return reinterpret_cast<const Class##Private *>(iPIMPL); \
				} \
                friend class Class##Private;
# define QTC_DECLARE_PUBLIC(Class) \
                inline Class* pub_func() { return static_cast<Class *>(iInterface); } \
                inline const Class* pub_func() const { \
					return static_cast<const Class *>(iInterface); \
				} \
        private: \
                friend class Class;
# define QTC_D(Class) Class##Private * const d = this->priv_func()
# define QTC_Q(Class) Class * const q = this->pub_func()
# define QTC_Q_PTR(INSTANCE) (INSTANCE)->iInterface
# define QTC_D_PTR(INSTANCE) (INSTANCE)->iPIMPL
# define QTC_Q_FUNC(INSTANCE) (INSTANCE)->pub_func()
# define QTC_D_FUNC(INSTANCE) (INSTANCE)->priv_func()
#endif

QT_BEGIN_NAMESPACE

class QCloudServicesObjectPrivate;
class QTCLOUDSERVICES_EXPORT QCloudServicesObject : public QObject {
    Q_OBJECT
private:
    QCloudServicesObject();
    Q_DISABLE_COPY(QCloudServicesObject)
protected:
    QCloudServicesObject(QCloudServicesObjectPrivate &dd, QObject *parent = 0);
public:
    virtual ~QCloudServicesObject();
protected:
#if !QTCLOUDSERVICES_USE_QOBJECT_PRIVATE
    QCloudServicesObjectPrivate *iPIMPL;
#endif
};

QT_END_NAMESPACE

#endif /* QCLOUDSERVICES_QCLOUDSERVICES_OBJECT_H */


