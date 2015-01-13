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

#include "osxbtdevicepair_p.h"
#include "osxbtutility_p.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

namespace OSXBluetooth {

ObjCStrongReference<IOBluetoothDevice> device_with_address(const QBluetoothAddress &address)
{
    if (address.isNull())
        return ObjCStrongReference<IOBluetoothDevice>(nil, false);

    const BluetoothDeviceAddress &iobtAddress = iobluetooth_address(address);
    ObjCStrongReference<IOBluetoothDevice> res([[IOBluetoothDevice deviceWithAddress:&iobtAddress] retain], false);
    return res;
}

PairingDelegate::~PairingDelegate()
{
}

}


QT_END_NAMESPACE


#ifdef QT_NAMESPACE
using namespace QT_NAMESPACE;
#endif

@implementation QT_MANGLE_NAMESPACE(OSXBTPairing)

- (id)initWithTarget:(const QBluetoothAddress &)address
      delegate:(OSXBluetooth::PairingDelegate *)object
{
    if (self = [super init]) {
        Q_ASSERT_X(!address.isNull(), Q_FUNC_INFO, "invalid target address");
        Q_ASSERT_X(object, Q_FUNC_INFO, "invalid delegate (null)");

        m_targetAddress = address;
        m_object = object;
        m_active = false;
    }

    return self;
}

- (void)dealloc
{
#if QT_MAC_PLATFORM_SDK_EQUAL_OR_ABOVE(__MAC_10_9, __IPHONE_NA)
    // Stop also sets a delegate to nil (Apple's docs).
    // 10.9 only.
    [m_pairing stop];
#else
    [m_pairing setDelegate:nil];
#endif
    [m_pairing release];

    [super dealloc];
}

- (IOReturn) start
{
    if (m_active)
        return kIOReturnBusy;

    Q_ASSERT_X(!m_targetAddress.isNull(), Q_FUNC_INFO, "invalid target address");

    QT_BT_MAC_AUTORELEASEPOOL;

    const BluetoothDeviceAddress &iobtAddress = OSXBluetooth::iobluetooth_address(m_targetAddress);
    // Device is autoreleased.
    IOBluetoothDevice *const device = [IOBluetoothDevice deviceWithAddress:&iobtAddress];
    if (!device) {
        qCCritical(QT_BT_OSX) << Q_FUNC_INFO << "failed to create a device "
                                 "to pair with";
        return kIOReturnError;
    }

    m_pairing = [[IOBluetoothDevicePair pairWithDevice:device] retain];
    if (!m_pairing) {
        qCCritical(QT_BT_OSX) << Q_FUNC_INFO << "failed to create pair";
        return kIOReturnError;
    }

    [m_pairing setDelegate:self];
    const IOReturn result = [m_pairing start];
    if (result != kIOReturnSuccess) {
        [m_pairing release];
        m_pairing = nil;
    } else
        m_active = true;

    return result;
}

- (bool)isActive
{
    return m_active;
}

- (void)stop
{
    // stop: stops pairing, removes the delegate
    // and disconnects if device was connected.
    if (m_pairing)
        [m_pairing stop];
}

- (const QBluetoothAddress &)targetAddress
{
    return m_targetAddress;
}

- (IOBluetoothDevicePair *)pairingRequest
{
    return [[m_pairing retain] autorelease];
}

- (IOBluetoothDevice *)targetDevice
{
    return [m_pairing device];//It's retained/autoreleased by pair.
}

// IOBluetoothDevicePairDelegate:

- (void)devicePairingStarted:(id)sender
{
    Q_UNUSED(sender)
}

- (void)devicePairingConnecting:(id)sender
{
    Q_UNUSED(sender)
}

- (void)deviceParingPINCodeRequest:(id)sender
{
    Q_UNUSED(sender)
}

- (void)devicePairingUserConfirmationRequest:(id)sender
        numericValue:(BluetoothNumericValue)numericValue
{
    if (sender != m_pairing) // Can never happen.
        return;

    Q_ASSERT_X(m_object, Q_FUNC_INFO, "invalid delegate (null)");

    m_object->requestUserConfirmation(self, numericValue);
}

- (void)devicePairingUserPasskeyNotification:(id)sender
        passkey:(BluetoothPasskey)passkey
{
    Q_UNUSED(sender)
    Q_UNUSED(passkey)
}

- (void)devicePairingFinished:(id)sender error:(IOReturn)error
{
    Q_ASSERT_X(m_object, Q_FUNC_INFO, "invalid delegate (null)");

    if (sender != m_pairing) // Can never happen though.
        return;

    m_active = false;
    if (error != kIOReturnSuccess)
        m_object->error(self, error);
    else
        m_object->pairingFinished(self);
}

- (void)deviceSimplePairingComplete:(id)sender
        status:(BluetoothHCIEventStatus)status
{
    Q_UNUSED(sender)
    Q_UNUSED(status)
}

@end