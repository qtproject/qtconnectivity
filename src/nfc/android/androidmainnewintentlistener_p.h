/****************************************************************************
**
** Copyright (C) 2015 BasysKom GmbH
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtNfc module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef ANDROIDMAINNEWINTENTLISTENER_P_H_
#define ANDROIDMAINNEWINTENTLISTENER_P_H_

#include <QtCore/private/qjnihelpers_p.h>

#include "qlist.h"
#include "qreadwritelock.h"
#include "androidjninfc_p.h"

QT_BEGIN_ANDROIDNFC_NAMESPACE

class MainNfcNewIntentListener : public QtAndroidPrivate::NewIntentListener, QtAndroidPrivate::ResumePauseListener
{
public:
    MainNfcNewIntentListener();
    ~MainNfcNewIntentListener();

    //QtAndroidPrivate::NewIntentListener
    bool handleNewIntent(JNIEnv *env, jobject intent);

    bool registerListener(AndroidNfcListenerInterface *listener);
    bool unregisterListener(AndroidNfcListenerInterface *listener);

    //QtAndroidPrivate::ResumePauseListener
    void handleResume();
    void handlePause();

private:
    void updateReceiveState();
protected:
    QList<AndroidNfc::AndroidNfcListenerInterface*> listeners;
    QReadWriteLock listenersLock;
    bool paused;
    bool receiving;
};

QT_END_ANDROIDNFC_NAMESPACE

#endif /* ANDROIDMAINNEWINTENTLISTENER_P_H_ */