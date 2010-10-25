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
 Module Name				: Timeouttimer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   : Network time outs
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include "TimeOutTimer.h"
#include "TimeOutNotifier.h"
#include "RippleVault.pan"


CTimeOutTimer* CTimeOutTimer::NewL(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify)
    {
	CTimeOutTimer* self = CTimeOutTimer::NewLC(aPriority, aTimeOutNotify);
	CleanupStack::Pop(self);
	return self;
    }

	
CTimeOutTimer* CTimeOutTimer::NewLC(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify)
    {
	CTimeOutTimer* self = new (ELeave) CTimeOutTimer(aPriority, aTimeOutNotify);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }


CTimeOutTimer::CTimeOutTimer(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify)
: CTimer(aPriority), iNotify(aTimeOutNotify)
    {
    }


CTimeOutTimer::~CTimeOutTimer()
    {
    }


void CTimeOutTimer::ConstructL()
    {
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
    }

void CTimeOutTimer::RunL()
    {
    // Timer request has completed, so notify the timer's owner
    if (iStatus == KErrNone)
        {
	    iNotify.TimerExpired();
        }
	}

