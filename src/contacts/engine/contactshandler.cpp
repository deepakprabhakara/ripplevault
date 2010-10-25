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
 Module Name				: ContactsHandler.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Main file for Contacts Backup, handles all logic. Also listens to contacts change events
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include <utf.h>
#include <bautils.h>
#ifndef __UIQ__
#include <aknnotewrappers.h>
#include <aknnotedialog.h>
#endif

#include "RippleVaultAppUi.h"
#include "ContactsDBEngine.h"
#include "ContactsHandler.h"
#include "RecordContactDB.h"
#include "ContactsSocketsEngineWriter.h"
#include "List.h"

CContactsHandler* CContactsHandler::NewL(CRippleVaultAppUi *aAppUi)
{
    CContactsHandler* tmp = new (ELeave)CContactsHandler(aAppUi);
    CleanupStack::PushL(tmp);
    tmp->ConstructL();
    CleanupStack::Pop();
    return tmp;
}


// ---------------------------------------------------------------------------
// CContactsHandler::ConstructL()
//
// Construction, Initialize ChangesDB.
// ---------------------------------------------------------------------------
void CContactsHandler::ConstructL()
{
   // Name of database file in the application directory
	//_LIT(KDatabaseFile,"changesdb.dat");
	TBuf<128> databasefile = CRippleVaultAppUi::ApplicationDriveAndPath();
	databasefile.Append(KContactsDBFile);
	iChangesDB = CChangesDB::NewL();
	iSocketsEngine = NULL;
	iAppUi->iContactsCount = 0;

	// A retry loop count for GPRS/socket related problems
	iRetryCount = MAX_RETRIES;
	TInt leaveCode = iChangesDB->CreateDbL(databasefile);
	if (leaveCode != KErrNone)
	{
		iChangesDB->OpenDbL(databasefile);
	}
	CActiveScheduler::Add(this);
	iChangesCount = 0;
	iPendingSync = 0;
	iNumSync = 0;
	iCurrSync = 1;
	iSMS = EFalse;
	iContactsChanges = 0;
	
	TInt roam;
	TInt upload;
	iAppUi->GetContactsSettingsFromDB(roam,upload);

	iContactsRoaming = roam;
	iContactsUpload = upload;	
}

// ---------------------------------------------------------------------------
// CContactsHandler::CContactsHandler
//
// Constructor
// ---------------------------------------------------------------------------
CContactsHandler::CContactsHandler(CRippleVaultAppUi *aAppUi) : CActive(EPriorityStandard)
{
	iContactState = KStateNone;
 	iAppUi = aAppUi;
}

// ---------------------------------------------------------------------------
// CContactsHandler::CContactsHandler
//
// Destructor
// ---------------------------------------------------------------------------
CContactsHandler::~CContactsHandler()
{
	Cancel();
    if(iChangesDB)
	{
        iChangesDB->Close();
        delete iChangesDB;
        iChangesDB = NULL;
    }
	if (iSocketsEngine != NULL) 
	{
		delete iSocketsEngine;
		iSocketsEngine = NULL;
	}
}

// ---------------------------------------------------------------------------
// CContactsHandler::DoCancel
//
// Cancel the request
// ---------------------------------------------------------------------------
void CContactsHandler::DoCancel()
{
}

