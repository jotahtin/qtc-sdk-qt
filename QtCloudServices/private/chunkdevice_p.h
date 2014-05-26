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

#include <QtCore/qiodevice.h>

QT_BEGIN_NAMESPACE

/*!
  \brief The ChunkDevice class is a simple QIODevice representing a part of another QIODevice

  Used for chunked upload so that we can pass a QIODevice to QNetworkAccessManager.

  \internal
*/

class ChunkDevice : public QIODevice {
    Q_OBJECT

public:
    ChunkDevice(QIODevice *source, qint64 startPos, qint64 chunkSize)
        : _source(source), _startPos(startPos), _chunkSize(chunkSize)
    {
        Q_ASSERT(source->isOpen());
        Q_ASSERT(source->isReadable());
        Q_ASSERT(!source->isSequential());
        source->seek(startPos);
    }

    bool isSequential() const Q_DECL_OVERRIDE
    {
        return _source->isSequential();
    }

    qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE {
        return _source->read(data, maxlen);
    }

    qint64 writeData(const char*, qint64) Q_DECL_OVERRIDE {
        return -1;
    }

    qint64 size() const Q_DECL_OVERRIDE
    {
        return qMin(_source->size() - _startPos, _chunkSize);
    }

    qint64 pos() const Q_DECL_OVERRIDE
    {
        return _source->pos() - _startPos;
    }

    bool seek(qint64 pos) Q_DECL_OVERRIDE {
        QIODevice::seek(pos);
        return _source->seek(pos + _startPos);
    }

    qint64 bytesAvailable() const Q_DECL_OVERRIDE
    {
        return qMin(_source->bytesAvailable(), _chunkSize + (_startPos - _source->pos()));
    }

private:
    QIODevice *_source;
    qint64 _startPos;
    qint64 _chunkSize;
};

QT_END_NAMESPACE
