/****************************************************************************
 **
 ** Copyright (C) 2016 The Qt Company Ltd.
 ** Contact: https://www.qt.io/licensing/
 **
 ** This file is part of the QtBluetooth module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and The Qt Company. For licensing terms
 ** and conditions see https://www.qt.io/terms-conditions. For further
 ** information use the contact form at https://www.qt.io/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 3 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL3 included in the
 ** packaging of this file. Please review the following information to
 ** ensure the GNU Lesser General Public License version 3 requirements
 ** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 2.0 or (at your option) the GNU General
 ** Public license version 3 or any later version approved by the KDE Free
 ** Qt Foundation. The licenses are as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
 ** included in the packaging of this file. Please review the following
 ** information to ensure the GNU General Public License requirements will
 ** be met: https://www.gnu.org/licenses/gpl-2.0.html and
 ** https://www.gnu.org/licenses/gpl-3.0.html.
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

package org.qtproject.qt5.android.bluetooth;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.content.Context;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattServer;
import android.bluetooth.BluetoothGattServerCallback;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.AdvertiseCallback;
import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseData.Builder;
import android.bluetooth.le.AdvertiseSettings;
import android.bluetooth.le.BluetoothLeAdvertiser;
import android.os.ParcelUuid;
import android.util.Log;

import java.util.UUID;

public class QtBluetoothLEServer {
    private static final String TAG = "QtBluetoothGattServer";

    /* Pointer to the Qt object that "owns" the Java object */
    @SuppressWarnings({"CanBeFinal", "WeakerAccess"})
    long qtObject = 0;
    @SuppressWarnings("WeakerAccess")

    private Context qtContext = null;

    // Bluetooth members
    private final BluetoothAdapter mBluetoothAdapter;
    private BluetoothGattServer mGattServer = null;
    private BluetoothLeAdvertiser mLeAdvertiser = null;

    public QtBluetoothLEServer(Context context)
    {
        qtContext = context;
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter == null || qtContext == null) {
            Log.w(TAG, "Missing Bluetooth adapter or Qt context. Peripheral role disabled.");
            return;
        }

        BluetoothManager manager = (BluetoothManager) qtContext.getSystemService(Context.BLUETOOTH_SERVICE);
        if (manager == null) {
            Log.w(TAG, "Bluetooth service not available.");
            return;
        }

        mGattServer = manager.openGattServer(qtContext, mGattServerListener);
        mLeAdvertiser = mBluetoothAdapter.getBluetoothLeAdvertiser();

        if (!mBluetoothAdapter.isMultipleAdvertisementSupported())
            Log.w(TAG, "Device does not support Bluetooth Low Energy advertisement.");
        else
            Log.w(TAG, "Let's do BTLE Peripheral.");
    }

    /*
     * Call back handler for the Gatt Server.
     */
    private BluetoothGattServerCallback mGattServerListener = new BluetoothGattServerCallback()
    {
        @Override
        public void onConnectionStateChange(BluetoothDevice device, int status, int newState) {
            Log.w(TAG, "Our gatt server connection state changed, new state: " + Integer.toString(newState));
            super.onConnectionStateChange(device, status, newState);

            int qtControllerState = 0;
            switch (newState) {
                case BluetoothProfile.STATE_DISCONNECTED:
                    qtControllerState = 0; // QLowEnergyController::UnconnectedState
                    break;
                case BluetoothProfile.STATE_CONNECTED:
                    qtControllerState = 2; // QLowEnergyController::ConnectedState
                    break;
            }

            int qtErrorCode;
            switch (status) {
                case BluetoothGatt.GATT_SUCCESS:
                    qtErrorCode = 0; break;
                default:
                    Log.w(TAG, "Unhandled error code on peripheral connectionStateChanged: " + status + " " + newState);
                    qtErrorCode = status;
                    break;
            }

            leServerConnectionStateChange(qtObject, qtErrorCode, qtControllerState);
        }

        @Override
        public void onServiceAdded(int status, BluetoothGattService service) {
            super.onServiceAdded(status, service);
        }

        @Override
        public void onCharacteristicReadRequest(BluetoothDevice device, int requestId, int offset, BluetoothGattCharacteristic characteristic) {
            super.onCharacteristicReadRequest(device, requestId, offset, characteristic);
        }

        @Override
        public void onCharacteristicWriteRequest(BluetoothDevice device, int requestId, BluetoothGattCharacteristic characteristic, boolean preparedWrite, boolean responseNeeded, int offset, byte[] value) {
            super.onCharacteristicWriteRequest(device, requestId, characteristic, preparedWrite, responseNeeded, offset, value);
        }

        @Override
        public void onDescriptorReadRequest(BluetoothDevice device, int requestId, int offset, BluetoothGattDescriptor descriptor) {
            super.onDescriptorReadRequest(device, requestId, offset, descriptor);
        }

        @Override
        public void onDescriptorWriteRequest(BluetoothDevice device, int requestId, BluetoothGattDescriptor descriptor, boolean preparedWrite, boolean responseNeeded, int offset, byte[] value) {
            super.onDescriptorWriteRequest(device, requestId, descriptor, preparedWrite, responseNeeded, offset, value);
        }

        @Override
        public void onExecuteWrite(BluetoothDevice device, int requestId, boolean execute) {
            super.onExecuteWrite(device, requestId, execute);
        }

        @Override
        public void onNotificationSent(BluetoothDevice device, int status) {
            super.onNotificationSent(device, status);
        }

        // MTU change disabled since it requires API level 22. Right now we only enforce lvl 21
//        @Override
//        public void onMtuChanged(BluetoothDevice device, int mtu) {
//            super.onMtuChanged(device, mtu);
//        }
    };

    public boolean connectServer()
    {
        if (mGattServer == null)
            return false;

        return true;
    }

    public void disconnectServer()
    {
        if (mGattServer == null)
            return;

        mGattServer.close();
    }

    public boolean startAdvertising(AdvertiseData advertiseData,
                                    AdvertiseData scanResponse,
                                    AdvertiseSettings settings)
    {
        if (mLeAdvertiser == null)
            return false;

        connectServer();

        Log.w(TAG, "Starting to advertise.");
        mLeAdvertiser.startAdvertising(settings, advertiseData, scanResponse, mAdvertiseListener);

        return true;
    }

    public void stopAdvertising()
    {
        if (mLeAdvertiser == null)
            return;

        mLeAdvertiser.stopAdvertising(mAdvertiseListener);
        Log.w(TAG, "Advertisement stopped.");
    }

    public void addService(BluetoothGattService service)
    {
        if (mGattServer == null)
            return;

        mGattServer.addService(service);
    }

    /*
     * Call back handler for Advertisement requests.
     */
    private AdvertiseCallback mAdvertiseListener = new AdvertiseCallback()
    {
        @Override
        public void onStartSuccess(AdvertiseSettings settingsInEffect) {
            super.onStartSuccess(settingsInEffect);
        }

        @Override
        public void onStartFailure(int errorCode) {
            Log.e(TAG, "Advertising failure: " + errorCode);
            super.onStartFailure(errorCode);
        }
    };

    public native void leServerConnectionStateChange(long qtObject, int errorCode, int newState);

}