// ---------------------------------------------------------------------------
// CContactsHandler::RunL
//
// Execute the state transition based on the event.
// ---------------------------------------------------------------------------
void CContactsHandler::RunL()
{
#ifdef __LOGME__
	iAppUi->LogText.Copy(_L("CHandler::RunL\n"));
	iAppUi->WriteLogFile(iAppUi->LogText);
#endif

//    TInt result;
	HBufC8 *buffer;
	TInt len;
	TBuf8<10> lbuf;
	HBufC8* tmpbuf;

    if (iStatus != KErrNone)
    {
        return;
    }
	switch(iContactState)
	{
	case KStateNone:
		break;
	case KDBChange:
#ifdef __LOGME__
	iAppUi->LogText.Copy(_L("CHandler::KDBChange\n"));
	iAppUi->WriteLogFile(iAppUi->LogText);
#endif

		GetSyncParams(iCurrSync, iNumSync);
		if (iNumSync == 0) 
			SendComplete();
		else {
			ProcessEventQueue();

			if (IsNetworkInProgress())
			{
				iContactState = KStateNone;
				//iRetryCount = MAX_RETRIES;
				//Notify();
			}
			else
				MoreToUpload();
		}

        break;
	case KDBStore: { // initiate connection request
//		TBool existsRecord;
#ifdef __LOGME__
	iAppUi->LogText.Copy(_L("CHandler::KDBStore\n"));
	iAppUi->WriteLogFile(iAppUi->LogText);
#endif

		CRecordContactDB *record;
		CList *ptr;
		iChangesCount = 0;
		CList* recordlist;
		TInt count, skipRecords, numRecords;

		buffer = HBufC8::NewL(8 * 1024);
		CleanupStack::PushL(buffer);

		/*iBuffer.Append(_L("<imei>"));
		iBuffer.Append(iAppUi->PhoneImei());
		iBuffer.Append(_L("</imei>"));
		iBuffer.Append(_L("<state>"));
		iBuffer.AppendNum(iAppUi->iState);
		iBuffer.Append(_L("</state>"));*/

		TBuf<20> msgType;

		iBuffer.Copy(_L("<root>"));
		iChangesDB->GetMessageType(msgType);
		CreateHeaderXML(msgType);
		iBuffer.Append(_L("<b>"));

		numRecords = GetNumRecords(iCurrSync, iNumSync);
		skipRecords = (iCurrSync - 1) * MAX_CONTACTS;
		/*if (numRecords <= 0) {
			iContactState = KStateNone;
			iRetryCount = MAX_RETRIES; // reset
			Notify();
			break;
		}*/

		recordlist = iChangesDB->GetLocalRecords(skipRecords, numRecords);
		ptr = recordlist;

		count = recordlist->Count();

		/*TBuf8<60> buf;
		buf.Copy(_L8("In KDBStore1\r\n"));
		iAppUi->Logger(buf);
		*/
#ifdef __LOGME__
			iAppUi->LogText.Copy(_L("RECORDS=\n"));
			iAppUi->WriteLogFile(iAppUi->LogText);

			iAppUi->LogText.Copy(_L(""));
			iAppUi->LogText.AppendNum(count);
			iAppUi->WriteLogFile(iAppUi->LogText);
#endif

		while(count) 
		{
			record = (CRecordContactDB*)ptr->GetContent();

			/*TBuf8<250> buf;
			buf.Copy(_L("Sending: "));
			buf.AppendNum((TInt)record->iContactId);
			buf.Append(_L(" "));
			buf.Append(*(record->iLastName));
			buf.Append(_L("\r\n"));
			iAppUi->Logger(buf);
			*/

			buffer->Des().Copy(_L(""));

			GetXMLData(buffer->Des(), record);

#ifdef __LOGME__
			iAppUi->LogText.Copy(_L("getxmlTEMPC=\n"));
			iAppUi->WriteLogFile(iAppUi->LogText);

			iAppUi->LogText.Copy(_L(""));
			iAppUi->LogText.AppendNum(iAppUi->iContactsCount);
			iAppUi->WriteLogFile(iAppUi->LogText);
#endif

			record->Cleanup();
			ptr = ptr->next;
			count--;

			/*if(iBuffer.Length() + buffer->Des().Length() > (MAX_BUFFER_LEN - 200)) // 200 bytes needed for http header
			{
				continue;
			}*/
			iBuffer.Append(*buffer);
			iChangesCount++;
		}
		if (iChangesCount || iPendingSync)
		{
			//TBuf8<60> buf;
			/*buf.Copy(_L8("In KDBStore\r\n"));
			iAppUi->Logger(buf);
			*/

			iContactState = KDBConnected;
			if (iChangesCount == 0)
				iBuffer.Append(_L("<add></add><update></update><delete></delete>"));
			// CHECK THIS
			Notify();
		}
		else
		{
			iContactState = KStateNone;
			iRetryCount = MAX_RETRIES; // reset

			if (iSocketsEngine != NULL)
			{
				iAppUi->iContactsCount = 0;
				delete iSocketsEngine;
				iSocketsEngine = NULL;
			}
		}
		recordlist->RemoveAllAndContent();
		CleanupStack::PopAndDestroy(1); // buffer

		iBuffer.Append(_L("</b>"));
		iBuffer.Append(_L("</root>"));

		//Notify();
		break;
		}
    case KDBConnected: {
        // construct and send data
		/*TBuf8<60> buff;
		buff.Copy(_L8("In KDBConnected\r\n"));
		iAppUi->Logger(buff);

		if (iPendingSync) {
			//TBuf8<50> buff;
			buff.Append(_L("sms processed"));
			buff.Append(_L(" "));
			buff.Append(iAppUi->ServerAddr());
			buff.Append(_L(" "));
			buff.AppendNum(iAppUi->ServerPort());
			buff.Append(_L("\r\n"));
			iAppUi->Logger(buff);
		}*/

		iPendingSync = 0;
		if (iSocketsEngine != NULL)
		{
			iAppUi->iContactsCount = 0;
			delete iSocketsEngine;
			iSocketsEngine = NULL;
		}
		/*if (iChangesCount == 0) // no local changes
		{
			TBuf8<50> buf;
			buf.Copy(_L("sent due to sms\r\n"));
			iAppUi->Logger(buf);

			// We have to send a poll here

			iBuffer.Copy(_L("<imei>"));
			iBuffer.Append(iAppUi->PhoneImei());
			iBuffer.Append(_L("</imei>"));
			iBuffer.Append(_L("<state>"));
			iBuffer.AppendNum(iAppUi->iState);
			iBuffer.Append(_L("</state>"));
			iBuffer.Append(_L("<add></add><update></update><delete></delete>"));
		}*/
		iBuffer.Append(_L("\r\n"));

		len = iBuffer.Length();
		lbuf.Num(len);

		//iAppUi->PrintToFile(iBuffer);
#ifndef WAP_PT
		tmpbuf = iBuffer.AllocL();
//		iAppUi->PrintToFile(tmpbuf);
		iBuffer.Copy(_L(""));
		//iBuffer.Append(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
		iBuffer.Append(_L("GET "));
		iBuffer.Append(iAppUi->iContactsUrl);
		//iBuffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
		iBuffer.Append(_L(" HTTP/1.0\r\nHOST:"));
		iBuffer.Append(iAppUi->iServerAddr);
		iBuffer.Append(_L("\r\nPORT:"));
		iBuffer.AppendNum(iAppUi->iPort);
		iBuffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));

		
		iBuffer.Append(lbuf);
		iBuffer.Append(_L("\r\n\r\n"));
		iBuffer.Append(*tmpbuf);
		delete tmpbuf;
#endif
		if (iSocketsEngine != NULL)
		{
			delete iSocketsEngine;
			iSocketsEngine = NULL;
		}

//check for roaming
		/*
		TInt network;	
		network = iAppUi->CheckNetwork();

		if(network == 5)
		{
			if(iContactsRoaming == 1)
			{
				//deepak change made
				iSocketsEngine = CContactsSocketsEngineWriter::NewL(*iAppUi, iAppUi->ServerAddr(),iAppUi->ServerPort());
				iSocketsEngine->servlet_data = iBuffer.AllocL();
				iSocketsEngine->Start();
			}
		}
		else
		{
				iSocketsEngine = CContactsSocketsEngineWriter::NewL(*iAppUi, iAppUi->ServerAddr(),iAppUi->ServerPort());
				iSocketsEngine->servlet_data = iBuffer.AllocL();
				iSocketsEngine->Start();
		}
		*/
#ifdef __LOGME__
		iAppUi->LogText.Copy(_L("RunL KDBConnected\n"));
		iAppUi->WriteLogFile(iAppUi->LogText);
#endif

		if(iContactsChanges == 1)
		{
			iContactsChanges = 0;		
			
			//iContactState = KStateNone;

			iAppUi->LogText.Copy(_L("CH ContactChanges\n"));
			iAppUi->WriteLogFile(iAppUi->LogText);
			
			if(iContactsUpload == 0)
			{
				iAppUi->LogText.Copy(_L("CH ContactChanges1\n"));
				iAppUi->WriteLogFile(iAppUi->LogText);

				iAppUi->iSynchronize = 0;
				iAppUi->UpdateContacts(1);//for contacts edited
			}
			else
			{
				iContactState = KStateNone;
				iAppUi->iContactsCount = 0;				
			}
		}
		else
		{
			iSocketsEngine = CContactsSocketsEngineWriter::NewL(*iAppUi, iAppUi->ServerAddr(),iAppUi->ServerPort());
			iSocketsEngine->servlet_data = iBuffer.AllocL();
			iSocketsEngine->Start();
		}

		break;
		}
	default:
		// should not come here
		break;
	}
}

TInt CContactsHandler::RunError(TInt aError)
{
	iContactState = KStateNone;

/*	TBuf8<60> buf;
	buf.Copy(_L8("CContactsHandler Leave Error: "));
	buf.AppendNum(aError);
	buf.Append(_L("\r\n"));
	iAppUi->WriteLogFile(buf);
*/
	return KErrNone;
}

void CContactsHandler::SendAgain()
{
	iAppUi->iContactsCount = 0;

	iCurrSync = 1;
	SetSyncParams(iCurrSync, iNumSync);

	//iRetryCount = MAX_RETRIES; // no need to retry

	if (iRetryCount > 0) {
		iRetryCount--;
		MoreToUpload();
	}
	else {
		TBuf8<60> buf;

#ifdef __LOGME__
		buf.Copy(_L8("Invalid Request Id\r\n"));
		iAppUi->WriteLogFile(buf);
#endif
	}

}

void CContactsHandler::SendCompleteI(TInt aPacketNo)
{
	TInt ccount = 0;
	iAppUi->GetTempContactsUploadSettingsFromDB(ccount);
	
	//iAppUi->iUploadContacts = iAppUi->iContactsCount;

	iAppUi->iContactsCount = ccount + iAppUi->iContactsCount;

#ifdef __LOGME__
			iAppUi->LogText.Copy(_L("\niContactsCount="));
			iAppUi->WriteLogFile(iAppUi->LogText);

			iAppUi->LogText.Copy(_L(""));
			iAppUi->LogText.AppendNum(iAppUi->iContactsCount);
			iAppUi->WriteLogFile(iAppUi->LogText);
#endif

	iAppUi->SaveTempContactsUploadSettingsToDB(iAppUi->iContactsCount);
	iAppUi->iContactsCount = 0;


	iAppUi->NetworkErrorNotification(0);

	if (aPacketNo != iCurrSync)
	{
/*		TBuf8<60> buf;
		buf.Copy(_L8("CContactsHandler Sync Error: "));
		buf.AppendNum(iCurrSync);
		buf.Append(_L(" "));
		buf.AppendNum(aPacketNo);
		buf.Append(_L("\r\n"));
		iAppUi->WriteLogFile(buf);*/
		return;
	}

	/*if (iCurrSync == 2)
		User::Exit(EEikCmdExit);
	*/
	
	iCurrSync = aPacketNo + 1;
	SetSyncParams(iCurrSync, iNumSync);

	iRetryCount = MAX_RETRIES; // no need to retry

	MoreToUpload();
}

