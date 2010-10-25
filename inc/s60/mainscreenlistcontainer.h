/*
* ============================================================================
*  Name     : CMainScreenListContainer from MainScreenListContainer.h
*  Part of  : 
*  Created  : 19/05/2005 by Ezhil

*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/


#ifndef MAINSCREENLISTCONTAINER_H
#define MAINSCREENLISTCONTAINER_H

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
*  CMainScreenListContainer  container control class.
*  
*/

class CMainScreenListContainer : public CCoeControl, MCoeControlObserver
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
        ~CMainScreenListContainer();
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

		CEikColumnListBox* iListBox;

		CDesCArray* items;
		
		TInt iContactCount;
		
    };
#endif



// End of File

