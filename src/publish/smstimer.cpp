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
 Module Name				: SmsTimer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :   Timer for Sms upload, check every 15 minutes if any photo uploads are pending
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include <eikgted.h>
#include <eikmenup.h>
#include <eikgted.h>
#include <eikenv.h>
#include <bautils.h>

#include <f32file.h>
#ifndef __UIQ__
	#include <avkon.hrh>
#endif

#include "Smstimer.h"
#include "timeouttimer.h"

const TInt CSmsTimer::KTimeOut = 60000000; // 60 seconds time-out

class CRippleVaultAppUi;

CSmsTimer* CSmsTimer::NewL(CRippleVaultAppUi& aAppUi)
{
	CSmsTimer* self = CSmsTimer::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSmsTimer* CSmsTimer::NewLC(CRippleVaultAppUi& aAppUi)
{
	CSmsTimer* self = new (ELeave) CSmsTimer(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSmsTimer::CSmsTimer(CRippleVaultAppUi& aAppUi) 
: CActive(EPriorityIdle),
iAppUi(aAppUi)
{
}


CSmsTimer::~CSmsTimer()
{
    Cancel();
	delete iTimer;
}

void CSmsTimer::ConstructL()
{
	//iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);
	iTimer = CTimeOutTimer::NewL(EPriorityIdle, *this);
    	
	CActiveScheduler::Add(this); 
}

void CSmsTimer::Start()
{
	iTimer->After(KTimeOut * 5);
}

// from CActive
void CSmsTimer::DoCancel()
{
	iTimer->Cancel();
}

// from CActive
void CSmsTimer::RunL()
{
	switch(iEngineStatus)
	{
		case EProcess:
			{	
				SetActive();
			}
			break;
		case EWait:
			{				
				SetActive();
			}
			break;
		default:
			break;
	}
}

void CSmsTimer::TimerExpired()
{
	DoCancel();

	iAppUi.UploadSuccessfullSms();
}

void CSmsTimer::ChangeStatus(TWaitState aNewStatus)
{
    iEngineStatus = aNewStatus;
}

void CSmsTimer::Print(const TDesC& aDes)
{
}
