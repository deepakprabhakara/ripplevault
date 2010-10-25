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
 Module Name				: CopyContactsAO.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Active Object for copying all contacts and storing to DB the first time the app is run
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include "CopyContactsAO.h"

#include <etel.h>

//#include <AgentClient.h>
//#include <cdbcols.h>
//#include <commdb.h>
#include "SocketsEngineWriter.h"
#include "RippleVaultAppUi.h"
#include "List.h"
#include "RecordContactDB.h"
#include "ContactsHandler.h"
#include "ContactsDBEngine.h"

#include <bautils.h>
 
CCopyContactsAO::CCopyContactsAO(CRippleVaultAppUi& aAppUi)
: CActive(EPriorityIdle),
  iAppUi(aAppUi)
{
	CActiveScheduler::Add(this);
}


CCopyContactsAO::~CCopyContactsAO()
{
	Cancel();
	delete iDatabase;
}

void CCopyContactsAO::StartL()
{
	// Open the database and get all the contacts into a list.
	iDatabase = CContactDatabase::OpenL();
	iContacts = iDatabase->SortedItemsL();

	iContactIndex = 0;
	
	TTime ctime;
	HBufC* currTime;
	
	ctime.HomeTime();
	currTime = iAppUi.GetTimeStamp(ctime);
	TBuf<30> tempstr;
	tempstr.Copy(_L("0"));
	
	iAppUi.iContactsHandler->iChangesDB->SetSyncTimes(tempstr, *currTime);

	// If there are actually any contacts in the DB, start by swapping the first one.
	if (iContacts->Count() > 0)
	{
		// Reschedule ourselves.
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		if (!IsActive())
			SetActive();

		CopyContactL(iContactIndex);
	}
	else 
	{
/*		TBuf8<60> buf;
		buf.Copy(_L8("Copy Contacts: SyncToServer"));
		iAppUi.WriteLogFile(buf);
*/		
		//iAppUi.iContactsHandler->iPendingSync = 1;
		//iAppUi.iContactsHandler->MoreToUpload();
		//iAppUi.iContactsHandler->SyncToServer();

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("Copy Contacts 1\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		//added by Deepak
			TBuf<128> path;
			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KInitUploadFile);

			RFs fs1;

			fs1.Connect();
			
			//delete file
			//fs1.Delete(KRegStep1);
			BaflUtils::DeleteFile(fs1, path);

			fs1.Close();

		if(iAppUi.iContactsHandler->iContactsUpload == 0 || iAppUi.iContactsHandler->iContactsUpload == 1)
		{
			iAppUi.LogText.Copy(_L("Copy Contacts 1\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

			iAppUi.iSynchronize = 0;
			iAppUi.UpdateContacts(0);
			/*
			iAppUi.iContactsTimerState = 1;
			iAppUi.iConnectionStatus = 2;
			iAppUi.CheckSubscriptionStatus(iAppUi.iConnectionStatus);///
			*/			
		}

		delete this;
	}
}

void CCopyContactsAO::RunL()
{
	// If there's been an error, bail out straight away.
	// User::LeaveIfError(iStatus.Int());
	if (iStatus != KErrNone)
	{
		//return;
	}

	iContactIndex++;

	if (iContactIndex < iContacts->Count())
	{
		// Reschedule ourselves.
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		if (!IsActive())
			SetActive();
		
		// Swap the next contact.
		CopyContactL(iContactIndex);
	}
	else if(iContactIndex>=iContacts->Count())
	{
		//iAppUi.iContactsHandler->iContactState = iAppUi.iContactsHandler->EDBHandlerState::KDBStore;

		iAppUi.iContactsHandler->iNumSync = iAppUi.iContactsHandler->iChangesDB->CountRecords();
		iAppUi.iContactsHandler->iCurrSync = 1;

		iAppUi.iContactsHandler->SetSyncParams(1, iAppUi.iContactsHandler->iNumSync);

		UpdateTimeStamp();
		
/*		TBuf8<60> buf;
		buf.Copy(_L8("Copy Contacts:"));
		buf.AppendNum(iAppUi.iContactsHandler->iNumSync);
		buf.Append(_L(" "));
		buf.AppendNum(iContacts->Count());
		buf.Append(_L("\r\n"));
		iAppUi.WriteLogFile(buf);
*/
		//iAppUi.iContactsHandler->iContactState = KDBStore;
		//iAppUi.iContactsHandler->Notify();
		//iAppUi.iContactsHandler->SyncToServer();

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("Copy Contacts 2\n"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

		//added by Deepak
			TBuf<128> path;
			path = CRippleVaultAppUi::ApplicationDriveAndPath();
			path.Append(KInitUploadFile);

			RFs fs1;

			fs1.Connect();
			
			//delete file
			//fs1.Delete(KRegStep1);
			BaflUtils::DeleteFile(fs1, path);

			fs1.Close();

		if(iAppUi.iContactsHandler->iContactsUpload == 0 || iAppUi.iContactsHandler->iContactsUpload == 1)
		{
			iAppUi.LogText.Copy(_L("Copy Contacts 2\n"));
			iAppUi.WriteLogFile(iAppUi.LogText);

			iAppUi.iSynchronize = 0;
			iAppUi.UpdateContacts(0);
			/*
			iAppUi.iContactsTimerState = 1;
			iAppUi.iConnectionStatus = 2;
			iAppUi.CheckSubscriptionStatus(iAppUi.iConnectionStatus);///
			*/			
		}

		delete this;
	}	
}

void CCopyContactsAO::DoCancel()
{
	// Synchronously compress the database.
	iDatabase->CompactL();

	delete iDatabase;
	iDatabase = NULL;
}

TInt CCopyContactsAO::RunError(TInt aError)
{
/*	TBuf8<60> buf;
	buf.Copy(_L8("CCopyContactsAO Leave Error: "));
	buf.AppendNum(aError);
	buf.Append(_L("\r\n"));
	iAppUi.WriteLogFile(buf);
*/
	// Just return KErrNone to stop the active scheduler panicking.
	return KErrNone;
}

void CCopyContactsAO::CopyContactL(TInt aIndex)
{
	TInt cid=(*iContacts)[aIndex];

	CContactItem* contact=NULL;
		
	TInt error = KErrNone;
	TRAP(error, contact = iDatabase->ReadContactL(cid));
	if (error == KErrNone)
	{
		if (contact->Type() != KUidContactGroup 
			&& !iAppUi.iContactsHandler->iChangesDB->ExistsId(cid)) {
			CRecordContactDB *record = iAppUi.iContactsHandler->CreateAddedRecordContactDB(*contact);
			iAppUi.iContactsHandler->iChangesDB->AddRecordFromContactsDB(record);
			iAppUi.iContactsHandler->iChangesDB->AddId(cid);
			delete record;
		}

		iDatabase->CloseContactL(cid);

		delete contact;
	}
}

void CCopyContactsAO::UpdateTimeStamp()
{
	RFs fs;
	fs.Connect();

	TBuf<256> cdbFileName; // Contact db file name

	if (iDatabase->FindContactFile(cdbFileName)) {

		TTime cdbTime;
		fs.Modified(cdbFileName, cdbTime);

		TDateTime janNineteenSeventy
				(1970,EJanuary,0,00,00,00,000000);
		TTime timejan(janNineteenSeventy);

		TTimeIntervalMicroSeconds interval =
				cdbTime.MicroSecondsFrom(timejan);

		TInt64 cdbVal;
		cdbVal = interval.Int64()/1000;

		TBuf<256> timestamp;
		timestamp.Num(cdbVal);

		iAppUi.AddTimeStampToFileL(&timestamp);
	}

	fs.Close();
}

// End of file
