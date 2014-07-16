/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
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

#ifndef QLOWENERGYSERVICEPRIVATE_P_H
#define QLOWENERGYSERVICEPRIVATE_P_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtBluetooth/qbluetooth.h>
#include <QtBluetooth/QLowEnergyService>
#include <QtBluetooth/QLowEnergyCharacteristic>

#include "qlowenergycontrollernew_p.h"

QT_BEGIN_NAMESPACE

class QLowEnergyServicePrivate : public QObject
{
    Q_OBJECT
public:
    explicit QLowEnergyServicePrivate(QObject *parent = 0);
    ~QLowEnergyServicePrivate();

    struct DescData {
        QByteArray value;
        QBluetoothUuid uuid;
    };

    struct CharData {
        QLowEnergyHandle valueHandle;
        QBluetoothUuid uuid;
        QLowEnergyCharacteristic::PropertyTypes properties;
        QByteArray value;
        QHash<QLowEnergyHandle, DescData> descriptorList;
    };

    enum GattAttributeTypes {
        PrimaryService = 0x2800,
        SecondaryService = 0x2801,
        IncludeAttribute = 0x2802,
        Characteristic = 0x2803
    };

    void setController(QLowEnergyControllerNewPrivate* control);
    void setError(QLowEnergyService::ServiceError newError);
    void setState(QLowEnergyService::ServiceState newState);

signals:
    void stateChanged(QLowEnergyService::ServiceState newState);
    void error(QLowEnergyService::ServiceError error);
    void characteristicChanged(const QLowEnergyCharacteristic &characteristic,
                               const QByteArray &newValue);
    void descriptorChanged(const QLowEnergyDescriptor &descriptor,
                           const QByteArray &newValue);

public:
    QLowEnergyHandle startHandle;
    QLowEnergyHandle endHandle;

    QBluetoothUuid uuid;
    QLowEnergyService::ServiceType type;
    QLowEnergyService::ServiceState state;
    QLowEnergyService::ServiceError lastError;

    QHash<QLowEnergyHandle, CharData> characteristicList;

    QPointer<QLowEnergyControllerNewPrivate> controller;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QSharedPointer<QLowEnergyServicePrivate>)

#endif // QLOWENERGYSERVICEPRIVATE_P_H