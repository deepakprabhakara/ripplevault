/*
* ============================================================================
*  Name     : CSmsFile from SmsFile.h
*  Part of  : FindBox
*  Created  : 6/15/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleAce
* ============================================================================
*/

#ifndef SMSFILE_H
#define SMSFILE_H

// INCLUDES
#ifndef __UIQ__
#include <aknlists.h> 
#include <aknsfld.h>  
#include <aknutils.h>
#include <akniconarray.h> 
#include <eikfrlb.h> 
#include <aknwaitdialog.h> 
#endif

#include <coecntrl.h>
#include <cntitem.h>
#include <cntfldst.h>
#include <eiktxlbx.h>
#include <eikclb.h>
#include <eikclbd.h>
#include <msvapi.h>                         // for MMsvSessionObserver 

// CLASS DECLARATION
class CPhotosBackupAppUi;
class CClientMtmRegistry;
class CMsvSession;
class CBaseMtm;

/**
*  CSmsFile  container control class.
*  
*/
class CSmsFile : public CCoeControl, MCoeControlObserver, MMsvSessionObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        //void ConstructL(const TRect& aRect);
		static CSmsFile* NewL(const TRect& aRect);
		
		CSmsFile();

		void ConstructL(const TRect& aRect);

		CPhotosBackupAppUi* iPhotosAppUi;	

		/**
        * Destructor.
        */
        ~CSmsFile();

    public: // New functions		

		/*void GetNames(TInt iCursorPos);
		TInt ListMails();
		void CopyMailId(TInt idContact);
		void RemoveMailId(TInt idContact);
		void SaveContacts();
		void ReadMessageFromFile();
		void SendOtherNumber();
		TInt OkPressKey();*/

		TBuf<200> iMobileNo;
		TBuf<300> iSmsText;		
		TInt iMode;

    public: // Functions from base classes		
		TBool MessageReceivedL(TMsvId aEntryId);
		void CmdSendL(TDesC& file, TInt aMode);
		void CmdSendLForUnSubScribe(TDesC& file,TDesC& msg);
		void CmdExitL();	
    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();
		
       /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

       /**
        * From CCoeControl,Draw.
        */
        void Draw(const TRect& aRect) const;

       /**
        * From ?base_class ?member_description
        */
        // event handling section
        // e.g Listbox events
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
      
    private: //data		
	private:
		void CompleteConstructL();				
		TBool InitializeCommunicationsL();
		TMsvId CreateNewMessageL();
		TBool SendMessageL();
		void SetEntryL(TMsvId aEntryId);
		TMsvId MoveMessageEntryL(TMsvId aTarget);
		void SetScheduledSendingStateL(CMsvEntrySelection* aSelection);
		TBool DeleteSentEntry(TMsvId aEntryId);
		void DeleteMessagesFromInboxL();

	private:    // from MMsvSessionObserver
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
    
	private:
		HBufC* iRecipient;              // recipients gsm number

		TMsvId iMsvId;                  // message server entry id
		CMsvSession* iSession;          // Client session on the message server
		CBaseMtm* iMtm;                 // Message Type Module (sms)
		CClientMtmRegistry* iMtmReg;    // Mtm client registry for creating new mtms		
    };

#endif

// End of File
