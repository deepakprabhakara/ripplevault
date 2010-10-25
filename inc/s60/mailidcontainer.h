/*
* ============================================================================
*  Name     : CMailIdContainer from MailIdContainer.h
*  Part of  : 
*  Created  : 6/16/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleAce
* ============================================================================
*/

#ifndef MAILIDCONTAINER_H
#define MAILIDCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <eikedwin.h>
#include <barsread.h>

// FORWARD DECLARATIONS
class CRippleVaultAppUi;

// CLASS DECLARATION

/**
*  CMailIdContainer  container control class.
*  
*/
class CMailIdContainer : public CCoeControl, MCoeControlObserver
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
        ~CMailIdContainer();

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

	 private: //data

    };

#endif

// End of File
