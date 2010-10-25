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
 Module Name				: ChangesContactsAO.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Active Object that checks pending changes to the Contacts DB
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include "ChangesContactsAO.h"

#include <cdbcols.h>
#include "RippleVaultAppUi.h"
#include "List.h"
#include "RecordContactDB.h"
#include "ContactsHandler.h"
#include "ContactsDBEngine.h"

 
CChangesContactsAO::CChangesContactsAO(CRippleVaultAppUi& aAppUi)
: CActive(EPriorityIdle),
  iAppUi(aAppUi)
{
	CActiveScheduler::Add(this);
}


CChangesContactsAO::~CChangesContactsAO()
{
	Cancel();
	delete iDatabase;
//	iIdList->RemoveAll();
	//delete iIdList;
	//iIdList = NULL;
}

void CChangesContactsAO::StartL(TTime aCDBTime, TTime aRippleTime)
{
	// Open the database and get all the contacts into a list.
	iDatabase = CContactDatabase::OpenL();
	iContacts = iDatabase->SortedItemsL();
	iIdList = new CList();

	iRippleTime = aRippleTime;
	iCDBTime = aCDBTime;

	iContactIndex = 0;
	iDeletedIndex = 0;

	// If there are actually any contacts in the DB, start by swapping the first one.
	if (iContacts->Count() >= 0)
	{
		// Reschedule ourselves.
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		if (!IsActive())
			SetActive();

		//CompareContactL(iContactIndex);

		ChangeStatus(EAddUpdate);
	}
	else 
	{
		iIdList->RemoveAll();
		delete this;
	}
}

TInt CChangesContactsAO::RunError(TInt aError)
{
/*	TBuf8<60> buf;
	buf.Copy(_L8("CChangesContactsAO Leave Error: "));
	buf.AppendNum(aError);
	buf.Append(_L("\r\n"));
	iAppUi.WriteLogFile(buf);
*/
	// Just return KErrNone to stop the active scheduler panicking.
	return KErrNone;
}

