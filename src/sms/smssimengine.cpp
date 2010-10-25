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
 Module Name				: SmsSimEngine.cpp
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


#include <eikgted.h>
#include "smssimengine.h"
#include "RippleVault.pan"
#include "RippleVaultappui.h"

#ifndef __UIQ__	
	#include <aknnotewrappers.h> 
	#include <RippleVault.rsg>
#else
#ifdef __S80__
	#include <RippleVault_series80.rsg>	
	#include "RippleVault_series80.hrh"
#else
	#include <RippleVault_uiq.rsg>	
	#include "RippleVault_uiq.hrh"
#endif
#endif	

class CRippleVaultAppUi;

_LIT8(KGdpSmsHeaderTag,"//simrippleprobe");

CSmsSimEngine* CSmsSimEngine::NewL(CRippleVaultAppUi& aAppUi)
{
	CSmsSimEngine* self = CSmsSimEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSmsSimEngine* CSmsSimEngine::NewLC(CRippleVaultAppUi& aAppUi)
{
	CSmsSimEngine* self = new (ELeave) CSmsSimEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSmsSimEngine::CSmsSimEngine(CRippleVaultAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CSmsSimEngine::~CSmsSimEngine()
{
    Cancel();
}

void CSmsSimEngine::ConstructL()
{
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iFs.Connect());
    	
	CActiveScheduler::Add(this); 
}

void CSmsSimEngine::Start()
{
	ChangeStatus(EDeleteSMS);
	// Close the socket as it may already be open.
	iSocketServer.Close();
	iSocket.Close();

	User::LeaveIfError(iSocketServer.Connect());
//	iSocketServer.Connect();
	// Open a socket
	User::LeaveIfError(iSocket.Open(iSocketServer, KSMSAddrFamily, KSockDatagram, KSMSDatagramProtocol));

	// Set the pattern to search for in incoming SMS messages. Messages which do not have our
	// signature will be consumed by the Messaging Application.
    TSmsAddr smsAddr;
	smsAddr.SetSmsAddrFamily(ESmsAddrMatchText);
	smsAddr.SetTextMatch(KGdpSmsHeaderTag);
	
	User::LeaveIfError(iSocket.Bind(smsAddr));

	// Wait for incoming messages	
    iOctlResult()= KSockSelectRead;
    iSocket.Ioctl(KIOctlSelect, iStatus, &(iOctlResult), KSOLSocket);


	iAppUi.iSmsValue = 1;	
			
	SetActive();
#ifndef __UIQ__	
	//iAppUi.iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&(iAppUi.iWaitDialog))));
	//iAppUi.iWaitDialog->SetTone( CAknNoteDialog::EConfirmationTone ); 	
	//iAppUi.iWaitDialog->ExecuteLD(R_SMS_DIALOG);
#else	
	//iAppUi.Show = 2;
	CEikDialog* iWaitDialog1;
	iWaitDialog1 = new(ELeave) CSmsProgInfoControl(*this);
	iWaitDialog1->ExecuteLD(R_SMS_DIALOG);
#endif	
}

void CSmsSimEngine::Disconnect()
    {
//    __ASSERT_ALWAYS(iEngineStatus == EConnected, User::Panic(KPanicSocketsEngine, ESocketsBadState));

	iSocket.Close();
//	ChangeStatus(EConnecting);
    }

// from CActive
void CSmsSimEngine::DoCancel()
    {
//    ChangeStatus(EWait);
    }

// from CActive
void CSmsSimEngine::RunL()
{
	switch(iEngineStatus)
	{    
		case EDeleteSMS:
		{	
			ChangeStatus(EProcessSMS);			

			iSocket.Ioctl(KIoctlReadMessageSucceeded, iStatus, NULL, KSolSmsProv);			
			
			SetActive();			
		}
		break;

		case EProcessSMS:
		{					
			iAppUi.iSmsValue = -1;
			iAppUi.Show = 0;

		#ifndef __UIQ__		
			if(iAppUi.iWaitDialog != NULL)
				iAppUi.iWaitDialog->ProcessFinishedL();

			//CAknInformationNote* note = new(ELeave) CAknInformationNote;
			//note->ExecuteLD(_L("Validated"));
		#else		
			//iAppUi.iSmsValue = 2;	
			iAppUi.ShowView();
		#endif			
			
			iAppUi.ConnectToServerForChangingNumber();			
			
			//iSocket.Close();				
		}
		break;
	default:
		break;
	}
}

void CSmsSimEngine::ChangeStatus(TWaitState aNewStatus)
    {
    iEngineStatus = aNewStatus;
    }
	

#ifdef __UIQ__
CSmsProgInfoControl::CSmsProgInfoControl(CSmsSimEngine& aAppUi):iAppUi(aAppUi)
{	
}

CSmsProgInfoControl::~CSmsProgInfoControl()
{
}

void CSmsProgInfoControl::PreLayoutDynInitL()
{		
}

TInt CSmsProgInfoControl::OkToExitL(TInt /*aKeycode*/)
{
	iAppUi.ExitApp();	
	return 0;
}

void CSmsSimEngine::ExitApp()
{
	iAppUi.ExitApplication();
}

#endif 