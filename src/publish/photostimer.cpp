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
 Module Name				: PhotosTimer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :   Timer for photos upload, check every 15 minutes if any photo uploads are pending
 

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

#include "photostimer.h"
#include "timeouttimer.h"

const TInt CPhotosTimer::KTimeOut = 60000000; // 60 seconds time-out

class CRippleVaultAppUi;

CPhotosTimer* CPhotosTimer::NewL(CRippleVaultAppUi& aAppUi)
{
	CPhotosTimer* self = CPhotosTimer::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CPhotosTimer* CPhotosTimer::NewLC(CRippleVaultAppUi& aAppUi)
{
	CPhotosTimer* self = new (ELeave) CPhotosTimer(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CPhotosTimer::CPhotosTimer(CRippleVaultAppUi& aAppUi) 
: CActive(EPriorityIdle),
iAppUi(aAppUi)
{
}


CPhotosTimer::~CPhotosTimer()
{
    Cancel();
	delete iTimer;
}

void CPhotosTimer::ConstructL()
{
	//iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);
	iTimer = CTimeOutTimer::NewL(EPriorityIdle, *this);
    	
	CActiveScheduler::Add(this); 
}

void CPhotosTimer::Start()
{
	iTimer->After(KTimeOut * 15);	
}

// from CActive
void CPhotosTimer::DoCancel()
{
	iTimer->Cancel();
}

// from CActive
void CPhotosTimer::RunL()
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

void CPhotosTimer::TimerExpired()
{
	DoCancel();

	//iAppUi.EnablePhotosUpload();
		TInt network;	
		network = iAppUi.CheckNetwork();

		if(network == NETWORK_ROAMING)
		{
			if(iAppUi.iRoaming == 1)
			{
				iAppUi.EnablePhotosUpload();
			}
		}
		else
			iAppUi.EnablePhotosUpload();


	iTimer->After(KTimeOut * 15);	
}

void CPhotosTimer::ChangeStatus(TWaitState aNewStatus)
{
    iEngineStatus = aNewStatus;
}

void CPhotosTimer::Print(const TDesC& aDes)
{
}
