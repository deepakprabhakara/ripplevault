/* Copyright (c) 2001, Nokia. All rights reserved */

#include <eikgted.h>
#include <eikmenup.h>
#include <eikgted.h>
#include <eikenv.h>
#include <bautils.h>
#include <AgentClient.h>
//#include <intconninit.h> 

#include <f32file.h>
#ifndef __UIQ__
	#include <avkon.hrh>
#endif

#include "contactstimer.h"
#include "timeouttimer.h"

const TInt CContactsTimer::KTimeOut = 60000000; // 60 seconds time-out

class CRippleVaultAppUi;

CContactsTimer* CContactsTimer::NewL(CRippleVaultAppUi& aAppUi)
{
	CContactsTimer* self = CContactsTimer::NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}

	
CContactsTimer* CContactsTimer::NewLC(CRippleVaultAppUi& aAppUi)
{
	CContactsTimer* self = new (ELeave) CContactsTimer(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CContactsTimer::CContactsTimer(CRippleVaultAppUi& aAppUi) 
: CActive(EPriorityStandard),
iAppUi(aAppUi)
{
}


CContactsTimer::~CContactsTimer()
{
    Cancel();
	delete iTimer;
}

void CContactsTimer::ConstructL()
{
	iTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);
    	
	CActiveScheduler::Add(this); 
}

void CContactsTimer::Start()
{
	iAppUi.LogText.Copy(_L("ContactsTimeStarted\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);

	iTimer->After(KTimeOut * 5);
}

void CContactsTimer::Disconnect()
{
}

// from CActive
void CContactsTimer::DoCancel()
{
	iTimer->Cancel();
}

// from CActive
void CContactsTimer::RunL()
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

void CContactsTimer::TimerExpired()
{
	DoCancel();

	iAppUi.LogText.Copy(_L("ContactsTimeExpired\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);

	if(iAppUi.iContactsHandler->iContactsUpload == 0)
	{
		iAppUi.LogText.Copy(_L("ContactsTimeExpired1\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
		
		if(iAppUi.iContactsTimerState == 1)
		{
			iAppUi.iSynchronize = 0;
			iAppUi.UpdateContacts(0);
			/*
			iAppUi.iConnectionStatus = 2;
			iAppUi.CheckSubscriptionStatus(iAppUi.iConnectionStatus);
			*/
		}
	}	
	else if(iAppUi.iContactsHandler->iContactsUpload == 1)
	{
		iAppUi.LogText.Copy(_L("ContactsTimeExpired3\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
		UploadContacts();
	}

	//CloseConnection();	

	iTimer->After(KTimeOut * 5);	
}

void CContactsTimer::UploadContacts()
{
	TInt days;	
	TInt contactsday;
	TInt syncstate = 0;
	
	iAppUi.GetConatctsTimeFromDB(contactsday, days, syncstate);

	if( contactsday == days )
	{
		iAppUi.LogText.Copy(_L("Day Equal\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
		
		iAppUi.iSynchronize = 0;
		iAppUi.UpdateContacts(0);
		
		syncstate = 1;

		iAppUi.SaveConatctsTimeToDB(contactsday, days, syncstate);
		/*
		iAppUi.iContactsTimerState = 1;
		iAppUi.iConnectionStatus = 2;
		iAppUi.CheckSubscriptionStatus(iAppUi.iConnectionStatus);///
		*/
	}	
	else
	{
		TTime time;
		TTime time1;
		TTime time2;
		TDateTime datetime;
		TBuf<50> timebuffer;
		
		_LIT(KTime,"00000000:");
		_LIT(KTime1,"00000000:000000.000000");
		_LIT(KTime2,"00000000:000500.000000");
		_LIT(KTime3,".000000");
		
		time1.Set(KTime1);
		time2.Set(KTime2);

		time.HomeTime();
		datetime = time.DateTime();

		//datetime.Set(0,0,0,datetime.Hour(),datetime.Minute(),0,0)
		timebuffer.Copy(_L(""));
		timebuffer.Append(KTime);
		
		if(datetime.Hour() < 10)
			timebuffer.Append(_L("0"));

		timebuffer.AppendNum(datetime.Hour());

		if(datetime.Minute() < 10)
			timebuffer.Append(_L("0"));

		timebuffer.AppendNum(datetime.Minute());
		timebuffer.Append(_L("00"));
		
		timebuffer.Append(KTime3);

		time.Set(timebuffer);

		iAppUi.LogText.Copy(_L("CT UploadContacts1\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);

		/*
		TInt iCurrSync;
		TInt iNumSync;

		iAppUi.iContactsHandler->GetSyncParams(iCurrSync, iNumSync);

		if (iNumSync == 0 && iAppUi.iContactsHandler->iChangesDB->CountRecords() == 0)
		{
			iAppUi.LogText.Copy(_L("CT UploadContacts2\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
		}
		else 
		{
			iAppUi.LogText.Copy(_L("CT UploadContacts3\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);
			
			iAppUi.UpdateContacts(0);			
		}
		*/

		if(syncstate == 0)
		{
			iAppUi.LogText.Copy(_L("Sync State\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

			if(time >= time1 && time <= time2)
			{
				iAppUi.LogText.Copy(_L("Count Increased\n"));
				iAppUi.WriteLogFile(iAppUi.LogText);

				contactsday++;		
				syncstate = 0;
				iAppUi.SaveConatctsTimeToDB(contactsday, days, syncstate);
			}
		}
	}
}

void CContactsTimer::ReportError(MEngineNotifier::TErrorType aErrorType, TInt aErrorCode)
{
    // No recovery or retries are attempted in this example so we just
    // disconnect and inform the user
    Disconnect();
}

void CContactsTimer::ResponseReceived(const TDesC8& aBuffer)
{
}

void CContactsTimer::ChangeStatus(TWaitState aNewStatus)
{
    iEngineStatus = aNewStatus;
}

void CContactsTimer::Print(const TDesC& aDes)
{
}