void CContactsHandler::SendCompleteN()
{
	TInt ccount = 0;
	iAppUi->GetTempContactsUploadSettingsFromDB(ccount);

	//iAppUi->iUploadContacts = iAppUi->iContactsCount;

	iAppUi->iContactsCount = ccount + iAppUi->iContactsCount;

#ifdef __LOGME__
			iAppUi->LogText.Copy(_L("\niContactsCount="));
			iAppUi->WriteLogFile(iAppUi->LogText);

			iAppUi->LogText.Copy(_L(""));
			iAppUi->LogText.AppendNum(iAppUi->iContactsCount);
			iAppUi->WriteLogFile(iAppUi->LogText);
#endif

	iAppUi->SaveTempContactsUploadSettingsToDB(iAppUi->iContactsCount);
	iAppUi->iContactsCount = 0;


	iAppUi->NetworkErrorNotification(0);
	/*if (iChangesCount > 0)
		iChangesDB->PopFromTop(iNumSync);
		*/

	/*iCurrSync = 1;
	iNumSync = 1;

	SetSyncParams(iCurrSync, iNumSync);
	*/
	

	/*TTime ctime;
	HBufC* currTime;
	
	ctime.HomeTime();
	currTime = iAppUi->GetTimeStamp(ctime);

	TBuf<128> syncTime;
	TBuf<128> candTime;

	iChangesDB->GetSyncTimes(syncTime, candTime);
	iChangesDB->SetSyncTimes(candTime, *currTime);
	*/

	iRetryCount = MAX_RETRIES; // no need to retry

	if (iSocketsEngine != NULL)
	{
		iAppUi->iContactsCount = 0;
		delete iSocketsEngine;
		iSocketsEngine = NULL;
	}
	
	Poll(1, 1);
}

void CContactsHandler::SendComplete()
{
	iAppUi->NetworkErrorNotification(0);
	/*TBuf<30> tempstr;
	tempstr.Copy(_L("sync"));
	
	iChangesDB->SetMessageType(tempstr);
	*/
#ifdef __LOGME__
	iAppUi->LogText.Copy(_L("CHandler::SendComplete\n"));
	iAppUi->WriteLogFile(iAppUi->LogText);
#endif

	ProcessEventQueue();

	iChangesDB->PopFromTop(iNumSync);
	
	iNumSync = iChangesDB->CountRecords();

	iCurrSync = 1;

	SetSyncParams(iCurrSync, iNumSync);

	TTime ctime;
	HBufC* currTime;
	
	ctime.HomeTime();
	currTime = iAppUi->GetTimeStamp(ctime);

	TBuf<128> syncTime;
	TBuf<128> candTime;

	iChangesDB->GetSyncTimes(syncTime, candTime);
	iChangesDB->SetSyncTimes(candTime, *currTime);

	iRetryCount = MAX_RETRIES; // no need to retry

	if (!iPendingSync)
		iSMS = EFalse;

	if (iNumSync > 0 || iPendingSync) {
		iContactState = KDBStore;
		iAppUi->iContactsTimerState = 1;
		Notify();
	}
	else {
		iContactState = KStateNone;
		iRetryCount = MAX_RETRIES;
		iAppUi->iContactsTimerState = 0;
	}

	TTime btime;
	btime.HomeTime();

	iAppUi->iPhotosDB->UpdateBackupTimestampToDB(btime);
	
	TInt ccount = 0;
	iAppUi->GetTempContactsUploadSettingsFromDB(ccount);

#ifdef __LOGME__
			iAppUi->LogText.Copy(_L("TEMPC=\n"));
			iAppUi->WriteLogFile(iAppUi->LogText);

			iAppUi->LogText.Copy(_L(""));
			iAppUi->LogText.AppendNum(ccount);
			iAppUi->WriteLogFile(iAppUi->LogText);
#endif
	TInt ccount1 = 0;
	iAppUi->GetContactsUploadSettingsFromDB(ccount1);

#ifdef __LOGME__
			iAppUi->LogText.Copy(_L("COUNTSOFAR=\n"));
			iAppUi->WriteLogFile(iAppUi->LogText);

			iAppUi->LogText.Copy(_L(""));
			iAppUi->LogText.AppendNum(ccount1);
			iAppUi->WriteLogFile(iAppUi->LogText);
#endif

	ccount = ccount + ccount1;

	iAppUi->iUploadContacts = ccount;
	iAppUi->SaveContactsUploadSettingsToDB(ccount);	

	iAppUi->iContactsCount = 0;

	iAppUi->SaveTempContactsUploadSettingsToDB(iAppUi->iContactsCount);
	
	if(iContactsUpload == 1)
	{
		TInt contactsday;
		TInt days=0;
		TInt syncstate = 0;

		iAppUi->GetConatctsTimeFromDB(contactsday, days, syncstate);

		contactsday = 0;
		syncstate = 0;

		iAppUi->SaveConatctsTimeToDB(contactsday, days, syncstate);	
	}

	//Notify();
	if (iSocketsEngine != NULL)
	{
		delete iSocketsEngine;
		iSocketsEngine = NULL;

		iAppUi->CloseConnection();
	}	
	
}

void CContactsHandler::MoreToUpload()
{
	/*TBuf<100> msg;
	
	msg.Copy(_L("Uploads: "));
	msg.AppendNum(iChangesCount);

#ifndef WAP_PT

#ifndef __UIQ__
	msg.Append(_L("\r\n"));
#else
	msg.Append(_L("\n"));
#endif
	msg.Append(_L("Downloads: "));
	msg.AppendNum(aDownloadCount);
#ifndef __UIQ__
			CAknInformationNote* note = new(ELeave) CAknInformationNote;
			note->SetTimeout(CAknNoteDialog::ELongTimeout);
			note->ExecuteLD(msg);
#else
			CEikonEnv::Static ()->InfoMsgWithDuration(msg, 3000000);
#endif

#endif // Wap Pt
    */

	/*TBuf8<60> buf;
	buf.Copy(_L8("In MoreToUpload\r\n"));
	iAppUi->Logger(buf);
	*/

	if (iSMS) 
		iPendingSync = 1;

	iContactState = KDBStore;
	Notify();
	if (iSocketsEngine != NULL)
	{
		iAppUi->iContactsCount = 0;
		delete iSocketsEngine;
		iSocketsEngine = NULL;
	}
}

void CContactsHandler::SendError()
{
	iChangesCount = 0;
	iAppUi->iContactsCount = 0;

	iAppUi->iUploadContacts = 0;

	if (iSMS) {
		/*TBuf8<60> buf;
		buf.Copy(_L8("Setting iPendingSync\r\n"));
		iAppUi->Logger(buf);
		*/

		iPendingSync = 1;
	}

	if (iRetryCount > 0)
	{
		/*TBuf8<60> buf;
		buf.Copy(_L8("Retrying\r\n"));
		iAppUi->Logger(buf);
		*/

		iRetryCount--;
		MoreToUpload();
	}
	else
	{
		//TTime time;
		//time.HomeTime();
	
//		TBuf8<100> buf;
		//TBuf<50> timeBuf;
		//time.FormatL(timeBuf, TPtrC(_L("%Y%M%D%3%2%1:%H%T%S.")));

//		buf.Copy(timeBuf);
//		buf.Append(_L8(": Network error\r\n"));
//		iAppUi->WriteLogFile(buf);

		if (iSocketsEngine != NULL)
		{
			iAppUi->iContactsCount = 0;
			delete iSocketsEngine;
			iSocketsEngine = NULL;

			iAppUi->CloseConnection();
		}
		iRetryCount = MAX_RETRIES;
		iContactState = KStateNone;
	}
}

void CContactsHandler::ProcessEventQueue()
{
	TInt result;

	while(1)
	{
		TContactDbObserverEvent *event;
		iContactEvents.RemoveFromQueue((void **)&event);

		if (event != NULL)
		{
			// retrieve from ContactsDB, store to ChangesDB
			//iContactState = KDBStore;
			result = ProcessContactsChange(*event);
			if (result == KErrNotFound || result == KErrNone)
			{
				User::Free(event);
			}
			else if (result == KErrInUse)
			{
				//iContactEvents.AddToQueue((void *)event);
				User::Free(event);
			}
			else
			{
				// error. shd not reach here
			}
		}
		else
		{
			break;
		}
	} // while (1)
}

