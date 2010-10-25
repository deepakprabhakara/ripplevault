/* Copyright (c) 2001, Nokia. All rights reserved */

#include <eikgted.h>
#include "SmsLockEngine.h"
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

_LIT8(KGdpSmsHeaderTag,"freeze");

CSmsLockEngine* CSmsLockEngine::NewL(CSecureAppUi& aAppUi)
{
	CSmsLockEngine* self = CSmsLockEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSmsLockEngine* CSmsLockEngine::NewLC(CSecureAppUi& aAppUi)
{
	CSmsLockEngine* self = new (ELeave) CSmsLockEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSmsLockEngine::CSmsLockEngine(CSecureAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CSmsLockEngine::~CSmsLockEngine()
{
    Cancel();
}

void CSmsLockEngine::ConstructL()
{
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iFs.Connect());
    	
	CActiveScheduler::Add(this); 
}

void CSmsLockEngine::Start()
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

void CSmsLockEngine::Disconnect()
{
	iSocket.Close();
}

// from CActive
void CSmsLockEngine::DoCancel()
{
}

// from CActive
void CSmsLockEngine::RunL()
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

			//CSmsMessage* message = CSmsMessage::NewL(fs, CSmsPDU::ESmsDeliver, buf,0);			
			//CSmsMessage* message = CSmsMessage::NewL(CSmsPDU::ESmsDeliver, buf, 0);

			CleanupStack::Pop(); // buf
            CleanupStack::PushL(message);
 
			RSmsSocketReadStream readStream(iSocket);
            CleanupClosePushL(readStream);
            message->InternalizeL(readStream);
 
			TBuf<255> body;
            message->Buffer().Extract(body, 0, message->Buffer().Length());
            
			iAppUi.iPassWord.Copy(body);
			
			iAppUi.iPassWord.Delete(0,7);

			TPtrC address = message->ToFromAddress();
			iAppUi.iMobileNumber.Copy(address);
			//iAppUi.iMobileNumber.Copy(_L("9880002278"));	

			CleanupStack::PopAndDestroy(2); // message, readStream			

			fs.Close();

			ChangeStatus(EProcessSMS);
			SetActive();

			iSocket.Ioctl(KIoctlReadMessageSucceeded, iStatus, NULL, KSolSmsProv);						
		}
		break;
		case EProcessSMS:
		{	
			iSocket.Close();

			TInt pos = 0;
			TBuf<10> web;

			pos = iAppUi.iPassWord.LocateReverse(' ');
			if(pos != -1)
			{
				if((iAppUi.iPassWord.Length()-(pos+1)) < 10)
				{
					web.Copy(iAppUi.iPassWord.Right(iAppUi.iPassWord.Length()-(pos+1)));
					web.LowerCase();

					TInt pos1 = 0;
					pos1 = web.Compare(_L("web"));
					
					if(pos1 == 0)
					{
						iAppUi.iPassWord.Delete(pos, iAppUi.iPassWord.Length()-pos);
					}
				}
				iAppUi.CheckPasswordForLocking(3);//for web
			}
			else
				iAppUi.CheckPasswordForLocking(0);//for phone
		}
		break;
	default:
		break;
	}
}

void CSmsLockEngine::ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode)
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

void CSmsLockEngine::ResponseReceived(const TDesC8& aBuffer)
    {
//    iConsole.PrintNotify(aBuffer);
    }

void CSmsLockEngine::ChangeStatus(TWaitState aNewStatus)
{
    // Update the status (and the status display)
    iEngineStatus = aNewStatus;
}

void CSmsLockEngine::Print(const TDesC& aDes)
{
// Print some text on the console
// iConsole.PrintNotify(aDes, CEikGlobalTextEditor::EItalic);
}
