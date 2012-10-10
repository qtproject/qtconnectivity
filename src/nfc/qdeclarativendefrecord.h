/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtNfc module of the Qt Toolkit.
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

#ifndef QDECLARATIVENDEFRECORD_P_H
#define QDECLARATIVENDEFRECORD_P_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtNfc/QNdefRecord>

QTNFC_BEGIN_NAMESPACE

class QDeclarativeNdefRecordPrivate;

class Q_NFC_EXPORT QDeclarativeNdefRecord : public QObject
{
    Q_OBJECT

    Q_DECLARE_PRIVATE(QDeclarativeNdefRecord)

    Q_PROPERTY(QString recordType READ recordType WRITE setRecordType NOTIFY recordTypeChanged)

public:
    explicit QDeclarativeNdefRecord(QObject *parent = 0);
    explicit QDeclarativeNdefRecord(const QNdefRecord &record, QObject *parent = 0);

    QString recordType() const;
    void setRecordType(const QString &t);

    QNdefRecord record() const;
    void setRecord(const QNdefRecord &record);

signals:
    void recordTypeChanged();

private:
    QDeclarativeNdefRecordPrivate *d_ptr;
};

void Q_NFC_EXPORT qRegisterNdefRecordTypeHelper(const QMetaObject *metaObject,
                                                         QNdefRecord::TypeNameFormat typeNameFormat,
                                                         const QByteArray &type);

Q_NFC_EXPORT QDeclarativeNdefRecord *qNewDeclarativeNdefRecordForNdefRecord(const QNdefRecord &record);

template<typename T>
bool qRegisterNdefRecordType(QNdefRecord::TypeNameFormat typeNameFormat, const QByteArray &type)
{
    qRegisterNdefRecordTypeHelper(&T::staticMetaObject, typeNameFormat, type);
    return true;
}

#define Q_DECLARE_NDEFRECORD(className, typeNameFormat, type) \
static bool _q_##className##_registered = qRegisterNdefRecordType<className>(typeNameFormat, type);

QTNFC_END_NAMESPACE

#endif // QDECLARATIVENDEFRECORD_P_H