// MContactDbObserver method
// ---------------------------------------------------------------------------
// CContactsHandler::HandleDatabaseEventL
//
// ContactDB Callback method
// ---------------------------------------------------------------------------
void CContactsHandler::HandleDatabaseEventL(TContactDbObserverEvent aEvent)
{
	// queue the event and notify the active object
    TContactDbObserverEvent* event = (TContactDbObserverEvent *)User::AllocL(sizeof(TContactDbObserverEvent));
    event->iContactId = aEvent.iContactId;
    event->iType = aEvent.iType;

    iContactEvents.AddToQueue(event);
	if (iContactState == KStateNone)
	{
		iContactsChanges = 1;
		iContactState = KDBChange;
	    Notify();
	}
}

// ---------------------------------------------------------------------------
// CContactsHandler::Notify()
// 
// Signal the active object
// ---------------------------------------------------------------------------
void CContactsHandler::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	
	if (!IsActive())
    SetActive();
}

// ---------------------------------------------------------------------------
// CContactsHandler::ProcessContactsChange()
// 
// Get the contact db change and execute it.
// ---------------------------------------------------------------------------
TInt CContactsHandler::ProcessContactsChange(const TContactDbObserverEvent& aEvent)
{
	TInt contactid=aEvent.iContactId;
	CContactDatabase* cdb = iAppUi->ContactDataBase();

	TInt error = KErrNone;

	switch (aEvent.iType)
	{
	case EContactDbObserverEventContactAdded:
    {
		CContactItem* contact=NULL;
		TBuf<256> ts;

		if (!iChangesDB->ExistsRecursedElement(contactid, 0))
		{
			TRAP(error, contact = cdb->ReadContactL(contactid));
			if (error == KErrNone)
			{
				CRecordContactDB *record = CreateAddedRecordContactDB(*contact);
				iChangesDB->AddRecordFromContactsDB(record);
				iChangesDB->AddId(contactid);

				ts.Copy(*(record->iTimeStamp));
				iAppUi->AddTimeStampToFileL(&ts);

				delete record;
		
				cdb->CloseContactL(contactid);
				delete contact;
			}
		}
		break;
    }
	case EContactDbObserverEventContactChanged:
    {
		CContactItem* contact=NULL;
		if (!iChangesDB->ExistsRecursedElement(contactid, 1))
		{
			TRAP(error, contact = cdb->ReadContactL(contactid));
			if (error == KErrNone)
			{
				CRecordContactDB *record = CreateUpdatedRecordContactDB(*contact);
				iChangesDB->AddRecordFromContactsDB(record);

				TBuf<256> ts;
				ts.Copy(*(record->iTimeStamp));
				iAppUi->AddTimeStampToFileL(&ts);

				delete record;
     	
				cdb->CloseContactL(contactid);
				delete contact;
			}
		}
		break;
    }
	case EContactDbObserverEventContactDeleted:
    {

		if (!iChangesDB->ExistsRecursedElement(contactid, 2))
		{
			CRecordContactDB *record = CreateDeletedRecordContactDB(contactid);
			iChangesDB->AddRecordFromContactsDB(record);
			iChangesDB->DeleteId(contactid);

			TBuf<256> ts;
			ts.Copy(*(record->iTimeStamp));
			iAppUi->AddTimeStampToFileL(&ts);

			delete record;
		}
		break;
    }
	default:
        break;
	}
    return error;
}

HBufC* CContactsHandler::GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField)
{
    TInt index = aFieldSet.Find(aField);
    HBufC *value = NULL;
    if ((index >= 0) && (index < aFieldSet.Count()))
    {
   	    CContactItemField& textfield = aFieldSet[index];
        CContactTextField* textValue = textfield.TextStorage();

	    value = textValue->Text().AllocLC();
        CleanupStack::Pop();
    }
    return value;
}

// ---------------------------------------------------------------------------
// CContactsHandler::CreateAddedRecordContactDB()
// 
// Create a structure from contactdb record for adding into the chagesdb
// ---------------------------------------------------------------------------
CRecordContactDB* CContactsHandler::CreateAddedRecordContactDB(const CContactItem& aContact)
{
    CContactItemFieldSet& fieldSet = aContact.CardFields();

    CRecordContactDB* chrecord = new CRecordContactDB();

    // Get the first name
    // TODO: return values are alloced, so perform delete when writing to changesdb
	chrecord->iContactId = aContact.Id();
	chrecord->iCreator = 101;
	chrecord->iChangeType = 0;
    TTime timevalue = aContact.LastModified();
	chrecord->iTimeStamp = iAppUi->GetTimeStamp(timevalue);

    chrecord->iFirstName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldGivenName);
    chrecord->iLastName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldFamilyName);
	chrecord->iCompanyName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldCompanyName);
	//chrecord->iEmail = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldEMail);
	//chrecord->iJobTitle = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldJobTitle);

	for (int i = 0; i < fieldSet.Count(); i++) 
	{
		// Titles
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldJobTitle) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTITLE)) 
		{
				chrecord->iJobTitles->Add(&fieldSet[i]);
				continue;
		}
		// Home Faxes
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldFax) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX)) 
		{
				chrecord->iHomeFaxes->Add(&fieldSet[i]);
				continue;
		}
		// Work Faxes
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldFax) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX)) {
				chrecord->iWorkFaxes->Add(&fieldSet[i]);
				continue;
		}
		// Faxes
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldFax) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX)) {
				chrecord->iFaxes->Add(&fieldSet[i]);
				continue;
		}
		// Home mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber)
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iHomeMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Work mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iWorkMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Home Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iHomeTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Work Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iWorkTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Telephones
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Home Emails
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldEMail) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET)) {
				chrecord->iHomeEmails->Add(&fieldSet[i]);
				continue;
		}
		// Work Emails
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldEMail) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET)) {
				chrecord->iWorkEmails->Add(&fieldSet[i]);
				continue;
		}
		// Emails
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldEMail) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET)) {
				chrecord->iEmails->Add(&fieldSet[i]);
				continue;
		}
	}

    return chrecord;
}

// ---------------------------------------------------------------------------
// CContactsHandler::CreateUpdatedRecordContactDB()
// 
// Create a structure from contactdb record for updating the chages
// ---------------------------------------------------------------------------
CRecordContactDB* CContactsHandler::CreateUpdatedRecordContactDB(const CContactItem& aContact)
{
    CContactItemFieldSet& fieldSet = aContact.CardFields();

    CRecordContactDB* chrecord = new CRecordContactDB();

    // Get the first name
    // TODO: return values are alloced, so perform delete when writing to changesdb
	chrecord->iContactId = aContact.Id();
	chrecord->iCreator = 101;
	chrecord->iChangeType = 1;
    TTime timevalue = aContact.LastModified();
	chrecord->iTimeStamp = iAppUi->GetTimeStamp(timevalue);

    chrecord->iFirstName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldGivenName);
    chrecord->iLastName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldFamilyName);
	chrecord->iCompanyName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldCompanyName);

	//chrecord->iEmail = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldEMail);
	//chrecord->iJobTitle = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldJobTitle);

	for (int i = 0; i < fieldSet.Count(); i++) {
		// Titles
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldJobTitle) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTITLE)) {
				chrecord->iJobTitles->Add(&fieldSet[i]);
				continue;
		}
		// Home Faxes
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldFax) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX)) {
				chrecord->iHomeFaxes->Add(&fieldSet[i]);
				continue;
		}
		// Work Faxes
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldFax) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX)) {
				chrecord->iWorkFaxes->Add(&fieldSet[i]);
				continue;
		}
		// Faxes
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldFax) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapFAX)) {
				chrecord->iFaxes->Add(&fieldSet[i]);
				continue;
		}
		// Home mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iHomeMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Work mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iWorkMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Home Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iHomeTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Work Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iWorkTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Telephones
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Home Emails
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldEMail) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET)) {
				chrecord->iHomeEmails->Add(&fieldSet[i]);
				continue;
		}
		// Work Emails
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldEMail) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK) 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET)) {
				chrecord->iWorkEmails->Add(&fieldSet[i]);
				continue;
		}
		// Emails
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldEMail) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapEMAILINTERNET)) {
				chrecord->iEmails->Add(&fieldSet[i]);
				continue;
		}
	}

    return chrecord;
}

