/*
* ============================================================================
*  Name     : CSecureContainer from SecureContainer.h
*  Part of  : Secure
*  Created  : 8/6/2005 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: Purpleace
* ============================================================================
*/

#ifndef SECURECONTAINER_H
#define SECURECONTAINER_H

// INCLUDES
#include <coecntrl.h>
   
// FORWARD DECLARATIONS
class CSecureAppUi;
class CEikLabel;        // for example labels

// CLASS DECLARATION

/**
*  CSecureContainer  container control class.
*  
*/
class CSecureContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

		CSecureAppUi &iAppUi;

		CSecureContainer(CSecureAppUi& aAppUi);

        /**
        * Destructor.
        */
        ~CSecureContainer();

    public: // New functions

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
        
        CEikLabel* iLabel;          // example label
        CEikLabel* iToDoLabel;      // example label
    };

#endif

// End of File
