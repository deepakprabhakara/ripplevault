/*
* ============================================================================
*  Name     : CSelectiveSmsListContainer from PhotoListContainer.h
*  Part of  : 
*  Created  : 17/06/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef SELECTIVESMSLISTCONTAINER_H
#define SELECTIVESMSLISTCONTAINER_H

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


// CLASS DECLARATION
class CRippleVaultAppUi;

/**
*  CSelectiveSmsListContainer  container control class.
*  
*/

class CSelectiveSmsListContainer : public CCoeControl, MCoeControlObserver
    {

    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */

        void ConstructL(const TRect& aRect);

		CRippleVaultAppUi* iPhotosAppUi;
		
        /**
        * Destructor.
        */

        ~CSelectiveSmsListContainer();

    public: // New functions
		
		void ListStoredContacts();		
		void SetFileList();
		TInt OkPressKey();		
		TInt ListContacts();
		void CopyContactsId(TInt pos);
		void RemoveContactsId(TInt position);
		void DeleteContacts();
		TInt GetStoredContactsCount();
		CDesCArray* items;
		
    public: // Functions from base classes
		CArrayFix<TInt>* iContactsArray;

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
		

        TKeyResponse CSelectiveSmsListContainer::OfferKeyEventL(
	    const TKeyEvent& aKeyEvent,TEventCode aType);

		void SetGraphicIconL( CEikColumnListBox* aListBox );

		void GraphicIconL( CArrayPtr<CGulIcon>* aIcons );		

    private: //data		
		CEikColumnListBox* iListBox;
    };

#endif

// End of File