// ---------------------------------------------------------------------------
// CContactsHandler::CreateDeletedRecordContactDB()
// 
// Create a structure from contactdb record for deleting from the chagesdb
// ---------------------------------------------------------------------------
CRecordContactDB* CContactsHandler::CreateDeletedRecordContactDB(TContactItemId contactid)
{
    //CContactItemFieldSet& fieldSet = aContact.CardFields();

    CRecordContactDB* chrecord = new CRecordContactDB();

    // Get the first name
    // TODO: return values are alloced, so perform delete when writing to changesdb
	chrecord->iContactId = contactid;
	chrecord->iCreator = 101;
	chrecord->iChangeType = 2;

	TDateTime janNineteenSeventy(1970,EJanuary,0,00,00,00,000000);
	TTime timejan(janNineteenSeventy);
	TBuf<256> timestamp;

	RFs fs;

	CContactDatabase *database;	// Contact database
	TBuf<256> cdbFileName; // Contact db file name

	database = iAppUi->ContactDataBase();

	fs.Connect();

	if (database->FindContactFile(cdbFileName)) {
				
		// Modif time of contacts DB
		TTime cdbTime; 
		fs.Modified(cdbFileName, cdbTime);

		TTimeIntervalMicroSeconds interval = cdbTime.MicroSecondsFrom(timejan);
		TInt64 k;
		k=interval.Int64()/1000;
		timestamp.Num(k);
	}

	chrecord->iTimeStamp = timestamp.AllocL();

	TBuf<2> buf;
	buf.Copy(_L(""));
	chrecord->iFirstName = buf.AllocL();
    chrecord->iLastName = buf.AllocL();
	chrecord->iCompanyName = buf.AllocL();

    return chrecord;
}

// ---------------------------------------------------------------------------
// CContactsHandler::CreateAddUpdateRecordContactDB()
// 
// Create a structure from contactdb record for adding/updating
// ---------------------------------------------------------------------------
CRecordContactDB* CContactsHandler::CreateAddUpdateRecordContactDB(const CContactItem& aContact)
{
    CContactItemFieldSet& fieldSet = aContact.CardFields();

    CRecordContactDB* chrecord = new CRecordContactDB();

    // Get the first name
    // TODO: return values are alloced, so perform delete when writing to changesdb
	chrecord->iContactId = aContact.Id();
	chrecord->iCreator = 101;
	chrecord->iChangeType = 3;
	TTime timevalue = aContact.LastModified();
	chrecord->iTimeStamp = iAppUi->GetTimeStamp(timevalue);

    chrecord->iFirstName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldGivenName);
    chrecord->iLastName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldFamilyName);
	chrecord->iCompanyName = GetTextFieldFromFieldSet(fieldSet, KUidContactFieldCompanyName);

	for (int i = 0; i < fieldSet.Count(); i++) {
		// Home mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iHomeMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Work mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iWorkMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Mobile nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapCELL)) {
				chrecord->iMobiles->Add(&fieldSet[i]);
				continue;
		}
		// Home Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapHOME)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iHomeTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Work Telephone nos
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapWORK)
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iWorkTelephones->Add(&fieldSet[i]);
				continue;
		}
		// Telephones
		if (fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldPhoneNumber) 
			&& fieldSet[i].TextStorage()->Text().Length() > 0 
			&& fieldSet[i].ContentType().ContainsFieldType(KUidContactFieldVCardMapTEL)) {
				chrecord->iTelephones->Add(&fieldSet[i]);
				continue;
		}
	}

    return chrecord;
}

// ---------------------------------------------------------------------------
// CContactsHandler::CreateIdListRecordContactDB()
// 
// Create a structure from contactdb record for sending idlist
// ---------------------------------------------------------------------------
CRecordContactDB* CContactsHandler::CreateIdListRecordContactDB(CList *idList)
{

    CRecordContactDB* chrecord = new CRecordContactDB();

	chrecord->iCreator = 101;
	chrecord->iChangeType = 4;

	chrecord->iIdList->RemoveAllAndContent();
	chrecord->iIdList = idList;
	
    return chrecord;
}

// ---------------------------------------------------------------------------
// CContactsHandler::ReplaceTag()
// 
// Convert special characters like < so that server parser does not fail
// ---------------------------------------------------------------------------
void CContactsHandler::ReplaceTag(TDes& data, TChar fromTag, TPtrC toTag)
{
	if (data.Length() <= 0)
		return;

	TInt len = data.Length();

	for (TInt i = 0; i < len; i++)
	{
		if (data[i] == fromTag) {
			data.Delete(i, 1);
			data.Insert(i, toTag);
			i += toTag.Length() - 1;
		}
		len = data.Length();
	}
}

// ---------------------------------------------------------------------------
// CContactsHandler::ConvertXMLTags()
// 
// Convert special characters like < so that server parser does not fail
// ---------------------------------------------------------------------------
void CContactsHandler::ConvertXMLTags(TDes& data)
{
	ReplaceTag(data, '&', _L("&amp;"));
	ReplaceTag(data, '<', _L("&lt;"));
	ReplaceTag(data, '>', _L("&gt;"));
	ReplaceTag(data, '\"', _L("&quot;"));
	ReplaceTag(data, '\'', _L("&apos;"));
	//ReplaceTag(data, 128, _L("&euro;")); // or 0x20ac
}

// ---------------------------------------------------------------------------
// CContactsHandler::ReplaceTag()
// 
// Convert special characters like < so that server parser does not fail
// ---------------------------------------------------------------------------
void CContactsHandler::ReplaceTag(TDes8& data, TChar fromTag, TPtrC8 toTag)
{
	if (data.Length() <= 0)
		return;

	TInt len = data.Length();

	for (TInt i = 0; i < len; i++)
	{
		if (data[i] == fromTag) {
			data.Delete(i, 1);
			data.Insert(i, toTag);
			i += toTag.Length() - 1;
		}
		len = data.Length();
	}
}

// ---------------------------------------------------------------------------
// CContactsHandler::ConvertXMLTags()
// 
// Convert special characters like < so that server parser does not fail
// ---------------------------------------------------------------------------
void CContactsHandler::ConvertXMLTags(TDes8& data)
{
	ReplaceTag(data, '&', _L8("&amp;"));
	ReplaceTag(data, '<', _L8("&lt;"));
	ReplaceTag(data, '>', _L8("&gt;"));
	ReplaceTag(data, '\"', _L8("&quot;"));
	ReplaceTag(data, '\'', _L8("&apos;"));
	//ReplaceTag(data, 128, _L8("&euro;")); // or 0x20ac
}

