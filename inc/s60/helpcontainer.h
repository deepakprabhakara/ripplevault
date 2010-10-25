/*
* ============================================================================
*  Name     : CHelpContainer from HelpContainer.h
*  Part of  : Ripple
*  Created  : 6/11/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleAce
* ============================================================================
*/

#ifndef HELPCONTAINER_H
#define HELPCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <eikedwin.h>
#include <barsread.h>
   
// FORWARD DECLARATIONS
class CEikLabel;        // for example labels
class CRippleVaultAppUi;
class CEikEdwin;


// This class is the view for Helpistered status Screen
/**
*  CHelpContainer  container control class.
*  
*/
class CHelpContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
    
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
		void ConstructL();
		//TInt iCursorPos;  
		TBuf<800> text;
		TInt iHelpFlag;  
				
		CRippleVaultAppUi* iPhotosAppUi;

		void AboutRippleVault();

		/**
        * Destructor.
        */
        ~CHelpContainer();

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

        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
		
		//TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
          //                          TEventCode aType);
        
    private: //data
        
        CEikEdwin* iEdwin;		
    };

#endif // HelpCONTAINER_H

// End of File
