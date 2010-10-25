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
 Module Name				: SmsEngine.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Engine to listen for SMS probes sent by server. Connects to server upon receiving probe to pick up pending changes
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include <eikgted.h>
#include "smsengine.h"
#include "RippleVault.pan"
#include "RippleVaultAppUi.h"
#include "ContactsHandler.h"

class CRippleVaultAppUi;

//_LIT8(KGdpSmsHeaderTag,"//rippleprobe");
_LIT8(KGdpSmsHeaderTag,"#Easy Vault#");

CSMSEngine* CSMSEngine::NewL(CRippleVaultAppUi& aAppUi)
{
	CSMSEngine* self = CSMSEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSMSEngine* CSMSEngine::NewLC(CRippleVaultAppUi& aAppUi)
{
	CSMSEngine* self = new (ELeave) CSMSEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSMSEngine::CSMSEngine(CRippleVaultAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CSMSEngine::~CSMSEngine()
{
    Cancel();
}

void CSMSEngine::ConstructL()
{
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iFs.Connect());
    	
	CActiveScheduler::Add(this); 
}

void CSMSEngine::Start()
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
	SetActive();	
}

// from CActive
void CSMSEngine::DoCancel()
    {
//    ChangeStatus(EWait);
    }

// from CActive
void CSMSEngine::RunL()
    {

switch(iEngineStatus)
{
    
case EDeleteSMS:{

	ChangeStatus(EProcessSMS);
	// Create an empty message and buffer for our incoming message.
	/*
	CSmsBuffer* buffer1=NULL;
	buffer1=CSmsBuffer::NewL();
	iSmsMsg = CSmsMessage::NewL(CSmsPDU::ESmsSubmit, buffer1);

//IMPORT_C static CSmsMessage* NewL(CSmsPDU::TSmsPDUType aType,CSmsBufferBase* aBuffer,TBool aIsRPError=EFalse);
	// Read the message.
	RSmsSocketReadStream readstream(iSocket);
	readstream >> *(iSmsMsg);
	*/

	// Let the socket know that we have read the message and it can be removed from the message
	// store.

	iSocket.Ioctl(KIoctlReadMessageSucceeded, iStatus, NULL, KSolSmsProv);
	
	SetActive();

}
break;

case EProcessSMS:{

	//iAppUi.iContactsHandler->SyncToServer();
	if(iAppUi.iContactsHandler->iContactsUpload == 0)
	{
		iAppUi.iSynchronize = 0;
		iAppUi.UpdateContacts(0);
		/*
		iAppUi.iContactsTimerState = 1;
		iAppUi.iConnectionStatus = 2;
		iAppUi.CheckSubscriptionStatus(iAppUi.iConnectionStatus);///
		*/			
	}

	// Cleanup
	//	CleanupStack::PopAndDestroy(); // hbuf, hFromAddress;
	//delete iSmsMsg;
	iSocket.Close();
	Start();
}
break;

default:
break;
}
    }

void CSMSEngine::ChangeStatus(TWaitState aNewStatus)
    {
		iEngineStatus = aNewStatus;
    }

