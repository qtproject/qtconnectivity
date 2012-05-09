/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QBLUETOOTHLOCALDEVICE_P_H
#define QBLUETOOTHLOCALDEVICE_P_H

#include "qbluetoothglobal.h"

#include "qbluetoothlocaldevice.h"

#ifdef QT_BLUEZ_BLUETOOTH
#include <QObject>
#include <QDBusContext>
#include <QDBusObjectPath>
#include <QDBusMessage>

class OrgBluezAdapterInterface;
class OrgBluezAgentAdaptor;

QT_BEGIN_NAMESPACE
class QDBusPendingCallWatcher;
QT_END_NAMESPACE

#ifdef NOKIA_BT_SERVICES
#include <QtServiceFramework/QServiceManager>
#include <QtCore/QMutex>
#endif
#endif

QT_BEGIN_HEADER

QTBLUETOOTH_BEGIN_NAMESPACE

class QBluetoothAddress;

#if defined(QT_BLUEZ_BLUETOOTH)
class QBluetoothLocalDevicePrivate : public QObject,
                                     protected QDBusContext
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QBluetoothLocalDevice)
public:
    QBluetoothLocalDevicePrivate(QBluetoothLocalDevice *q, QBluetoothAddress localAddress = QBluetoothAddress());
    ~QBluetoothLocalDevicePrivate();

    OrgBluezAdapterInterface *adapter;
    OrgBluezAgentAdaptor *agent;
    QString agent_path;
    QBluetoothAddress localAddress;
    QBluetoothAddress address;
    QBluetoothLocalDevice::Pairing pairing;
    QBluetoothLocalDevice::HostMode currentMode;

public Q_SLOTS: // METHODS
    void Authorize(const QDBusObjectPath &in0, const QString &in1);
    void Cancel();
    void ConfirmModeChange(const QString &in0);
    void DisplayPasskey(const QDBusObjectPath &in0, uint in1, uchar in2);
    void Release();
    uint RequestPasskey(const QDBusObjectPath &in0);

    void RequestConfirmation(const QDBusObjectPath &in0, uint in1);
    QString RequestPinCode(const QDBusObjectPath &in0);

    void pairingCompleted(QDBusPendingCallWatcher*);

    void PropertyChanged(QString,QDBusVariant);

#ifdef NOKIA_BT_SERVICES
    void powerStateChanged(bool powered);
    void pairingCompleted(bool success);
#endif

private:
    QDBusMessage msgConfirmation;
    QDBusConnection *msgConnection;

    QBluetoothLocalDevice *q_ptr;

    void initializeAdapter();
};

#ifdef NOKIA_BT_SERVICES
class NokiaBtManServiceConnection: public QObject
{
    Q_OBJECT

public:
    NokiaBtManServiceConnection();
    void acquire();
    void release();
    void setPowered(bool powered);
    bool powered() const;
    void setHostMode(QBluetoothLocalDevice::HostMode mode);
    void requestPairing(const QBluetoothAddress &address);

signals:
    void poweredChanged(bool powered);
    void pairingCompleted(bool success);

private:
    QObject *m_btmanService;
    int m_refCount;
    QMutex m_refCountMutex;
    bool m_forceDiscoverable;
    bool m_forceConnectable;
    QString m_pairingAddress;

private slots:
    void connectToBtManService();
    void disconnectFromBtManService();
    void sfwIPCError(QService::UnrecoverableIPCError);
    void powerStateChanged(int powerState);
    void pairingFinished(const QString &address, int direction, int status);
};
Q_GLOBAL_STATIC(NokiaBtManServiceConnection, nokiaBtManServiceInstance)
#endif

#else
class QBluetoothLocalDevicePrivate : public QObject
{
};
#endif

QTBLUETOOTH_END_NAMESPACE

QT_END_HEADER

#endif // QBLUETOOTHLOCALDEVICE_P_H