void CChangesContactsAO::RunL()
{
	// If there's been an error, bail out straight away.
	// User::LeaveIfError(iStatus.Int());
	if (iStatus != KErrNone)
	{
		//return;
	}

	switch(iEngineStatus)
	{
	case EAddUpdate:
		//iContactIndex++;

		if (iContactIndex < iContacts->Count())
		{
			// Reschedule ourselves.
			TRequestStatus* status = &iStatus;
			User::RequestComplete(status, KErrNone);
			if (!IsActive())
				SetActive();
			
			// Swap the next contact.
			CompareContactL(iContactIndex);

			iContactIndex++;
		}
		else if(iContactIndex>=iContacts->Count())
		{
			//iAppUi.iContactsHandler->iContactState = iAppUi.iContactsHandler->EDBHandlerState::KDBStore;

			TDateTime janNineteenSeventy(1970,EJanuary,0,00,00,00,000000);
			TTime timejan(janNineteenSeventy);
			TBuf<256> timestamp;

			TTimeIntervalMicroSeconds interval = iCDBTime.MicroSecondsFrom(timejan);
			TInt64 k;
			k=interval.Int64()/1000;
			timestamp.Num(k);

			iAppUi.AddTimeStampToFileL(&timestamp);
			
			iDeletedIds = iAppUi.iContactsHandler->iChangesDB->GetDeletedIds(iIdList);
			iDeletedPtr = iDeletedIds;

			ChangeStatus(EDelete);

			TRequestStatus* status = &iStatus;
			User::RequestComplete(status, KErrNone);
			if (!IsActive())
				SetActive();

		}
		break;
	case EDelete:
			//for (TInt i = 0; i < iDeletedIds->Count(); i++)
		if (iDeletedIndex < iDeletedIds->Count())
		{
			TRequestStatus* status = &iStatus;
			User::RequestComplete(status, KErrNone);
			if (!IsActive())
				SetActive();

			TInt cid;
			TBool found;
			CList *ptr;
			TInt id;

			cid = (TInt)iDeletedPtr->GetContent();
			ptr = iIdList;
			found = EFalse;
			for (TInt i = 0; i < iIdList->Count(); i++)
			{
				id = (TInt)ptr->GetContent();
				if (id == cid) {
					found = ETrue;
					break;
				}
				ptr = ptr->next;
			}

			if (!found)
			{
				CContactItem* contact=NULL;
				TInt error = KErrNone;

				TRAP(error, contact = iDatabase->ReadContactL(cid));
				if (error == KErrNone)
				{
					iDatabase->CloseContactL(cid);
					delete contact;
				}
				else if (error == KErrNotFound)
				{
					CRecordContactDB *record = iAppUi.iContactsHandler->CreateDeletedRecordContactDB(cid);
					iAppUi.iContactsHandler->iChangesDB->AddRecordFromContactsDB(record);
					iAppUi.iContactsHandler->iChangesDB->DeleteId(cid);
					delete record;
				}
			}

			iDeletedPtr = iDeletedPtr->next;
			iDeletedIndex++;
		}
		else
		{
			iDeletedIds->RemoveAll();
			iIdList->RemoveAll();

			//iAppUi.iContactsHandler->SyncToServer();

			if(iAppUi.iContactsHandler->iContactsUpload == 0)
			{
				iAppUi.LogText.Copy(_L("Change Contacts 1\n"));
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
		break;
	}	
}

void CChangesContactsAO::DoCancel()
{
	// Synchronously compress the database.
	iDatabase->CompactL();

	delete iDatabase;
	iDatabase = NULL;
}

/*TInt CChangesContactsAO::RunError(TInt aError)
{
	TBuf8<60> buf;
	buf.Copy(_L8("CChangesContactsAO Leave Error: "));
	buf.AppendNum(aError);
	buf.Append(_L("\r\n"));
	iAppUi.WriteLogFile(buf);

	// Just return KErrNone to stop the active scheduler panicking.
	return KErrNone;
}*/

void CChangesContactsAO::ChangeStatus(TChangesAOState aNewStatus)
{
    iEngineStatus = aNewStatus;
}

void CChangesContactsAO::CompareContactL(TInt aIndex)
{
	TInt cid=(*iContacts)[aIndex];

	iIdList->Add((void *)cid);

	CContactItem* contact=NULL;
		
	TInt error = KErrNone;
	TRAP(error, contact = iDatabase->ReadContactL(cid));
	if (error == KErrNone)
	{
		//TInt64 offsettime;
		TTime univtime, hometime;
		univtime.UniversalTime();
		hometime.HomeTime();

		if (contact->LastModified().Int64() + hometime.Int64() > (iRippleTime.Int64() + univtime.Int64()) && contact->Type() != KUidContactGroup) {

#ifdef __LOGME__
		iAppUi.LogText.Copy(_L("PENDING CHANGE"));
		iAppUi.WriteLogFile(iAppUi.LogText);
#endif

			if (iAppUi.iContactsHandler->iChangesDB->ExistsId(cid)) {
				CRecordContactDB *record = iAppUi.iContactsHandler->CreateUpdatedRecordContactDB(*contact);
				iAppUi.iContactsHandler->iChangesDB->AddRecordFromContactsDB(record);

				/*if (contact->LastModified() > iLastTime) {
					iLastTime = contact->LastModified();
					TBuf<256> ts;
					ts.Copy(*(record->iTimeStamp));
					iAppUi.AddTimeStampToFileL(&ts);
				}*/

				delete record;
			}
			else {
				CRecordContactDB *record = iAppUi.iContactsHandler->CreateAddedRecordContactDB(*contact);
				iAppUi.iContactsHandler->iChangesDB->AddRecordFromContactsDB(record);
				iAppUi.iContactsHandler->iChangesDB->AddId(cid);

				/*if (contact->LastModified() > iLastTime) {
					iLastTime = contact->LastModified();
					TBuf<256> ts;
					ts.Copy(*(record->iTimeStamp));
					iAppUi.AddTimeStampToFileL(&ts);
				}*/

				delete record;
			}
				
		}

		iDatabase->CloseContactL(cid);
		delete contact;
			//CEikonEnv::Static ()->AlertWin(_L("end add update ..."));
	}
}

// End of file
