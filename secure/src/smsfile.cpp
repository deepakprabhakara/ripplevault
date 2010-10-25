/*
* ============================================================================
*  Name     : CSmsFile from SmsFile.cpp
*  Part of  : FindBox
*  Created  : 6/15/2004 by Ezhil
*  Implementation notes:
*     Initial content was generated by Series 60 AppWizard.
*  Version  :
*  Copyright: PurpleAce
* ============================================================================
*/

// INCLUDE FILES
#include "SmsFile.h"
#include "SecureAppUi.h"
#include <badesca.h> 

#include <bautils.h>				// file helpers
#include <txtrich.h>                // for CRichText
#include <smscmds.h>                // for TSmsMtmCommand (asynchronous sms commands)
#include <eikenv.h>                 // for CEikonEnv
#include <smuthdr.h>                // for CSmsHeader
#include <smsetdlg.h>               // for CSmsAddServiceCentreDialog
#include <comabs.h>                 // for CCommandAbsorbingControl
#include <mtclreg.h>                // for CClientMtmRegistry 
#include <smutset.h>                // for CSmsMessageSettings

#ifdef __UIQ__	
#ifdef __S80__
	#include <secure_s80.rsg>
#else
	#include <secure_uiq.rsg>
#endif
#else
	#include <secure.rsg>	
#endif	

// this is the content of the message
//_LIT(KGDSMSTag, "I have tried this new service. Click on the link to find out more http://61.95.206.73:8080/ripple/client?action=p1");


// Own constants
const TUid KUidGDSMS = { 0x102032D0 };      // GDSMS application UID 
//const TInt KTagLength = 4;                  // length of our message tag
const TInt KMaxTelephoneNumberLength = 30;  // maximum length for a gsm number


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSmsFile::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

CSmsFile::CSmsFile()
	{
	}


//             Static NewL() function to start the standard two
//             phase construction.
//
CSmsFile* CSmsFile::NewL(const TRect& aRect)
	{
	CSmsFile* self = new(ELeave) CSmsFile();
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	}



//             Second phase construction.
//
void CSmsFile::ConstructL(const TRect& aRect)
    {
	// Control is a window owning control
	//CreateWindowL();
	           // Extent of the control. This is
	           // the whole rectangle available to application.
	           // The rectangle is passed to us from the application UI. 

	iRecipient=HBufC::NewL(KMaxTelephoneNumberLength);  // for recipient sms number
	iMsvId = NULL;                                      // MsvId for keeping track of the message server entries. 

	// Create CMsvSession
	iSession = CMsvSession::OpenAsyncL(*this);

	 
	// load the bitmap from an .mbm file	           
	//SetRect(aRect);
			   // At this stage, the control is ready to draw so
	           // we tell the UI framework by activating it.
	ActivateL();
	}

// Destructor
CSmsFile::~CSmsFile()
    {
		delete iRecipient;

		delete iMtm;
		delete iMtmReg;

		iMsvId = NULL;

		delete iSession;
    }

// ---------------------------------------------------------
// CSmsFile::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CSmsFile::SizeChanged()
    {
    }


