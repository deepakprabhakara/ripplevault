/*******************************************************************

 Copyright(c) 2000-2005 

 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.

 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..

 Project Name               : Ripple Vault Symbian Client
 Module Name				: PhotoTimeoutTimer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : 
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include "phototimeouttimer.h"
#include "TimeOutNotifier.h"
#include "ripplevault.pan"


CPhotoTimeoutTimer* CPhotoTimeoutTimer::NewL(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify)
    {
	CPhotoTimeoutTimer* self = CPhotoTimeoutTimer::NewLC(aPriority, aTimeOutNotify);
	CleanupStack::Pop(self);
	return self;
    }

	
CPhotoTimeoutTimer* CPhotoTimeoutTimer::NewLC(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify)
    {
	CPhotoTimeoutTimer* self = new (ELeave) CPhotoTimeoutTimer(aPriority, aTimeOutNotify);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CPhotoTimeoutTimer::CPhotoTimeoutTimer(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify)
: CTimer(aPriority), iNotify(aTimeOutNotify)
    {
    }


CPhotoTimeoutTimer::~CPhotoTimeoutTimer()
    {
    	Cancel();
    }


void CPhotoTimeoutTimer::ConstructL()
    {
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
    }

void CPhotoTimeoutTimer::RunL()
    {
    // Timer request has completed, so notify the timer's owner
    if (iStatus == KErrNone)
        {
	    iNotify.TimerExpired();
        }
	}

