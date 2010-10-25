/* Copyright (c) 2001, Nokia. All rights reserved */

#include <eikgted.h>
#include "SmsUnSubscribeEngine.h"
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

_LIT8(KGdpSmsHeaderTag,"#Secure Unsubscribe#");

CSmsUnSubscribeEngine* CSmsUnSubscribeEngine::NewL(CSecureAppUi& aAppUi)
{
	CSmsUnSubscribeEngine* self = CSmsUnSubscribeEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSmsUnSubscribeEngine* CSmsUnSubscribeEngine::NewLC(CSecureAppUi& aAppUi)
{
	CSmsUnSubscribeEngine* self = new (ELeave) CSmsUnSubscribeEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSmsUnSubscribeEngine::CSmsUnSubscribeEngine(CSecureAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CSmsUnSubscribeEngine::~CSmsUnSubscribeEngine()
{
    Cancel();
}

void CSmsUnSubscribeEngine::ConstructL()
{
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iFs.Connect());
    	
	CActiveScheduler::Add(this); 
}

void CSmsUnSubscribeEngine::Start()
{
	ChangeStatus(EReadSMS);
	
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

void CSmsUnSubscribeEngine::Disconnect()
{
	iSocket.Close();
}

// from CActive
void CSmsUnSubscribeEngine::DoCancel()
{
}

// from CActive
void CSmsUnSubscribeEngine::RunL()
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
 
            TBuf<255> body;
            message->Buffer().Extract(body, 0, message->Buffer().Length());
            
			iPassWord.Copy(body);

			iPassWord.Delete(0,KGdpSmsHeaderTag().Length());

			TBuf<60> temp;
			temp.Copy(_L(". Your request to unsubscribe has been completed."));

			TInt pos;				
			pos = iPassWord.Find(temp);
			
			iPassWord.Delete(pos,temp.Length());

			TPtrC address = message->ToFromAddress();
			iNumber.Copy(address);

			CleanupStack::PopAndDestroy(2); // message, readStream			
			
			ChangeStatus(EProcessSMS);
			SetActive();

			iSocket.Ioctl(KIoctlReadMessageSucceeded, iStatus, NULL, KSolSmsProv);			

			fs.Close();
		}
		break;
		case EProcessSMS:
		{				
			iSocket.Close();
			
			iAppUi.CheckPasswordForUnSubscribe(iPassWord, iNumber);
		}
		break;
	default:
		break;
	}
}

void CSmsUnSubscribeEngine::ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode)
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

void CSmsUnSubscribeEngine::ResponseReceived(const TDesC8& aBuffer)
    {
//    iConsole.PrintNotify(aBuffer);
    }

void CSmsUnSubscribeEngine::ChangeStatus(TWaitState aNewStatus)
{
    // Update the status (and the status display)
    iEngineStatus = aNewStatus;
}

void CSmsUnSubscribeEngine::Print(const TDesC& aDes)
{
// Print some text on the console
// iConsole.PrintNotify(aDes, CEikGlobalTextEditor::EItalic);
}
