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
 Module Name				: ReadSmsAO.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Active Object for updating Sms on the phone
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/


#include "ReadSmsAO.h"
#include "Constants.h"

#ifdef __UIQ__
#ifdef __S80__
	#include "MessageTextListBox.h"
#else
	#include "SmsScreenView_Uiq.h"
#endif
#else
	#include "ManualSmsListContainer.h"
#endif
 
#ifdef __UIQ__
#ifdef __S80__
CReadSmsAO::CReadSmsAO(CMessageTextListBox& aAppView)
: CActive(EPriorityHigh),iAppView(aAppView)
#else
CReadSmsAO::CReadSmsAO(CSmsScreenView& aAppView)
: CActive(EPriorityHigh),iAppView(aAppView)
#endif
#else
CReadSmsAO::CReadSmsAO(CManualSmsListContainer& aAppView)
: CActive(EPriorityHigh),iAppView(aAppView)
#endif
{
	CActiveScheduler::Add(this);
}

CReadSmsAO::~CReadSmsAO()
{
	ReleaseMtm();

	Cancel();
}

void CReadSmsAO::StartL()
{
	PrepareMtm(SMS_INBOX);

	iInboxFlag = 0;
	iInboxIndex = 0;
	iSentItemsIndex = 0;

	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
}

void CReadSmsAO::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
{
}

void CReadSmsAO::RunL()
{
	if (iInboxCount > 0 && iInboxFlag == 0)
	{	
		if(ReadSms())

#ifdef __UIQ__
		//if(iAppView.iInboxCount % 10 == 0 || iAppView.iInboxCount < 10)
		if(iAppView.iAppUi.iImageFlag == 5)
		{
			iAppView.GetTopIndex();
			iAppView.SetFileList(1);
		}
#else		
		iAppView.SetFileList(3);
#endif

		iInboxCount--;

		if (iInboxCount == 0)
		{
			ReleaseMtm();
			PrepareMtm(SMS_SENTITEMS);
			//GetSentItemSmsCount();
			iSentItemFlag = 0;
		}

		Notify();
	}
	else if (iSentItemCount > 0 && iSentItemFlag == 0)
	{	
		if(ReadSms())
				
#ifdef __UIQ__
		//if(iAppView.iSentItemCount % 10 == 0 || iAppView.iSentItemCount < 10)
		if(iAppView.iAppUi.iImageFlag == 6)
		{
			iAppView.GetTopIndex();
			iAppView.SetFileList(1);
		}
#else
		iAppView.SetFileList(3);
#endif

		iSentItemCount--;

		if (iSentItemCount == 0)
		{
			ReleaseMtm();
			PrepareMtm(SMS_INBOX);
			iInboxFlag = 0;
		}

		Notify();
	}
	else
	{	
		if (iInboxCount == 0 && iSentItemCount == 0)
			iAppView.CloseReadSms();
	}	
}

void CReadSmsAO::DoCancel()
{
}

TInt CReadSmsAO::RunError(TInt aError)
{
	return KErrNone;
}

void CReadSmsAO::Notify()
{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);

	if (!IsActive())
		SetActive();
}

void CReadSmsAO::PrepareMtm(TInt aSmsType)
{
	iSmsType = aSmsType;

	if( aSmsType == 0)
		aSmsType = KMsvGlobalInBoxIndexEntryId;
	else if( aSmsType == 1)
		aSmsType = KMsvSentEntryId;

	iSession = CMsvSession::OpenSyncL(*this );

	iMtmReg = CClientMtmRegistry::NewL(*iSession);

	TMsvSelectionOrdering sort(KMsvGroupByType, EMsvSortByDateReverse, ETrue);
	//sort.SetShowInvisibleEntries(ETrue);

	iParentEntry  = CMsvEntry::NewL(*iSession, aSmsType, sort);	

	// get list of SMS entries in inbox
	iEntries  = iParentEntry->ChildrenL();
}

void CReadSmsAO::ReleaseMtm()
{
	if(iEntries != NULL)
	{
		delete iEntries;
		iEntries = NULL;
	}

	if(iParentEntry != NULL)
	{
		delete iParentEntry;
		iParentEntry = NULL;
	}

	if(iMtmReg != NULL)
	{
		delete iMtmReg;
		iMtmReg = NULL;
	}

	if(iSession != NULL)
	{
		delete iSession;
		iSession = NULL;
	}
}

TInt CReadSmsAO::ReadSms()
{
	TInt flag = 0;

	TMsvId id=0;

	if(iSmsType == 0)
		id = iEntries->At(iInboxIndex);
	else if(iSmsType == 1)
		id = iEntries->At(iSentItemsIndex);	
	
	CMsvEntry* entry = iSession->GetEntryL(id);
	TMsvEntry msvEntry = entry->Entry();

	CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iMtmReg->NewMtmL(msvEntry.iMtm));
		
	smsMtm->SwitchCurrentEntryL(id);
	
	TUid type = smsMtm->Type();
		
	if(type == KUidMsgTypeSMS)
	{
		TBuf<2000> smsnumber;
		TBuf<160> datetime;
		TBuf<2> ampm;
		TTime time;

		smsMtm->LoadMessageL();

		if(iSmsType == 0)
			iAppView.InboxSmsDetailsarray->InsertL(iAppView.iInboxCount, (*iParentEntry)[iInboxIndex].iDetails);
		else if(iSmsType == 1)
			iAppView.SentSmsDetailsarray->InsertL(iAppView.iSentItemCount, (*iParentEntry)[iSentItemsIndex].iDetails);

		smsMtm->Body().Extract(smsnumber,0);
		
		if(iSmsType == 0)
			iAppView.InboxSmsMessagearray->InsertL(iAppView.iInboxCount, smsnumber);
		else if(iSmsType == 1)
			iAppView.SentSmsMessagearray->InsertL(iAppView.iSentItemCount, smsnumber);	
		
		if(iSmsType == 0)
			time = (*iParentEntry)[iInboxIndex].iDate;
		else if(iSmsType == 1)
			time = (*iParentEntry)[iSentItemsIndex].iDate;

		TDateTime aDateTime = time.DateTime();		
		_LIT(KFormatTxt,"%d/%d/%d %d:%d:%d ");		
				
		TInt hour = aDateTime.Hour();
		if(hour>12) 
		{
			hour=hour-12;
			ampm.Copy(_L("pm"));
		}
		else
		{
			ampm.Copy(_L("am"));
		}

		datetime.Copy(_L(""));

		datetime.Format(KFormatTxt, aDateTime.Day()+1, TInt(aDateTime.Month()+1), 
			aDateTime.Year(), hour, aDateTime.Minute(), aDateTime.Second());

		datetime.Append(ampm);
		
		if(iSmsType == 0)
			iAppView.InboxSmsDatearray->InsertL(iAppView.iInboxCount,datetime );
		else if(iSmsType == 1)
			iAppView.SentSmsDatearray->InsertL(iAppView.iSentItemCount,datetime );
		
		if(iSmsType == 0)
			iAppView.iInboxCount++;
		else if(iSmsType == 1)
			iAppView.iSentItemCount++;	

		flag = 1;
	}

	if(iSmsType == 0)
		iInboxIndex++;
	else if(iSmsType == 1)
		iSentItemsIndex++;

	delete smsMtm;
	delete entry;

	return flag;
}