// ---------------------------------------------------------
// CSmsFile::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CSmsFile::CountComponentControls() const
    {
    TInt count=0;
    return count; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CSmsFile::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CSmsFile::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CSmsFile::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CSmsFile::Draw(const TRect& aRect) const
    {
    /*CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbGray);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);*/
    }

// ---------------------------------------------------------
// CSmsFile::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CSmsFile::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


/*
-----------------------------------------------------------------------------

    CSmsFile::CompleteConstructL()

    Creates client MTM registry when session is ready for use. 
    This completes model construction and is called after 'server
    ready' event is received after async opening of CMsvSession.

-----------------------------------------------------------------------------
*/

void CSmsFile::CompleteConstructL()
    {
    // We get a MtmClientRegistry from our session
    // this registry is used to instantiate new mtms.
    iMtmReg = CClientMtmRegistry::NewL(*iSession);    
    }


/*
-----------------------------------------------------------------------------

    CSmsFile::HandleSessionEventL()

    Handles session event observer and calls event handling functions in
    observer. Note that if additional session event handlers are defined in
    the session, they are called before this function (as this is the
    main session observer).

    The type of event is indicated by the value of aEvent. The 
    interpretation of the TAny arguments depends on this type. For most 
    event types, the action that is taken, for example, updating the 
    display, is client-specific. All clients though should respond to 
    EMsvCloseSession and EMsvServerTerminated events. 

-----------------------------------------------------------------------------
*/
void CSmsFile::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
    {
    switch (aEvent)
        {

        /*case EMsvEntriesCreated:        // A new entry has been created in the message server
            // We are interested in messages that are created in Inbox
            TMsvId* entryId;
            entryId = static_cast<TMsvId*>(aArg2);          // entry id from the session event
            
            if ( *entryId == KMsvGlobalInBoxIndexEntryId )  // new entry has been created in Inbox folder
                {
                // We take the created entries into a selection
                CMsvEntrySelection* entries = static_cast<CMsvEntrySelection*>(aArg1);

                // entry pointer for making changes in the actual message contexts
                CMsvEntry* entry;

                //Process each created entry, one at a time.
                for(TInt i = 0; i < entries->Count(); i++)
                    {
                    // Setting all messages in the selection as invisible
                    entry = iSession->GetEntryL( entries->At(i) );  // this reserves memory for a new CMsvEntry
                    TMsvEntry msvEntry(entry->Entry());
                    msvEntry.SetVisible(EFalse);                    // set as invisible

                    if( MessageReceivedL(entries->At(i)) ) // this checks the entry and handles it if it is targeted to GDSMS app   
                        {
                        // this is our message, set also as read
                        msvEntry.SetUnread(EFalse);
                        msvEntry.iMtmData3 = KUidGDSMS.iUid;        // use our app uid as an identifier
                        }
                    else
                        {
                        // message was not for us, settin it as visible again
                        msvEntry.SetVisible(ETrue);
                        }

                    entry->ChangeL( msvEntry );                     // commit changes
                    delete entry;
                    }
				}
            break;
*/
        case EMsvEntriesMoved:      // this event is given when message entries are moved
            {
            // An entry has been moved to another parent
            // We are interested messages that have been moved to Sent folder
            TMsvId* entryId;
            entryId = static_cast<TMsvId*>(aArg2); 

            if ( *entryId == KMsvSentEntryId )   // the entry has been moved into Sent folder
                {
                // We take the moved entries into a selection
                CMsvEntrySelection* entries = static_cast<CMsvEntrySelection*>(aArg1);

                //Process each created entry, one at a time.
                for(TInt i = 0; i < entries->Count(); i++)
                    {
                    DeleteSentEntry(entries->At(i)); // this checks the entry and deletes if it is created by GDSMS app                
                    }
                }
            }
			break;

            // This event tells us that the session has been opened
        case EMsvServerReady:
            CompleteConstructL();       // Construct the mtm registry & sms mtm
            break;

        default:
            // All other events are ignored
            break;
        }

    }

/*
-----------------------------------------------------------------------------

    CSmsFile::CmdSendL()

    Handle send command  
    
-----------------------------------------------------------------------------
*/

void CSmsFile::CmdSendL(TDesC& file, TInt aMode)
    {
	iMobileNo.Copy(file);	
	iMode = aMode;

	if(iMode == 0)
		iSmsText.Copy(*(iEikonEnv->AllocReadResourceL(R_SMS_TEXT)));
	else if(iMode == 1)
		iSmsText.Copy(*(iEikonEnv->AllocReadResourceL(R_SMS_FREEZE_TEXT)));
	else if(iMode == 2)
		iSmsText.Copy(*(iEikonEnv->AllocReadResourceL(R_SMS_FREEZE_VACUUM_TEXT)));
		
    if (!InitializeCommunicationsL())
        {
        //iEikonEnv->InfoWinL(_L("Error"),_L("Problems in initializing\ncommunications."));
        return;
        }
    if (!SendMessageL())
        {
        //iEikonEnv->InfoWinL(_L("Error"),_L("Problems in sending\nmessage."));
        return;
        }
    }

void CSmsFile::CmdSendLForUnSubScribe(TDesC& file,TDesC& msg)
    {
	iMobileNo.Copy(file);

	iSmsText.Copy(msg);

    if (!InitializeCommunicationsL())
        {
        iEikonEnv->InfoWinL(_L("Error"),_L("Problems in initializing\ncommunications."));
        return;
        }
    if (!SendMessageL())
        {
        iEikonEnv->InfoWinL(_L("Error"),_L("Problems in sending\nmessage."));
        return;
        }
    }

/*
-----------------------------------------------------------------------------

    CSmsFile::CmdExitL()
    
    Exit application
  
-----------------------------------------------------------------------------
*/

void CSmsFile::CmdExitL()
    {
		DeleteMessagesFromInboxL();    
    }



/*
-----------------------------------------------------------------------------

    CSmsFile::InitializeCommunicationsL()
    
    Initialize a new message and ask the user for a recipient gsm number.

    Return values:      ETrue or EFalse

-----------------------------------------------------------------------------
*/
TBool CSmsFile::InitializeCommunicationsL()
    { 
    // first the tel number
    // we get it from our telNumDialog	
    
	iRecipient->Des().Copy(iMobileNo);

    // set up a new message
    iMsvId = CreateNewMessageL();
    // Set the new message to be the current entry
    SetEntryL(iMsvId);

    return ETrue;
    }



/*
-----------------------------------------------------------------------------

    CSmsFile::CreateNewMessageL()

    Creates a new message server entry and set up default values.

    Return values:      TMsvId (the id of created entry)

-----------------------------------------------------------------------------
*/
TMsvId CSmsFile::CreateNewMessageL()
	{
    TMsvEntry newEntry;              // This represents an entry in the Message Server index
    newEntry.iMtm = KUidMsgTypeSMS;                         // message type is SMS
    newEntry.iType = KUidMsvMessageEntry;                   // this defines the type of the entry: message 
    newEntry.iServiceId = KMsvLocalServiceIndexEntryId;     // ID of local service (containing the standard folders)
    newEntry.iDate.HomeTime();                              // set the date of the entry to home time
    newEntry.SetInPreparation(ETrue);                       // a flag that this message is in preparation

    //----
    //newEntry.iBioType = 0x1000ffff;                         // define a bio UID if sending a bio message over SMS bearer
    //----

    // - CMsvEntry accesses and acts upon a particular Message Server entry.
    // - NewL() does not create a new entry, but simply a new object to access an existing entry.
    // - It takes in as parameters the client's message server session,
    //   ID of the entry to access and initial sorting order of the children of the entry. 
	//
    CMsvEntry* entry = CMsvEntry::NewL(*iSession, KMsvDraftEntryIdValue ,TMsvSelectionOrdering());
    CleanupStack::PushL(entry);
    CCommandAbsorbingControl::NewLC();
    CMsvOperationWait* wait = CMsvOperationWait::NewLC();
    wait->Start();
    // We create a new entry asynchronously...
    CMsvOperation* oper = entry->CreateL(newEntry,wait->iStatus);
    CleanupStack::PushL(oper);
    CActiveScheduler::Start();

    // ...and keep track of the progress of the create operation.
    TMsvLocalOperationProgress progress = McliUtils::GetLocalProgressL(*oper);
    User::LeaveIfError(progress.iError);

    // Set our entry context to the created one
    entry->SetEntryL(progress.iId); // operation progress contains the ID of the ceated entry

    CleanupStack::PopAndDestroy(4); // entry,oper,wait, CCommandAbsorbingControl

    return progress.iId;
	}



/*
-----------------------------------------------------------------------------

    CSmsFile::SetEntryL(TMsvId aEntryId)

    Set up current message entry.

    Note: It can be useful to remember the original entry id for 
          error handling.

-----------------------------------------------------------------------------
*/

void CSmsFile::SetEntryL(TMsvId aEntryId)
    {
	// Get the server entry from our session
    CMsvEntry* entry = iSession->GetEntryL(aEntryId);
    CleanupStack::PushL(entry);
    
    // Check if our mtm is different from the mtm set to our entry
    if (iMtm == NULL || entry->Entry().iMtm != (iMtm->Entry()).Entry().iMtm)
        {
        // If so, we delete the old... 
        delete iMtm;
        iMtm = NULL;
        
        // ...and get a new one from the MtmRegistry
        iMtm = iMtmReg->NewMtmL(entry->Entry().iMtm);
        iMtm->SetCurrentEntryL(entry);  
        }
    else
        {
        // if there is no need to change our mtm,
        // we only set our entry as current.
        iMtm->SetCurrentEntryL(entry);
        }

    CleanupStack::Pop(); //entry
    entry = NULL;
	}



/* 
-----------------------------------------------------------------------------
    CSmsFile::SendMessageL()

    Prepares the message body and sends the message.

    Return values:      ETrue or EFalse

-----------------------------------------------------------------------------
*/
TBool CSmsFile::SendMessageL()
    {

    TMsvEntry msvEntry = iMtm->Entry().Entry();

    // We get the message body from Mtm and insert a bodytext
    CRichText& mtmBody = iMtm->Body();
    mtmBody.Reset();
    mtmBody.InsertL(0, iSmsText);   // insert our msg tag as the body text
    
    // set iRecipient into the Details of the entry
    msvEntry.iDetails.Set(iRecipient->Des());  // set recipient info in details
    msvEntry.SetInPreparation(EFalse);         // set inPreparation to false

    msvEntry.SetSendingState(KMsvSendStateWaiting);   // set the sending state (immediately)
	msvEntry.iDate.HomeTime();                        // set time to Home Time
 

    // To handle the sms specifics we start using SmsMtm
    CSmsClientMtm* smsMtm = STATIC_CAST(CSmsClientMtm*, iMtm);


    // 
    smsMtm->RestoreServiceAndSettingsL();
 
	// CSmsHeader encapsulates data specific for sms messages,
    // like service center number and options for sending.
    CSmsHeader& header = smsMtm->SmsHeader();
    CSmsSettings* sendOptions = CSmsSettings::NewL();
    CleanupStack::PushL(sendOptions);
    sendOptions->CopyL(smsMtm->ServiceSettings()); // restore existing settings

    // set send options
    sendOptions->SetDelivery(ESmsDeliveryImmediately);      // set to be delivered immediately
    header.SetSmsSettingsL(*sendOptions);

	// let's check if there's sc address
	if (header.Message().ServiceCenterAddress().Length() == 0)
		{
		// no, there isn't. We assume there is at least one sc number set and use
		// the default SC number. 
		CSmsSettings* serviceSettings = &(smsMtm->ServiceSettings());
        
		// if number of scaddresses in the list is null
        if (!serviceSettings->NumSCAddresses())
            {
			// here there should be a dialog in which user can add sc number
			//iEikonEnv->InfoWinL(_L("No service center number"),_L("cannot send this one."));
			}
		else
			{
            // set sc address to default. 
            CSmsNumber* sc = 0;
			sc = &(serviceSettings->SCAddress(serviceSettings->DefaultSC()));
			header.Message().SetServiceCenterAddressL(sc->Address());
			}
		}
	CleanupStack::PopAndDestroy(); // send options 
    
    // Add our recipient to the list, takes in two TDesCs, first is real address and second is an alias
    // works also without the alias parameter.
	smsMtm->AddAddresseeL(iRecipient->Des(),msvEntry.iDetails);
    
    // Next we mark our message so later on we know which
    // message to delete from the Sent folder
    msvEntry.iMtmData3 = KUidGDSMS.iUid;     // use our app uid as an identifier

    // save message
    CMsvEntry& entry = iMtm->Entry();
    entry.ChangeL(msvEntry);                // make sure that we are handling the right entry
	smsMtm->SaveMessageL();                 // closes the message

    // This moves the message entry to outbox, we'll schedule it for sending after this. 
    TMsvId movedId = MoveMessageEntryL( KMsvGlobalOutBoxIndexEntryId );  // move message to outbox

 
    // We must create an entry selection for message copies (although now we only have one message in selection)
    CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
    CleanupStack::PushL(selection);

    selection->AppendL(movedId);            // add our message to the selection
    SetScheduledSendingStateL(selection);   // schedule the sending with the active scheduler

    CleanupStack::PopAndDestroy(); // selection

    return ETrue; // at this point the message has been sent

    }

/* 
-----------------------------------------------------------------------------
    CSmsFile::MoveMessageEntryL(TMsvId aTarget) const

    Moves an entry to another parent.

    Return values:      TMsvId of the moved message

-----------------------------------------------------------------------------
*/
TMsvId CSmsFile::MoveMessageEntryL( TMsvId aTarget )
    {

	TMsvEntry msvEntry( (iMtm->Entry()).Entry() );
    TMsvId id = msvEntry.Id();

	if (msvEntry.Parent() != aTarget)
		{
		TMsvSelectionOrdering sort;
		sort.SetShowInvisibleEntries(ETrue);    // we want to handle also the invisible entries
        // Take a handle to the parent entry
		CMsvEntry* parentEntry = CMsvEntry::NewL(iMtm->Session(), msvEntry.Parent(), sort);
		CleanupStack::PushL(parentEntry);
		
		// Move original from the parent to the new location
		CCommandAbsorbingControl::NewLC();
		CMsvOperationWait* wait = CMsvOperationWait::NewLC();
		wait->Start();
		
		CMsvOperation* op = parentEntry->MoveL(msvEntry.Id(), aTarget, wait->iStatus);
		
		CleanupStack::PushL(op);
		CActiveScheduler::Start();		 
		TMsvLocalOperationProgress prog=McliUtils::GetLocalProgressL(*op);
		User::LeaveIfError(prog.iError);
		
		id = prog.iId; // id of the moved entry
		
		CleanupStack::PopAndDestroy(4);// op, wait, parentEntry, CCommandAbsorbingControl
		}
	return id;
    }


/*
-----------------------------------------------------------------------------

    void CSmsFile::SetScheduledSendingStateL

    Schedules the message to be sent through the etel server.

    Return values:      none

-----------------------------------------------------------------------------
*/

void CSmsFile::SetScheduledSendingStateL(CMsvEntrySelection* aSelection)
    {
    
    CBaseMtm* smsMtm = iMtm;

    // Add entry to task scheduler
    TBuf8<1> dummyParams;
    CCommandAbsorbingControl::NewLC();
    CMsvOperationWait* waiter = CMsvOperationWait::NewLC();
    waiter->Start();
    // invoking async schedule copy command on our mtm
    CMsvOperation* op= smsMtm->InvokeAsyncFunctionL(
            ESmsMtmCommandScheduleCopy,
            *aSelection,
            dummyParams,
            waiter->iStatus);
    CleanupStack::PushL(op);
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy(3); // waiter, op, CCommandAbsorbingControl
	}

/*
-----------------------------------------------------------------------------

    CSmsFile::DeleteSentEntry()

    Delete our message from the Sent folder. We are double checking that
    the entry we are handling is indeed in the Sent folder - AND that it
    is the same message entry that wsa sent by this application.

    If so, the message will be deleted from the Sent folder.

    Return values:      ETrue or EFalse

-----------------------------------------------------------------------------
*/
TBool CSmsFile::DeleteSentEntry(TMsvId aEntryId)
    {
	
    // Load this entry to our mtm
    SetEntryL( aEntryId ); 

    TMsvEntry msvEntry( (iMtm->Entry()).Entry() );


    if (msvEntry.Parent() == KMsvSentEntryId) // check again that our entry is in sent
		{

        if (msvEntry.iMtmData3 == KUidGDSMS.iUid)    // this entry has been created by our app
            {
            // Taking a handle to the Sent folder...
		    TMsvSelectionOrdering sort;
		    sort.SetShowInvisibleEntries(ETrue);    // we want to handle also the invisible entries
            // Take a handle to the parent entry
		    CMsvEntry* parentEntry = CMsvEntry::NewL(iMtm->Session(), msvEntry.Parent(), sort);
		    CleanupStack::PushL(parentEntry);
		
            // here parentEntry is the Sent folder (must be so that we can call DeleteL) 
		    parentEntry->DeleteL(msvEntry.Id());
		
		    CleanupStack::PopAndDestroy(parentEntry);
            
            // information to the user
            //iEikonEnv->InfoMsg(_L("Message deleted in SENT folder."));

            return ETrue; // entry was deleted
            }
        }
    return EFalse; // no entries deleted
    }


/*
-----------------------------------------------------------------------------

    CSmsFile::MessageReceivedL()

    Handle a new message entry that has been created on the messaging
    server. This method checks if the message was targeted for this
    application and then removes it from the server.

-----------------------------------------------------------------------------
*/
TBool CSmsFile::MessageReceivedL(TMsvId aEntryId)
    {

    TBool returnVal = EFalse;

    CMsvEntry* entry = iSession->GetEntryL(aEntryId);
    CleanupStack::PushL(entry);

    TMsvEntry msvEntry = entry->Entry();
    
    // first we create a new mtm to handle this message (in case our own mtm is in use)
    CBaseMtm* smsMtm = iMtmReg->NewMtmL(msvEntry.iMtm);
    smsMtm->SwitchCurrentEntryL(aEntryId);

    smsMtm->LoadMessageL();     // load the message

    if (smsMtm->Body().Read(0,4).Compare(iSmsText)==0) // message is targeted to us
        {
        // Now we process the message
        //iEikonEnv->InfoMsg(smsMtm->Body().Read(0,4)); // this will flash our message text in the upper right corner of the screen
        
        returnVal = ETrue;
		}

  
    // release allocated memory
    delete smsMtm; 

    CleanupStack::Pop(); // entry
    delete entry;
    return returnVal;
    }

/*
-----------------------------------------------------------------------------

    CSmsFile::DeleteMessagesFromInboxL()

    This method reads the Inbox and deletes all GDSM messages that are 
    invisible.

-----------------------------------------------------------------------------
*/
void CSmsFile::DeleteMessagesFromInboxL()
    {
    // information to the user
    //iEikonEnv->InfoMsg(_L("Deleting messages from Inbox."));

    // then delete message from inbox, first take a handle to Inbox...
    TMsvSelectionOrdering sort;
	sort.SetShowInvisibleEntries(ETrue);    // we want to handle also the invisible entries
    // Take a handle to the Inbox entry
	CMsvEntry* parentEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, sort);
	CleanupStack::PushL(parentEntry);
	
    CMsvEntrySelection* entries = parentEntry->ChildrenL();   // A selection of all the entries of the Inbox
    CleanupStack::PushL(entries);

    // go through all entries in the Inbox
    for(TInt i = 0; i < entries->Count(); i++)
        {
        if( parentEntry->ChildDataL(entries->At(i)).iMtmData3 == KUidGDSMS.iUid )   // check that message is for GDSMS
            {
            // delete the current entry (message)
	        parentEntry->DeleteL(entries->At(i));
            }
        }
    
		
    // information to the user
    //iEikonEnv->InfoMsg(_L("Done."));

	CleanupStack::PopAndDestroy(2);   // entries, parentEntry
	}
// End of File  