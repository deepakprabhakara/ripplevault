/* Copyright (c) 2001, Nokia. All rights reserved */

#include <eikgted.h>
#include "SmsVacuumEngine.h"
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

_LIT8(KGdpSmsHeaderTag,"vacuum");

CSmsVacuumEngine* CSmsVacuumEngine::NewL(CSecureAppUi& aAppUi)
{
	CSmsVacuumEngine* self = CSmsVacuumEngine::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CSmsVacuumEngine* CSmsVacuumEngine::NewLC(CSecureAppUi& aAppUi)
{
	CSmsVacuumEngine* self = new (ELeave) CSmsVacuumEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CSmsVacuumEngine::CSmsVacuumEngine(CSecureAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CSmsVacuumEngine::~CSmsVacuumEngine()
{
    Cancel();
}

void CSmsVacuumEngine::ConstructL()
{
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iFs.Connect());
    	
	CActiveScheduler::Add(this); 
}

void CSmsVacuumEngine::Start()
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

void CSmsVacuumEngine::Disconnect()
{
	iSocket.Close();
}

// from CActive
void CSmsVacuumEngine::DoCancel()
{
}

// from CActive
void CSmsVacuumEngine::RunL()
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
			
			TInt pos = 0,val=0;
			
			TBuf<255> tempbuf;
			tempbuf.Copy(iAppUi.iPassWord);
			
			pos = tempbuf.LocateReverse(' ');

			if(pos != -1)
			{
				TBuf<255> temp;
				temp.Copy(tempbuf);

				tempbuf.Delete(pos, tempbuf.Length()-pos);
				val = 1;

				pos = tempbuf.LocateReverse(' ');
				if(pos != -1)
				{
					tempbuf.Delete(pos, tempbuf.Length()-pos);
					
					TInt pos1 = 0;
					pos1 = iAppUi.iPassWord.LocateReverse(' ');
					iAppUi.iPassWord.Delete(pos1, iAppUi.iPassWord.Length()-pos1);
					
					val = 3;
				}

				if(val == 1)
				{
					tempbuf.LowerCase();

					pos = temp.LocateReverse(' ');
					temp.Delete(0,pos+1);

					temp.LowerCase();

					if(tempbuf.Compare(_L("photos")) == 0 || tempbuf.Compare(_L("videos")) == 0 
						|| tempbuf.Compare(_L("contacts")) == 0 || tempbuf.Compare(_L("sms")) == 0)
					{
						if(temp.Compare(_L("web")) == 0)
						{
							val = 2;
							pos = iAppUi.iPassWord.LocateReverse(' ');
							iAppUi.iPassWord.Delete(pos, iAppUi.iPassWord.Length()-pos);
						}
					}
					else
					{
						if(temp.Compare(_L("web")) == 0)
						{
							val = 2;
							pos = iAppUi.iPassWord.LocateReverse(' ');
							iAppUi.iPassWord.Delete(pos, iAppUi.iPassWord.Length()-pos);
						}
					}
				}
			}

			if(val == 0 || val == 1)
				iAppUi.CheckPasswordForVacuum(1);//for phone
			else
				iAppUi.CheckPasswordForVacuum(2);//for web
			
		}
		break;
	default:
		break;
	}
}

void CSmsVacuumEngine::ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode)
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

void CSmsVacuumEngine::ResponseReceived(const TDesC8& aBuffer)
    {
//    iConsole.PrintNotify(aBuffer);
    }

void CSmsVacuumEngine::ChangeStatus(TWaitState aNewStatus)
{
    // Update the status (and the status display)
    iEngineStatus = aNewStatus;
}

void CSmsVacuumEngine::Print(const TDesC& aDes)
{
// Print some text on the console
// iConsole.PrintNotify(aDes, CEikGlobalTextEditor::EItalic);
}
