/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef LOWENERGYNOTIFICATIONHUB_H
#define LOWENERGYNOTIFICATIONHUB_H

#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtCore/private/qjnihelpers_p.h>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtBluetooth/QBluetoothAddress>
#include <jni.h>

#include <QtBluetooth/QLowEnergyCharacteristic>
#include "qlowenergycontroller_p.h"

QT_BEGIN_NAMESPACE

class LowEnergyNotificationHub : public QObject
{
    Q_OBJECT
public:
    explicit LowEnergyNotificationHub(const QBluetoothAddress &remote,
                                      QObject *parent = 0);
    ~LowEnergyNotificationHub();

    static void lowEnergy_connectionChange(JNIEnv*, jobject, jlong qtObject,
                                           jint errorCode, jint newState);
    static void lowEnergy_servicesDiscovered(JNIEnv*, jobject, jlong qtObject,
                                             jint errorCode, jobject uuidList);
    static void lowEnergy_serviceDetailsDiscovered(JNIEnv *, jobject,
                                                   jlong qtObject, jobject uuid);
    static void lowEnergy_characteristicRead(JNIEnv*env, jobject, jlong qtObject,
                                             jobject serviceUuid,
                                             jint handle, jobject charUuid,
                                             jint properties, jbyteArray data);

    QAndroidJniObject javaObject()
    {
        return jBluetoothLe;
    }

signals:
    void connectionUpdated(QLowEnergyController::ControllerState newState,
            QLowEnergyController::Error errorCode);
    void servicesDiscovered(QLowEnergyController::Error errorCode, const QString &uuids);
    void serviceDetailsDiscoveryFinished(const QString& serviceUuid);
    void characteristicRead(const QBluetoothUuid &serviceUuid,
            int handle, const QBluetoothUuid &charUuid,
            int properties, const QByteArray& data);

public slots:
private:
    static QReadWriteLock lock;

    QAndroidJniObject jBluetoothLe;
    long javaToCtoken;

};

QT_END_NAMESPACE

#endif // LOWENERGYNOTIFICATIONHUB_H