// ---------------------------------------------------------------------------
// CContactsHandler::GetPhonesXMLForAdd()
// 
// Construct the XML data that has to be sent to the server
// ---------------------------------------------------------------------------
void CContactsHandler::GetPhonesXMLForAdd(TPtr buffer, CList *items, TPtrC type)
{
	if (items->Count() == 0)
		return;

	TBuf<30> tag;
	
	// Get subtags
	if (type.Find(_L("mblsh")) != KErrNotFound)
		tag.Copy(_L("mblh"));
	else if (type.Find(_L("mblsb")) != KErrNotFound)
		tag.Copy(_L("mblb"));
	else if (type.Find(_L("mbls")) != KErrNotFound)
		tag.Copy(_L("mbl"));
	else if (type.Find(_L("telsh")) != KErrNotFound)
		tag.Copy(_L("telh"));
	else if (type.Find(_L("telsb")) != KErrNotFound)
		tag.Copy(_L("telb"));
	else if (type.Find(_L("tels")) != KErrNotFound)
		tag.Copy(_L("tel"));
	else if (type.Find(_L("fxsh")) != KErrNotFound)
		tag.Copy(_L("fxh"));
	else if (type.Find(_L("fxsb")) != KErrNotFound)
		tag.Copy(_L("fxb"));
	else if (type.Find(_L("fxs")) != KErrNotFound)
		tag.Copy(_L("fx"));
	else if (type.Find(_L("emlsh")) != KErrNotFound)
		tag.Copy(_L("emlh"));
	else if (type.Find(_L("emlsb")) != KErrNotFound)
		tag.Copy(_L("emlb"));
	else if (type.Find(_L("emls")) != KErrNotFound)
		tag.Copy(_L("eml"));
	else if (type.Find(_L("ttls")) != KErrNotFound)
		tag.Copy(_L("ttl"));
	else
		tag.Copy(_L("id"));

	buffer.Append(_L("<"));
	buffer.Append(type);
	buffer.Append(_L(">"));

	CList *ptr = items;
	// Loop through the list and append item values to buffer
	for (int i = 0; i < items->Count(); i++) {
		TPtr* data = (TPtr* )ptr->GetContent();
		TPtr& idata = *data;
		TInt id = 0;
		do {
			int pos = idata.Find(_L(","));
			if (pos == KErrNotFound)
				break;

			TBuf<200> itemBuf;
			itemBuf.Copy(&idata[0], pos);
			ConvertXMLTags(itemBuf);

			buffer.Append(_L("<"));
			buffer.Append(tag);
			buffer.Append(_L(" id=\""));
			buffer.AppendNum(id);
			id++;
			buffer.Append(_L("\">"));
			buffer.Append(itemBuf);
			buffer.Append(_L("</"));
			buffer.Append(tag);
			buffer.Append(_L(">"));

			if (pos == idata.Length() - 1)
				break;

			idata.Copy(&idata[pos + 1], idata.Length() - pos - 1);
		} while (TRUE);

		ptr = ptr->next;
	}

	buffer.Append(_L("</"));
	buffer.Append(type);
	buffer.Append(_L(">"));
}

// ---------------------------------------------------------------------------
// CContactsHandler::GetPhonesXMLForUpdate()
// 
// Construct the XML data that has to be sent to the server
// ---------------------------------------------------------------------------
void CContactsHandler::GetPhonesXMLForUpdate(TPtr buffer, CList *items, TPtrC type, HBufC* timestamp)
{
	if (items->Count() == 0)
		return;

	TBuf<30> tag;
	
	// Get subtags
	if (type.Find(_L("mobiles_home")) != KErrNotFound)
		tag.Copy(_L("mobile_home"));
	else if (type.Find(_L("mobiles_business")) != KErrNotFound)
		tag.Copy(_L("mobile_business"));
	else if (type.Find(_L("mobiles")) != KErrNotFound)
		tag.Copy(_L("mobile"));
	else if (type.Find(_L("telephones_home")) != KErrNotFound)
		tag.Copy(_L("telephone_home"));
	else if (type.Find(_L("telephones_business")) != KErrNotFound)
		tag.Copy(_L("telephone_business"));
	else if (type.Find(_L("telephones")) != KErrNotFound)
		tag.Copy(_L("telephone"));
	else if (type.Find(_L("faxs_home")) != KErrNotFound)
		tag.Copy(_L("fax_home"));
	else if (type.Find(_L("faxs_business")) != KErrNotFound)
		tag.Copy(_L("fax_business"));
	else if (type.Find(_L("faxs")) != KErrNotFound)
		tag.Copy(_L("fax"));
	else if (type.Find(_L("emails_home")) != KErrNotFound)
		tag.Copy(_L("email_home"));
	else if (type.Find(_L("emails_business")) != KErrNotFound)
		tag.Copy(_L("email_business"));
	else if (type.Find(_L("emails")) != KErrNotFound)
		tag.Copy(_L("email"));
	else if (type.Find(_L("titles")) != KErrNotFound)
		tag.Copy(_L("title"));

	buffer.Append(_L("<"));
	buffer.Append(type);
	buffer.Append(_L(">"));

	CList *ptr = items;
	// Loop through the list and append item values to buffer
	for (int i = 0; i < items->Count(); i++) {
		TPtr* data = (TPtr* )ptr->GetContent();
		TPtr& idata = *data;
		int id = 0;
		do {
			int pos = idata.Find(_L(","));
			if (pos == KErrNotFound)
				break;

			TBuf<200> itemBuf;
			itemBuf.Copy(&idata[0], pos);
			ConvertXMLTags(itemBuf);

			buffer.Append(_L("<"));
			buffer.Append(tag);
			buffer.Append(_L(" id=\""));
			buffer.AppendNum(id);
			buffer.Append(_L("\" t=\""));
			buffer.Append(*timestamp);
			buffer.Append(_L("\">"));
			buffer.Append(itemBuf);
			buffer.Append(_L("</"));
			buffer.Append(tag);
			buffer.Append(_L(">"));

			id++;

			if (pos == idata.Length() - 1)
				break;

			idata.Copy(&idata[pos + 1], idata.Length() - pos - 1);
		} while (TRUE);

		ptr = ptr->next;
	}

	buffer.Append(_L("</"));
	buffer.Append(type);
	buffer.Append(_L(">"));
}

