/*
* ============================================================================
*  Name     : CMainSubScreenListContainer from MainSubScreenListContainer.h
*  Part of  : 
*  Created  : 19/05/2005 by Ezhil

*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/


#ifndef MAINSUBSCREENLISTCONTAINER_H
#define MAINSUBSCREENLISTCONTAINER_H

// INCLUDES
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <coecntrl.h>
#include <aknlists.h>
#include <akniconarray.h> 
#include <eikclbd.h>

// CLASS DECLARATION

class CRippleVaultAppUi;

/**
*  CMainSubScreenListContainer  container control class.
*  
*/

class CMainSubScreenListContainer : public CCoeControl, MCoeControlObserver
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
        ~CMainSubScreenListContainer();
    public: // New functions
		void SetFileList();
		void ShowListBox();		
		void ShowScreen();
		
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

        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,TEventCode aType);

		void SetGraphicIconL( CEikColumnListBox* aListBox );

		void GraphicIconL( CArrayPtr<CGulIcon>* aIcons );		


    private: //data

		//Anupam : Need to review to remove some private member.

	CEikColumnListBox* iListBox;

		CDesCArray* items;
		
		TInt iContactCount;
    };
#endif



// End of File

