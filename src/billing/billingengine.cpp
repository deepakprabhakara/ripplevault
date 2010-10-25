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
 Module Name				: BillingEngine.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-06-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     Progress dialog for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : Billing Engine for Asurion deployment.


***************************************************************/


#include <eikgted.h>
#include "billingengine.h"
#include "RippleVault.pan"
#include "RippleVaultAppUi.h"
//#include "ContactsHandler.h"

class CRippleVaultAppUi;

//_LIT8(KGdpSmsHeaderTag,"//asurionbillingprobe");
_LIT8(KGdpSmsHeaderTag,"#Easy Billing#");

CBillingEngine* CBillingEngine::NewL(CRippleVaultAppUi& aAppUi)
{
	CBillingEngine* self = CBillingEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CBillingEngine* CBillingEngine::NewLC(CRippleVaultAppUi& aAppUi)
{
	CBillingEngine* self = new (ELeave) CBillingEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CBillingEngine::CBillingEngine(CRippleVaultAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CBillingEngine::~CBillingEngine()
{
    Cancel();
}

void CBillingEngine::ConstructL()
{
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iFs.Connect());
    	
	CActiveScheduler::Add(this); 
}

void CBillingEngine::Start()
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

void CBillingEngine::Disconnect()
    {
	iSocket.Close();
//	ChangeStatus(EConnecting);
    }

// from CActive
void CBillingEngine::DoCancel()
    {
//    ChangeStatus(EWait);
    }

// from CActive
void CBillingEngine::RunL()
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

	iAppUi.iBillingFlag = 0;
	iAppUi.SendBillingSMS();

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
void CBillingEngine::ChangeStatus(TWaitState aNewStatus)
    {
    iEngineStatus = aNewStatus;

    }
