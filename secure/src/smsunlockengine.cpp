/* Copyright (c) 2001, Nokia. All rights reserved */

#include <eikgted.h>
#include "SmsUnLockEngine.h"
#include "Secureappui.h"	

#ifdef __UIQ__	
#ifdef __S80__
	#include <secure_s80.rsg>
	#include "Secure_s80.hrh"
#else
	#include <secure_uiq.rsg>
	#include "Secure_uiq.hrh"
#endif
#else
	#include <Secure.rsg>
	#include "Secure.hrh"
#endif

class CSecureAppUi;

_LIT8(KGdpSmsHeaderTag,"unfreeze");

CSmsUnLockEngine* CSmsUnLockEngine::NewL(CSecureAppUi& aAppUi)
{
	CSmsUnLockEngine* self = CSmsUnLockEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSmsUnLockEngine* CSmsUnLockEngine::NewLC(CSecureAppUi& aAppUi)
{
	CSmsUnLockEngine* self = new (ELeave) CSmsUnLockEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSmsUnLockEngine::CSmsUnLockEngine(CSecureAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CSmsUnLockEngine::~CSmsUnLockEngine()
{
    Cancel();
}

void CSmsUnLockEngine::ConstructL()
{
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iFs.Connect());
    	
	CActiveScheduler::Add(this); 
}

void CSmsUnLockEngine::Start()
{
	ChangeStatus(EReadSMS);
	//ChangeStatus(EDeleteSMS);
	
	// Close the socket as it may already be open.
	iSocketServer.Close();
	iSocket.Close();

	User::LeaveIfError(iSocketServer.Connect());

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

void CSmsUnLockEngine::Disconnect()
{
	iSocket.Close();
}

// from CActive
void CSmsUnLockEngine::DoCancel()
{
}

// from CActive
void CSmsUnLockEngine::RunL()
{
	switch(iEngineStatus)
	{    
		case EReadSMS:
		{	
			CSmsBuffer *buf=CSmsBuffer::NewL();
            CleanupStack::PushL(buf);

			RFs fs;
			
			fs.Connect();

#ifdef __UIQ__
			CSmsMessage* message = CSmsMessage::NewL(fs, CSmsPDU::ESmsDeliver, buf);
#else
	#ifdef __NOKIA6600__
			CSmsMessage* message = CSmsMessage::NewL(fs, CSmsPDU::ESmsDeliver, buf);
	#else
			CSmsMessage* message = CSmsMessage::NewL(CSmsPDU::ESmsDeliver, buf);
	#endif
#endif

			CleanupStack::Pop(); // buf
            CleanupStack::PushL(message);
 
            RSmsSocketReadStream readStream(iSocket);
            CleanupClosePushL(readStream);
            message->InternalizeL(readStream);
 
            TBuf<1024> body;
            message->Buffer().Extract(body, 0, message->Buffer().Length());
            
			iAppUi.iPassWord.Copy(body);

			iAppUi.iPassWord.Delete(0,9);

			CleanupStack::PopAndDestroy(2); // message, readStream			
			
			if(iAppUi.iPassWord.Length() > 255)
				iFlag = 1;
			else 
				iFlag = 0;

			ChangeStatus(EProcessSMS);
			SetActive();

			iSocket.Ioctl(KIoctlReadMessageSucceeded, iStatus, NULL, KSolSmsProv);			

			fs.Close();
		}
		break;
		case EProcessSMS:
		{				
			iSocket.Close();
			
			if(iFlag == 0)
				iAppUi.CheckPasswordForUnLocking();
			else if(iFlag == 1)
			{
				iAppUi.UnLockPhone();
				iAppUi.CreateSecureStopFile();
				User::Exit(EEikCmdExit);
			}
		}
		break;
	default:
		break;
	}
}

void CSmsUnLockEngine::ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode)
{
    // No recovery or retries are attempted in this example so we just
    // disconnect and inform the user
    Disconnect();

    switch (aErrorType)
    {
		case MEngineNotifier::EDisconnected:
			//iConsole.ErrorNotify(_L("CSMSEngine\nDisconnected"), aErrorCode);
			break;
		case MEngineNotifier::EGeneralReadError:
			//iConsole.ErrorNotify(_L("CSMSEngine\nRead Error"), aErrorCode);
			break;
		case MEngineNotifier::ETimeOutOnWrite:
			//iConsole.ErrorNotify(_L("CSMSEngine\nTime Out on write"), aErrorCode);
			break;
		case MEngineNotifier::EGeneralWriteError:
			//iConsole.ErrorNotify(_L("CSMSEngine\nWrite Error"), aErrorCode);
			break;
		default:
			//User::Panic(KPanicSocketsEngine, ESocketsBadStatus);
			break;
    }
}

void CSmsUnLockEngine::ResponseReceived(const TDesC8& aBuffer)
    {
//    iConsole.PrintNotify(aBuffer);
    }

void CSmsUnLockEngine::ChangeStatus(TWaitState aNewStatus)
{
    // Update the status (and the status display)
    iEngineStatus = aNewStatus;
}

void CSmsUnLockEngine::Print(const TDesC& aDes)
{
// Print some text on the console
// iConsole.PrintNotify(aDes, CEikGlobalTextEditor::EItalic);
}
