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
 Module Name				: SmsFile.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 10-05-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  To send outgoing SMS
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include "SmsFile.h"
#include "RippleVaultAppUi.h"

#include <mtclreg.h>                        // for CClientMtmRegistry 
#include <msvids.h>                         // for Message type IDs
#include <smscmds.h>
#include <smuthdr.h>
#include <smutset.h>
#include <msvuids.h>
#include <txtrich.h>
#include <smsclnt.h>
#include <mtmdef.h>
#include <gsmupdu.h>

class dummyhandler: public MMsvSessionObserver {
public:
	virtual void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) { }
};

CSmsFile::CSmsFile(CRippleVaultAppUi& aAppUi):iAppUi(aAppUi),
CActive(EPriorityStandard)
	{
	}

CSmsFile::~CSmsFile()
{
	Cancel();
	
	delete sendOptions;
	delete op;
	delete iMtm;
	delete iReceiveMtm;
	delete iMtmReg;
	delete iReceiveMtmReg;
	delete iReceiveSession;
	delete iSession;
	delete dummy;
}

CSmsFile* CSmsFile::NewL(CRippleVaultAppUi& aAppUi)
	{
	CSmsFile* self = new(ELeave) CSmsFile(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CSmsFile::ConstructL()
{
	dummy=new (ELeave) dummyhandler;

	// Create CMsvSession
	iReceiveSession = CMsvSession::OpenSyncL(*this); // new session is opened synchronously
	iReceiveMtmReg = CClientMtmRegistry::NewL(*iReceiveSession);
	iReceiveMtm = iReceiveMtmReg->NewMtmL(KUidMsgTypeSMS);

	iSession = CMsvSession::OpenSyncL(*dummy); // new session is opened synchronously
	iMtmReg = CClientMtmRegistry::NewL(*iSession);
	iMtm = iMtmReg->NewMtmL(KUidMsgTypeSMS);

	CActiveScheduler::Add(this); // add to scheduler
}

TInt CSmsFile::send_message()//const TDesC& recipient, const TDesC& body)
{
	TRAPD(err, send_messageL(iMobileNo, iSmsText));
	if (err!=KErrNone /*&& reception_handler*/) {
		TBuf<50> errs;
		errs.Format(_L("err %d: %S"), err, &state);
		//reception_handler->handle_error(errs);
		handle_error(errs);
	}
	//send_messageL(recipient, body);
	return err;
}

void CSmsFile::send_messageL(const TDesC& recipient, const TDesC& body)
{
	if (iStatus==KRequestPending) {
		User::Leave(KErrServerBusy);
	}

	state=_L("send_messageL");
	
	TMsvEntry newEntry;								 // This represents an entry in the Message Server index
	newEntry.iMtm = KUidMsgTypeSMS;                         // message type is CSmsFile
	newEntry.iType = KUidMsvMessageEntry;                   // this defines the type of the entry: message 
	newEntry.iServiceId = KMsvLocalServiceIndexEntryId;     // ID of local service (containing the standard folders)
	newEntry.iDate.HomeTime();                              // set the date of the entry to home time
			
	newEntry.SetInPreparation(ETrue);                       // a flag that this message is in preparation
	

	// get ref to outbox
	state=_L("NewL");
	CMsvEntry *entry =CMsvEntry::NewL(*iSession, KMsvGlobalOutBoxIndexEntryId ,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);

	// create message in outbox
	state=_L("CreateL");
	entry->CreateL(newEntry);	
	
	state=_L("GetEntry");
	entry = iSession->GetEntryL(newEntry.Id());
	
	// SetCurrentEntryL takes ownership of the CMsvEntry
	state=_L("SetCurrentEntry");
	iMtm->SetCurrentEntryL(entry);

	CleanupStack::Pop(); // entry

	state=_L("Entry()");
	TMsvEntry msvEntry = (iMtm->Entry()).Entry();
	
	state=_L("Body()");
	CRichText& mtmBody = iMtm->Body();
	mtmBody.Reset();
	
	state=_L("set message");
	mtmBody.InsertL(0, body);   // insert our msg tag as the body text
	
	// set iRecipient into the Details of the entry
	msvEntry.iDetails.Set(recipient);  // set recipient info in details
	msvEntry.SetInPreparation(EFalse);         // set inPreparation to false
	
	msvEntry.SetSendingState(KMsvSendStateWaiting);   // set the sending state (immediately)
	msvEntry.iDate.HomeTime();                        // set time to Home Time
	
	// To handle the CSmsFile specifics we start using SmsMtm
	CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iMtm);
	
	state=_L("RestoreServiceAndSettingsL");
	smsMtm->RestoreServiceAndSettingsL();
	
	state=_L("set header");
	// CSmsHeader encapsulates data specific for CSmsFile messages,
	// like service center number and options for sending.
	CSmsHeader& header = smsMtm->SmsHeader();
	state=_L("get options");

	if (!sendOptions) {
		sendOptions = CSmsSettings::NewL();
		sendOptions->CopyL(smsMtm->ServiceSettings()); // restore existing settings
		state=_L("SetDelivery");
		sendOptions->SetDelivery(ESmsDeliveryImmediately);      // set to be delivered immediately
	}
	
	// set send options
	// ERROR HERE!
	state=_L("SetSmsSettingsL");
	header.SetSmsSettingsL(*sendOptions);
	
	state=_L("check sc");
	// let's check if there's sc address
	if (header.Message().ServiceCenterAddress().Length() == 0)
	{
		// no, there isn't. We assume there is at least one sc number set and use
		// the default SC number. 
		CSmsSettings* serviceSettings = &(smsMtm->ServiceSettings());
		
		// if number of scaddresses in the list is null
		if (!serviceSettings->NumSCAddresses())
		{
			// FIXME: what to do?
			User::Leave(1);
		} else {
			// set sc address to default. 
			CSmsNumber* sc = 0;
			sc = &(serviceSettings->SCAddress(serviceSettings->DefaultSC()));
			header.Message().SetServiceCenterAddressL(sc->Address());
		}
	}
	
	state=_L("add addresses");
	// Add our recipient to the list, takes in two TDesCs, first is real address and second is an alias
	// works also without the alias parameter.
	smsMtm->AddAddresseeL(recipient, msvEntry.iDetails);
	
	// if message is to be deleted after sending, mark with our UID
	msvEntry.iMtmData3 = KUidRippleVaultApp.iUid;
	
	state=_L("save");
	// save message
	iMtm->Entry().ChangeL(msvEntry);                // make sure that we are handling the right entry
	smsMtm->SaveMessageL();                 // closes the message
	
	state=_L("move");
	// This moves the message entry to outbox, we'll schedule it for sending after this. 
	//    TMsvId movedId = MoveMessageEntryL( KMsvGlobalOutBoxIndexEntryId );  // move message to outbox
	TMsvId movedId = iMtm->Entry().Entry().Id();
	
	// We must create an entry selection for message copies (although now we only have one message in selection)
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	selection->AppendL(movedId);            // add our message to the selection
	
	TBuf8<1> dummyParams;
	//    CCommandAbsorbingControl::NewLC();
	
	state=_L("InvokeAsyncFunctionL");
	// invoking async schedule copy command on our mtm
	op=iMtm->InvokeAsyncFunctionL(
		ESmsMtmCommandScheduleCopy,
		*selection,
		dummyParams,
		iStatus);
	
	CleanupStack::Pop(); // selection

#ifdef __LOGME__
	iAppUi.LogText.Copy(_L("SENT\n"));
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif

	SetActive();
	
	//return KErrNone;
}

void CSmsFile::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
{
	switch(aEvent) {
/*	case EMsvEntriesCreated:
		{
			if (!aArg1 || !aArg2 || !iReceiveMtm) return;
			CMsvEntrySelection* entries=(CMsvEntrySelection *)aArg1;
			TMsvId id=*(TMsvId*)aArg2;
			if (id != KMsvGlobalInBoxIndexEntryId) return;
			
			for (int i=0; i< entries->Count(); i++) 
			{
				//handle_received(entries->At(i), id);
			}
		}
		break;
*/
	case EMsvEntriesMoved:      // this event is given when message entries are moved
		{
			// An entry has been moved to another parent
			// We are interested messages that have been moved to Sent folder
			if (!aArg1 || !aArg2 || !aArg3 ) return;
			
			TMsvId* toEntryId;
			TMsvId* fromEntryId;
			toEntryId = static_cast<TMsvId*>(aArg2); 
			fromEntryId = static_cast<TMsvId*>(aArg3);
			
			CMsvEntrySelection* entries = static_cast<CMsvEntrySelection*>(aArg1);

			if ( *toEntryId == KMsvSentEntryId )   // the entry has been moved into Sent folder
			{
				// We take the moved entries into a selection
						
				//Process each created entry, one at a time.
				for(TInt i = 0; i < entries->Count(); i++)
				{
					DeleteSentEntry(entries->At(i)); // this checks the entry and deletes if it is created by GDSMS app
					//handle_sent(entries->At(i)); 
				}

			}
			
//			for(TInt i = 0; i < entries->Count(); i++)
//			{
				//handle_moved(entries->At(i), *fromEntryId, *toEntryId);
//			}
		}
		break;

/*	case EMsvEntriesChanged:
		{
			if (!aArg1) return;

			CMsvEntrySelection* entries = static_cast<CMsvEntrySelection*>(aArg1);
			TMsvId* ParentFolder = static_cast<TMsvId*>(aArg2);
			
			// 'changed' event + Inbox folder = Read
			if (*ParentFolder == KMsvGlobalInBoxIndexEntryId )
			{
				for(TInt i = 0; i < entries->Count(); i++)
				{
					//handle_read(entries->At(i));
				}
			}
			else
			{
				for(TInt i = 0; i < entries->Count(); i++)
				{
					//handle_changed(entries->At(i));
				}
			}
		}
		break;

	case EMsvEntriesDeleted:
		{
				
			if (!aArg1 || !aArg2) return;

			TMsvId* folderId;
			folderId = static_cast<TMsvId*>(aArg2); 
				
			CMsvEntrySelection* entries = static_cast<CMsvEntrySelection*>(aArg1);
						
			for(TInt i = 0; i < entries->Count(); i++)
			{
				handle_deleted(entries->At(i), *folderId);
			}
			
			
				
			
		}
		break;
*/
	default:
		break;
	}
	
}

TBool CSmsFile::DeleteSentEntry(TMsvId entry)
{
	TInt err;
	// Load this entry to our mtm
	TRAP(err, iReceiveMtm->SwitchCurrentEntryL(entry));
	// probably wasn't compatible, ignore
	if (err!=KErrNone) return EFalse;
	TRAP(err, iReceiveMtm->LoadMessageL());
	// probably wasn't compatible, ignore
	if (err!=KErrNone) return EFalse;
	
	TMsvEntry msvEntry( (iReceiveMtm->Entry()).Entry() );
		
        if (msvEntry.iMtmData3 == KUidRippleVaultApp.iUid)    // this entry has been created by our app
	{
		// Taking a handle to the Sent folder...
		TMsvSelectionOrdering sort;
		sort.SetShowInvisibleEntries(ETrue);    // we want to handle also the invisible entries
		// Take a handle to the parent entry
		CMsvEntry* parentEntry = CMsvEntry::NewL(iReceiveMtm->Session(), msvEntry.Parent(), sort);
		CleanupStack::PushL(parentEntry);
		
		// here parentEntry is the Sent folder (must be so that we can call DeleteL) 
		parentEntry->DeleteL(msvEntry.Id());

		CleanupStack::Pop();

			//if(iBillSms == 1)
				//iAppUi.CheckBillingStatus(5);

		return ETrue; // entry was deleted
	}
	
	return EFalse; // no entries deleted
}

void CSmsFile::handle_error(const TDesC& descr)
{
#ifdef __LOGME__
	iAppUi.LogText.Copy(descr);
	iAppUi.WriteLogFile(iAppUi.LogText);
#endif
}

bool CSmsFile::loadmessageL(const TMsvId& entry_id, TMsvEntry& entry)
{

	TInt err;
	//if (!reception_handler) return;
	
	CMsvEntry *realentry;
	TRAP(err, realentry=iReceiveSession->GetEntryL(entry_id) );

	CleanupStack::PushL(realentry);
	if (err!=KErrNone) 
	{
		TBuf<50> msg;
		msg.Format(_L("Error in GetEntry %d"), err);
		handle_error(msg);
		return false;
	}
	entry=realentry->Entry();
	if (entry.iMtm != KUidMsgTypeSMS) 
	{
		handle_error(_L("Not an CSmsFile"));
		return false;
	}
	
	// SetCurrentEntryL takes ownership of the CMsvEntry
	TRAP(err, iReceiveMtm->SetCurrentEntryL(realentry));
	
	CleanupStack::Pop(); // entry

	if (err!=KErrNone) 
	{
		TBuf<50> msg;
		msg.Format(_L("Error in SetCurrentEntryL: %d"), err);
		handle_error(msg);
		return false;
	}
	TMsvPartList validationFlags(KMsvMessagePartBody|
		KMsvMessagePartDescription|KMsvMessagePartOriginator);
	if (iReceiveMtm->ValidateMessage(validationFlags != 0)) 
	{
		return false;
	}
	
	// The message loading sometimes fails with KErrAccessDenied - the message server is
	// busy. Just retry loading. 
	TInt retry_count=0;
	TInt RETRIES=5;
	while (retry_count<RETRIES) 
	{
		TRAP(err, iReceiveMtm->LoadMessageL());
		if (err==KErrAccessDenied || err==KErrInUse) {
			retry_count++;
			User::After(TTimeIntervalMicroSeconds32(100*1000)); //100 ms
		} else if (err==-1) {
			// can ignore
			return false;
		} else if (err!=KErrNone) {
			TBuf<50> msg;
			msg.Format(_L("Error in LoadMessageL: %d"), err);
			handle_error(msg);
			return false;
		} else {
			break;
		}
	}
	if (retry_count==RETRIES) {
		//reception_handler->handle_error(_L("max retries on LoadMessageL reached"));
		handle_error(_L("max retries on LoadMessageL reached"));
		return false;
	}
	return true;
}

void CSmsFile::handle_received(const TMsvId& entry_id, const TMsvId& folder_id)
{

	//TInt err;

	TMsvEntry entry;
	if (!loadmessageL(entry_id, entry)) return;

}

void CSmsFile::handle_deleted(const TMsvId& entry_id, const TMsvId& parent_id)
{

	CMsvEntry* realentry;
	TRAPD(err, realentry = iReceiveSession->GetEntryL(entry_id));
	if (err!=KErrNone) 
	{
		return;
	}
	if (realentry->Entry().iMtm != KUidMsgTypeSMS) {
		return;
	}

	//CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iReceiveMtm);
	
}

void CSmsFile::handle_sent(const TMsvId& entry_id)
{
	TMsvEntry entry;
	if (!loadmessageL(entry_id, entry)) return;
		
	//CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iReceiveMtm);

	//-------------------------------------------------------------------------------------------
}

void CSmsFile::handle_moved(const TMsvId& /*entry_id*/, const TMsvId& /*from_entry_id*/, const TMsvId& /*to_entry_id*/)
{
	/*CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iReceiveMtm);

	CList<i_handle_received_sms*>::Node* i=iHandlers->iFirst;
	while (i) 
	{
		i->Item->handle_move(entry_id, from_entry_id, to_entry_id, smsMtm->SmsHeader().FromAddress());
		i=i->Next;
	}*/

}

void CSmsFile::handle_changed(const TMsvId& /*entry_id*/)
{
	/*CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iReceiveMtm);
	
	CList<i_handle_received_sms*>::Node* i=iHandlers->iFirst;
	while (i) 
	{
		//if ( buf.Length()<=0)
		//{
			i->Item->handle_change(entry_id, _L(""));
		//}
		//else
		//{
		//	i->Item->handle_change(entry_id, smsMtm->SmsHeader().FromAddress());
		//}
		i=i->Next;
	}*/
}

void CSmsFile::handle_read(const TMsvId& entry_id)
{
	//CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iReceiveMtm);

}

void CSmsFile::CmdSendL(TDesC& mobileno, TDesC& smstext, TInt type)
    {
	iBillSms = 0;

	if(type==0){
		iMobileNo.Copy(_L("+"));
	}
	else{
		iMobileNo.Copy(_L(""));
	}

	iMobileNo.Append(mobileno);

	if(type == 0)
		iSmsText.Copy(_L("//simrippleprobe"));
	else if(type == 1)
		iSmsText.Copy(smstext);

    if (!send_message())
        {
        //iEikonEnv->InfoWinL(_L("Error"),_L("Problems in sending\nmessage."));
        return;
        }
    }

void CSmsFile::CmdSendBillingL(TDesC& file, TDesC& message)
    {
	iBillSms = 1;

	//iMobileNo.Copy(_L("+"));
	iMobileNo.Copy(file);
	iSmsText.Copy(message);

    if (!send_message())
        {
        //iEikonEnv->InfoWinL(_L("Error"),_L("Problems in sending\nmessage."));
        return;
        }
    }

void CSmsFile::RunL()
{




	TBuf<50> sharingName;
	sharingName.Copy(_L(""));
		
	iAppUi.GetSharingMails(sharingName);		

	if(sharingName.Length()!=0)
	{
		iMobileNo.Copy(sharingName);
		send_message();
	}
	else
	{
		iAppUi.iPhotosDB->DeleteSharingsFirstRecord();
	}
}