// ---------------------------------------------------------------------------
// CContactsHandler::GetXMLData()
// 
// Construct the XML data that has to be sent to the server
// ---------------------------------------------------------------------------
void CContactsHandler::GetXMLData(TPtr8 buffer, CRecordContactDB *record)
{
	CnvUtfConverter converter;
	TBuf8<1024> data;

	buffer.Copy(_L(""));

	if (record->iChangeType == 0) {
		iAppUi->iContactsCount++;
		
		iAppUi->iUploadContacts++;

#ifndef __UIQ__		
		//iAppUi->ActivateView(2);
		//iAppUi->ActivateView(4);
#endif
		
		buffer.Append(_L("<add><ct t=\""));
		buffer.Append(*(record->iTimeStamp));
		buffer.Append(_L("\" id =\""));
		buffer.AppendNum((TInt)record->iContactId);
		buffer.Append(_L("\"><fn>"));

		converter.ConvertFromUnicodeToUtf8(data, *(record->iFirstName));
		//data.Copy(*(record->iFirstName));
		ConvertXMLTags(data);
		buffer.Append(data);
		buffer.Append(_L("</fn><ln>"));

		converter.ConvertFromUnicodeToUtf8(data, *(record->iLastName));
		//data.Copy(*(record->iLastName));
		ConvertXMLTags(data);
		buffer.Append(data);
		buffer.Append(_L("</ln><cn>"));

		converter.ConvertFromUnicodeToUtf8(data, *(record->iCompanyName));
		//data.Copy(*(record->iCompanyName));
		ConvertXMLTags(data);
		buffer.Append(data);
		buffer.Append(_L("</cn>"));

		HBufC *phBuf = HBufC::NewL(1024);
		CleanupStack::PushL(phBuf);

		if (record->iJobTitles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iJobTitles, _L("ttls"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeFaxes, _L("fxsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkFaxes, _L("fxsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iFaxes, _L("fxs"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeMobiles, _L("mblsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkMobiles, _L("mblsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iMobiles, _L("mbls"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeTelephones, _L("telsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkTelephones, _L("telsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iTelephones, _L("tels"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeEmails, _L("emlsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkEmails, _L("emlsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iEmails, _L("emls"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}

		buffer.Append(_L("</ct></add>"));
		
		CleanupStack::PopAndDestroy(1);
	}
	else if (record->iChangeType == 1) {
		/*buffer.Append(_L("<update><ct t=\""));
		buffer.Append(*(record->iTimeStamp));
		buffer.Append(_L("\" id =\""));
		buffer.AppendNum((TInt)record->iContactId);
		buffer.Append(_L("\"><fn>"));

		buffer.Append(_L("</id><fn"));
		buffer.Append(_L("\">"));
		//converter.ConvertFromUnicodeToUtf8(data, *(record->iFirstName));
		data.Copy(*(record->iFirstName));
		ConvertXMLTags(data);
		buffer.Append(data);

		buffer.Append(_L("</firstname><lastname t=\""));
		buffer.Append(*(record->iTimeStamp));
		buffer.Append(_L("\">"));
		//converter.ConvertFromUnicodeToUtf8(data, *(record->iLastName));
		data.Copy(*(record->iLastName));
		ConvertXMLTags(data);
		buffer.Append(data);

		buffer.Append(_L("</lastname><company t=\""));
		buffer.Append(*(record->iTimeStamp));
		buffer.Append(_L("\">"));
		//converter.ConvertFromUnicodeToUtf8(data, *(record->iCompanyName));
		data.Copy(*(record->iCompanyName));
		ConvertXMLTags(data);
		buffer.Append(data);
		buffer.Append(_L("</company>"));

		HBufC *phBuf = HBufC::NewL(1024);
		CleanupStack::PushL(phBuf);

		if (record->iJobTitles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iJobTitles, _L("titles"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iHomeFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iHomeFaxes, _L("faxs_home"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iWorkFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iWorkFaxes, _L("faxs_business"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iFaxes, _L("faxs"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iHomeMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iHomeMobiles, _L("mobiles_home"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iWorkMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iWorkMobiles, _L("mobiles_business"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iMobiles, _L("mobiles"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iHomeTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iHomeTelephones, _L("telephones_home"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iWorkTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iWorkTelephones, _L("telephones_business"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iTelephones, _L("telephones"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iHomeEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iHomeEmails, _L("emails_home"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iWorkEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iWorkEmails, _L("emails_business"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}
		if (record->iEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForUpdate(phBuf->Des(), record->iEmails, _L("emails"), record->iTimeStamp);
			//converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			//buffer.Append(data);
			buffer.Append(*phBuf);
		}

		buffer.Append(_L("</contact></update>"));*/

		buffer.Append(_L("<update><ct t=\""));
		buffer.Append(*(record->iTimeStamp));
		buffer.Append(_L("\" id =\""));
		buffer.AppendNum((TInt)record->iContactId);
		buffer.Append(_L("\"><fn>"));

		converter.ConvertFromUnicodeToUtf8(data, *(record->iFirstName));
		//data.Copy(*(record->iFirstName));
		ConvertXMLTags(data);
		buffer.Append(data);
		buffer.Append(_L("</fn><ln>"));

		converter.ConvertFromUnicodeToUtf8(data, *(record->iLastName));
		//data.Copy(*(record->iLastName));
		ConvertXMLTags(data);
		buffer.Append(data);
		buffer.Append(_L("</ln><cn>"));

		converter.ConvertFromUnicodeToUtf8(data, *(record->iCompanyName));
		//data.Copy(*(record->iCompanyName));
		ConvertXMLTags(data);
		buffer.Append(data);
		buffer.Append(_L("</cn>"));

		HBufC *phBuf = HBufC::NewL(1024);
		CleanupStack::PushL(phBuf);

		if (record->iJobTitles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iJobTitles, _L("ttls"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeFaxes, _L("fxsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkFaxes, _L("fxsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iFaxes->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iFaxes, _L("fxs"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeMobiles, _L("mblsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkMobiles, _L("mblsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iMobiles, _L("mbls"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeTelephones, _L("telsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkTelephones, _L("telsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iTelephones, _L("tels"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iHomeEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeEmails, _L("emlsh"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iWorkEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkEmails, _L("emlsb"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		if (record->iEmails->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iEmails, _L("emls"));
			converter.ConvertFromUnicodeToUtf8(data, *phBuf);
			buffer.Append(data);
			//buffer.Append(*phBuf);
		}
		buffer.Append(_L("</ct></update>"));

		CleanupStack::PopAndDestroy(1);
	}
	else if (record->iChangeType == 2) {
		iAppUi->iContactsCount--;

#ifndef __UIQ__
		//iAppUi->ActivateView(2);
		//iAppUi->ActivateView(4);
#endif		
		buffer.Append(_L("<delete><ct t=\""));
		buffer.Append(*(record->iTimeStamp));
		buffer.Append(_L("\" id=\""));
		buffer.AppendNum((TInt)record->iContactId);
		buffer.Append(_L("\">"));
		buffer.Append(_L("</ct></delete>"));

	}
	else if (record->iChangeType == 3) {
		buffer.Append(_L("<mapsa><ct id=\""));
		buffer.AppendNum((TInt)record->iContactId);
		buffer.Append(_L("\"><gcid>"));
		buffer.Append(*(record->iGlobalId));
		buffer.Append(_L("</gcid></ct></mapsa>"));
	}
	else if (record->iChangeType == 4) {
		buffer.Append(_L("<mapsd><ct id=\""));
		buffer.AppendNum((TInt)record->iContactId);
		buffer.Append(_L("\">"));
		buffer.Append(_L("</ct></mapsd>"));
	}
#if 0
	else if (record->iChangeType == 3) {
		buffer.Append(_L("<add_update><contact t="));
		buffer.Append(*(record->iTimeStamp));
		buffer.Append(_L("><id>"));
		buffer.AppendNum((TInt)record->iContactId);
		buffer.Append(_L("</id><firstname>"));
		buffer.Append(*(record->iFirstName));
		buffer.Append(_L("</firstname><lastname>"));
		buffer.Append(*(record->iLastName));
		buffer.Append(_L("</lastname><company>"));
		buffer.Append(*(record->iCompanyName));
		buffer.Append(_L("</company>"));

		HBufC *phBuf = HBufC::NewL(10 * 1024);

		if (record->iHomeMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeMobiles, _L("mobiles_home"));
			buffer.Append(*phBuf);
		}
		if (record->iWorkMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkMobiles, _L("mobiles_business"));
			buffer.Append(*phBuf);
		}
		if (record->iMobiles->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iMobiles, _L("mobiles"));
			buffer.Append(*phBuf);
		}
		if (record->iHomeTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iHomeTelephones, _L("telephones_home"));
			buffer.Append(*phBuf);
		}
		if (record->iWorkTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iWorkTelephones, _L("telephones_business"));
			buffer.Append(*phBuf);
		}
		if (record->iTelephones->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iTelephones, _L("telephones"));
			buffer.Append(*phBuf);
		}

		buffer.Append(_L("</contact></add_update>"));

		phBuf->Des().Copy(_L(""));
		phBuf->Des().Append(buffer);
		
		TBuf<20> len;
		len.AppendNum(phBuf->Des().Length());
		
		buffer.Copy(_L(""));
		//buffer.Append(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
		
		buffer.Append(_L("GET "));
		buffer.Append(iAppUi->iContactsUrl);
		//buffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
		buffer.Append(_L(" HTTP/1.0\r\nHOST:"));
		buffer.Append(iAppUi->iServerAddr);
		buffer.Append(_L("\r\nPORT:"));
		buffer.AppendNum(iAppUi->iPort);
		buffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


		buffer.Append(len);
		buffer.Append(_L("\r\n\r\n"));
		buffer.Append(*phBuf);

		delete phBuf;
	}
	else {
		HBufC *phBuf = HBufC::NewL(10 * 1024);
		
		if (record->iIdList->Count() > 0) {
			phBuf->Des().Copy(_L(""));
			GetPhonesXMLForAdd(phBuf->Des(), record->iIdList, _L("id_list"));
			buffer.Append(*phBuf);
		}

		phBuf->Des().Copy(_L(""));
		phBuf->Des().Append(buffer);
		
		TBuf<20> len;
		len.AppendNum(phBuf->Des().Length());
		
		buffer.Copy(_L(""));
		//buffer.Append(_L("GET /ripple/rippleservlet HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
		buffer.Append(_L("GET "));
		buffer.Append(iAppUi->iContactsUrl);
		//buffer.Append(_L(" HTTP/1.0\r\nHOST:\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));
		buffer.Append(_L(" HTTP/1.0\r\nHOST:"));
		buffer.Append(iAppUi->iServerAddr);
		buffer.Append(_L("\r\nPORT:"));
		buffer.AppendNum(iAppUi->iPort);
		buffer.Append(_L("\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:"));


		buffer.Append(len);
		buffer.Append(_L("\r\n\r\n"));
		buffer.Append(*phBuf);

		delete phBuf;
	}
#endif // if 0
}

void CContactsHandler::SetSyncParams(TInt currRecord, TInt numRecords)
{
	RFs fs;
	RFile file;

	fs.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRegPath);

	TInt err = file.Open(fs, path, EFileWrite);
	if (err == KErrNone) {

		TBuf8<10> cr;
		TBuf8<10> nr;
		TBuf8<20> state;

		cr.Num(currRecord);
		nr.Num(numRecords);
		
		file.SetSize(0);
		state.Copy(cr);
		state.Append(_L(","));
		state.Append(nr);
		file.Write(state);
	}

	file.Close();
	fs.Close();
}

void CContactsHandler::GetSyncParams(TInt &currRecord, TInt &numRecords)
{
	RFs fs;
	RFile file;

	fs.Connect();

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRegPath);

	TInt err = file.Open(fs, path, EFileRead);
	if (err == KErrNone) {

		TBuf8<20> state;
		//User::LeaveIfError(file.Read(state));
		file.Read(state);
		
		if(state.Length()!=0)
		{
			TBuf8<10> cr;
			TBuf8<10> nr;

			TInt pos = state.Find(_L8(","));

			cr.Copy(&state[0], pos);
			nr.Copy(&state[pos + 1], state.Length() - pos - 1);


			TLex8 lexc(cr);
			lexc.Val(currRecord);
			
			TLex8 lexn(nr);
			lexn.Val(numRecords);
		}
	}

	file.Close();
	fs.Close();
}

TInt CContactsHandler::GetNumPackets(TInt numRecords)
{
	if (numRecords % MAX_CONTACTS == 0)
		return (numRecords / MAX_CONTACTS);
	else
		return (numRecords / MAX_CONTACTS + 1);
}

TInt CContactsHandler::GetNumRecords(TInt currRecord, TInt numRecords)
{
	if (numRecords == 0)
		return 0;

	TInt records = currRecord * MAX_CONTACTS - numRecords;
	if (records <= 0)
		return (MAX_CONTACTS);
	else
		return (MAX_CONTACTS - records);
}


void CContactsHandler::CreateHeaderXML(TDesC &msgType)
{
	//TBuf<20> msgType;
	HBufC* currTime;
	TBuf<128> syncTime;
	TBuf<128> candTime;
	TInt numPackets;

	TTime ctime;

	ctime.HomeTime();

	currTime = iAppUi->GetTimeStamp(ctime);
	iChangesDB->GetSyncTimes(syncTime, candTime);

	GetSyncParams(iCurrSync, iNumSync);
	numPackets = GetNumPackets(iNumSync);
	if (numPackets == 0)
		numPackets = 1;
	
	//iChangesDB->GetMessageType(msgType);
	
	iBuffer.Append(_L("<h>"));
	iBuffer.Append(_L("<msgt>"));
	iBuffer.Append(msgType);
	iBuffer.Append(_L("</msgt>"));

	iBuffer.Append(_L("<msg></msg>"));

	iBuffer.Append(_L("<uid>"));
	iBuffer.Append(iAppUi->iUserName);
	iBuffer.Append(_L("</uid>"));

	iBuffer.Append(_L("<hp>"));
	iBuffer.Append(iAppUi->iMobileNo);
	iBuffer.Append(_L("</hp>"));

	iBuffer.Append(_L("<did>"));
	iBuffer.Append(iAppUi->PhoneImei());
	iBuffer.Append(_L("</did>"));

	iBuffer.Append(_L("<currec>"));
	iBuffer.AppendNum(iCurrSync);
	iBuffer.Append(_L("</currec>"));

	iBuffer.Append(_L("<totrec>"));
	iBuffer.AppendNum(numPackets);
	iBuffer.Append(_L("</totrec>"));

	iBuffer.Append(_L("<loc>en_us</loc>"));
	
	iBuffer.Append(_L("<ctime>"));
	iBuffer.Append(*currTime);
	iBuffer.Append(_L("</ctime>"));

	iBuffer.Append(_L("<lstime>"));
	if(syncTime.Compare(_L(""))==0)
		syncTime.Copy(_L("0"));
	iBuffer.Append(syncTime);
	iBuffer.Append(_L("</lstime>"));

	iBuffer.Append(_L("<synctype><mod>cs</mod><gran>contactlevel</gran>\
<adde>true</adde><upde>true</upde><dele>true</dele></synctype>"));

	iBuffer.Append(_L("</h>"));
}

void CContactsHandler::Poll(TInt seconds, TInt aType)
{
	//iChangesDB->SetMessageType(_L("poll"));
	TBuf<10> pl;
	pl.Copy(_L("poll"));

	iBuffer.Copy(_L("<root>"));
	CreateHeaderXML(pl);
	iBuffer.Append(_L("<b></b></root>"));
	TBuf8<10> secs;

	if (aType == 1)
		secs.Copy(_L("p"));
	else
		secs.Copy(_L("statreg"));

	secs.AppendNum(seconds);

	TInt pos = iBuffer.Find(_L8("</msg>"));
	iBuffer.Insert(pos, secs);

	iContactState = KDBConnected;

	Notify();
	if (iSocketsEngine != NULL)
	{
		iAppUi->iContactsCount = 0;
		delete iSocketsEngine;
		iSocketsEngine = NULL;
	}
}

void CContactsHandler::SyncToServer()
{
	//if (iNumSync == 0)
	//	Poll(1, 1);

	/*iPendingSync = 1;
	if (iContactState == KStateNone)
	{
		iContactState = KDBChange;
		Notify();
	}
    return;*/

	if (iContactState != KStateNone)
		return;

	if (IsNetworkInProgress())
	{
		//iContactState = KStateNone;
		return;
		//iRetryCount = MAX_RETRIES;
		//Notify();
	}

	iSMS = ETrue;

	//iAppUi->iContactsCount = 0;
	//iAppUi->SaveTempContactsUploadSettingsToDB(iAppUi->iContactsCount);

	GetSyncParams(iCurrSync, iNumSync);
	/*TBuf8<50> buf;
	buf.Copy(_L("sms processing "));
	buf.AppendNum(iNumSync);
	buf.Append(_L(" "));
	buf.Append(iAppUi->ServerAddr());
	buf.Append(_L(" "));
	buf.AppendNum(iAppUi->ServerPort());
	buf.Append(_L("\r\n"));
	iAppUi->Logger(buf);*/
	if (iNumSync == 0 && iChangesDB->CountRecords() == 0) {
		iPendingSync = 1;
		SendComplete();
	}
	else {
		ProcessEventQueue();
		MoreToUpload();
	}
}

void CContactsHandler::ConnectToServer()
{
	if(iSocketsEngine == NULL)
	{
		iSocketsEngine = CContactsSocketsEngineWriter::NewL(*iAppUi, iAppUi->ServerAddr(),iAppUi->ServerPort());
		iSocketsEngine->servlet_data = iBuffer.AllocL();
		iSocketsEngine->Start();
	}
}

void CContactsHandler::ResetState()
{
	iContactState = KStateNone;
}
