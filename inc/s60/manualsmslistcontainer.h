/*
* ============================================================================
*  Name     : CManualSmsListContainer from PhotoListContainer.h
*  Part of  : 
*  Created  : 17/06/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef MANUALSMSLISTCONTAINER_H
#define MANUALSMSLISTCONTAINER_H

// INCLUDES
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <coecntrl.h>
#include <aknlists.h>
#include <akntabgrp.h>
#include <aknnavide.h>
#include <coecntrl.h>

#include <aknsfld.h>  
#include <aknutils.h>
#include <akniconarray.h> 
#include <eiktxlbx.h>
#include <eikfrlb.h> 
#include <eikclb.h>
#include <eikclbd.h>
#include <msvapi.h> 

// CLASS DECLARATION
class CRippleVaultAppUi;
class CMsvSession;
class MMsvSessionObserver;
class CReadSmsAO;
//class CWriteSmsAO;

/**
*  CManualSmsListContainer  container control class.
*  
*/

//class CManualSmsListContainer : public CCoeControl, MCoeControlObserver
class CManualSmsListContainer : public CCoeControl, MMsvSessionObserver
    {

    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */

        void ConstructL(const TRect& aRect);

		CRippleVaultAppUi* iPhotosAppUi;
		
		CAknSearchField* CreateFindBoxL(CEikListBox* aListBox,
            CTextListBoxModel* aModel,
				CAknSearchField::TSearchFieldStyle aStyle );
		
        /**
        * Destructor.
        */

        ~CManualSmsListContainer();

    public: // New functions
				
		void SizeChangedForFindBox();
		
		void GetInboxSms();
			
		void GetSentItemSms();
			
		void CreateListBox();

		void ShowListBox();	
		
		void CloseReadSms();
		
		void CloseWriteSms();
		
		void SetFileList(TInt val);

		void SendSelected();

		void MarkSelectedItem();
		
		//void WriteSmsToFile(TInt aId,TInt aSmsState);

		void MarkUnMarkSelectedItem();
		void MarkAllItems();
		void UnMarkAllItems();
		void MarkSelectedPhoto();
		void UnMarkSelectedPhoto();
		
		TInt IsImageSelected();
		TInt GetSelectedSmsCount();		
		CDesCArray* items;
		TInt iInboxCount;
		TInt iSentItemCount;
		TInt iFlag;
		TInt iCount1;
		TInt iCount2;

		TInt iTempCount;
		
		CArrayFix<TInt>* iInboxSelectedSms;		
		CArrayFix<TInt>* iSentItemsSelectedSms;

		CDesCArray *InboxSmsDatearray;
		CDesCArray *InboxSmsMessagearray;
		CDesCArray *InboxSmsDetailsarray;
		
		CDesCArray *SentSmsDatearray;
		CDesCArray *SentSmsMessagearray;
		CDesCArray *SentSmsDetailsarray;
		
    public: // Functions from base classes		

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

//        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

        TKeyResponse CManualSmsListContainer::OfferKeyEventL(
	    const TKeyEvent& aKeyEvent,TEventCode aType);

		void SetGraphicIconL( CEikFormattedCellListBox* aListBox );

		void GraphicIconL( CArrayPtr<CGulIcon>* aIcons );				

    private: //data
		CAknNavigationControlContainer * iNaviPane ;
		CAknNavigationDecorator * iNaviDeco ;
		CAknTabGroup * iTabGroup ;		

		CEikFormattedCellListBox* iListBox;
		CAknSearchField* iFindBox;
		CAknListBoxFilterItems* iIndex;
		//TInt iSelectedSms[500];
		TInt iCursorPos;

		TFileName smstext;

		CReadSmsAO	*iReadSmsAO;
		//CWriteSmsAO	*iWriteSmsAO;
    };

#endif

// End of File

