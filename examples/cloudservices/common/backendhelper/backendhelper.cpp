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

#include <Enginio/enginioclient.h>
#include <QtWidgets>
#include "backendhelper.h"
#include "ui_helperdialog.h"

const QString instanceAddressKey = QStringLiteral("instanceAddress");
const QString backendIdKey = QStringLiteral("backendId");
const QString showAgainKey = QStringLiteral("showAgain");

static QString ginstanceAddress;
static QString gBackendId;
static QTime gBackendTime;

static void backendResolve(const QString &exampleName) {
    QString fileName = QStringLiteral("QtCloudServicesExamples.conf");
    for (int i = 0; i < 4; ++i) {
        if (QFile::exists(fileName))
            break;
        fileName = fileName.prepend("../");
    }

    QFileInfo settingsFile = QFileInfo(fileName);

    QScopedPointer<QSettings> settings(settingsFile.exists()
        ? new QSettings(settingsFile.absoluteFilePath(), QSettings::IniFormat)
        : new QSettings("com.digia", "QtCloudServicesExamples"));

    settings->beginGroup(exampleName);
    gBackendId = settings->value(backendIdKey).toString();
    bool askAgain = settings->value(showAgainKey, true).toBool();

    if (askAgain) {
        if (gBackendTime.secsTo(QTime::currentTime())<10) {
            askAgain = false;
        }
    }

    if (askAgain || gBackendId.isEmpty() || ginstanceAddress.isEmpty()) {
        Ui::Dialog dialog;
        QDialog d;
        dialog.setupUi(&d);
        dialog.exampleName->setText(exampleName);

        dialog.instanceAddress->setText("https://api.engin.io");
        dialog.backendId->setText(gBackendId);

        if (d.exec() == QDialog::Accepted) {
            ginstanceAddress = dialog.instanceAddress->text();
            gBackendId = dialog.backendId->text();
            gBackendTime = QTime::currentTime();
            askAgain = !dialog.askAgain->isChecked();
            settings->setValue(instanceAddressKey, ginstanceAddress);
            settings->setValue(backendIdKey, gBackendId);
            settings->setValue(showAgainKey, askAgain);
        }
    }

    settings->endGroup();
    settings->sync();

}

QString instanceAddress(const QString &exampleName)
{
    backendResolve(exampleName);
    return ginstanceAddress;
}

QString backendId(const QString &exampleName)
{
    backendResolve(exampleName);
    return gBackendId;
}

