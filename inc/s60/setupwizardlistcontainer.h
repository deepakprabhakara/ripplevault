/*

* ============================================================================

*  Name     : CSetupWizardListContainer.h

*  Part of  : 

*  Copyright (c) 2000-2005 PurpleACE. All rights reserved.

* ============================================================================

*/



#ifndef SETUPWIZARDLISTCONTAINER_H

#define SETUPWIZARDLISTCONTAINER_H



// INCLUDES

#include <coecntrl.h>

#include <eiklbo.h>

#include <AknTextSettingPage.h>

#include <akntabgrp.h>
#include <aknnavide.h>

// FORWARD DECLARATIONS

class CEikLabel;

class CSetupWizardListView;



//class CDesCArrayFlat;

class CAknQueryValueTextArray;

class CAknQueryValueText;



// CLASS DECLARATION



/**

* CSetupWizardListContainer  container control class.

*/

class CSetupWizardListContainer : public CCoeControl

    {

     

    public: // Constructors and destructor



        /**

        * Copy constructor.

        * @param aView Pointer to View class object.

        */

        CSetupWizardListContainer( CSetupWizardListView* aView );



        /**

        * EPOC constructor.

        * @param aRect Frame rectangle for container.

        */

        void ConstructL( const TRect& aRect );



        /**

        * Destructor.

        */

        virtual ~CSetupWizardListContainer();



    public: // New functions

		CRippleVaultAppUi* iPhotosAppUi;
		
        /**

        * Displays label by ID of resource.

        * @param aResourceId ID of resource for text.

        */

    void DisplayLabelL();

        

    private: // From CCoeControl



        /**

        * From CCoeControl, OfferKeyEventL.

        * Handles the key events.

        * @return If key-event is consumed, EKeyWasConsumed.

        *  Else EKeyWasNotConsumed.

        * @param aKeyEvent Key event.

        * @param aType Type of key event(EEventKey, EEventKeyUp or

        *  EEventKeyDown).

        */

        void SizeChanged();



        /**

        * From CoeControl, CountComponentControls.

        * Returns number of component.

        * @return Number of controls.

        */

        TInt CountComponentControls() const;



        /**

        * From CCoeControl, ComponentControl.

        * Returns pointer to particular component.

        * @return Pointer to container's component control,

        *  identified by index.

        * @param aIndex Index of the control.

        */

        CCoeControl* ComponentControl( TInt aIndex ) const;



        /**

        * From CCoeControl, Draw.

        * Fills the window's rectangle.

        */

        void Draw( const TRect& aRect ) const;



    private: // Data


		//CAknNavigationControlContainer * iNaviPane ;
		//CAknNavigationDecorator * iNaviDeco ;
		//CAknTabGroup * iTabGroup ;
        CSetupWizardListView* iView;

    };



#endif



// End of File

