/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "todosmodel.h"

#include <QtCore/qjsonvalue.h>
#include <QtCore/qjsonobject.h>
#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>

#include <Enginio/enginioreply.h>

TodosModel::TodosModel(QObject *parent)
    : QEnginioModel(parent)
{}

QVariant TodosModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && section == 0 && role == Qt::DisplayRole) {
        return QStringLiteral("Todo List");
    }

    return QEnginioModel::headerData(section, orientation, role);
}

#if USE_ROLE_NAME
//![data]
QVariant TodosModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::FontRole) {
        bool completed = QEnginioModel::data(index, CompletedRole).value<QJsonValue>().toBool();
        QFont font;
        font.setPointSize(20);
        font.setStrikeOut(completed);
        return font;
    }

    if (role == Qt::TextColorRole) {
        bool completed = QEnginioModel::data(index, CompletedRole).value<QJsonValue>().toBool();
        return completed ? QColor("#999") : QColor("#333");
    }

    if (role == CompletedRole) {
        return QEnginioModel::data(index, CompletedRole).value<QJsonValue>().toBool();
    }

    // fallback to base class
    return QEnginioModel::data(index, role);
}
//![data]
//![roleNames]
QHash<int, QByteArray> TodosModel::roleNames() const
{
    QHash<int, QByteArray> roles = QEnginioModel::roleNames();
    roles.insert(TitleRole, "title");
    roles.insert(Qt::DisplayRole, "title");
    roles.insert(Qt::EditRole, "title");
    roles.insert(CompletedRole, "completed");
    return roles;
}
//![roleNames]
#else
QVariant TodosModel::enginioData(const QEnginioObject &aEnginioObject,
                                 const QModelIndex &aIndex, int aRole) const
{
    if (aRole == Qt::FontRole) {
        bool completed = aEnginioObject["completed"].toBool();
        QFont font;
        font.setPointSize(20);
        font.setStrikeOut(completed);
        return font;
    }

    if (aRole == Qt::TextColorRole) {
        bool completed = aEnginioObject["completed"].toBool();
        return completed ? QColor("#999") : QColor("#333");
    }

    if (aRole == TodosModel::CompletedRole) {
        return aEnginioObject["completed"].toBool();
    }

    if (aRole == Qt::DisplayRole) {
        return aEnginioObject["title"];
    }

    return QVariant();
}
bool TodosModel::setEnginioData(QEnginioObject aEnginioObject,
                                const QModelIndex &aIndex, const QVariant &aValue, int aRole)
{
    if (!aEnginioObject.isPersistent()) {
        return false;
    }

    if (aRole == Qt::EditRole) {
        aEnginioObject["title"] = aValue.toString();
        aEnginioObject.save();
        return true;
    }

    if (aRole == TodosModel::CompletedRole) {
        aEnginioObject["completed"] = aValue.toBool();
        aEnginioObject.save();
        return true;
    }

    return false;
}



#endif