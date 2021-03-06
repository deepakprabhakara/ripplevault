/*
* ============================================================================
*  Name     : CTestFileContainer2 from TestFileContainer2.h
*  Part of  : 
*  Created  : 3/29/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef TESTFILECONTAINER2_H
#define TESTFILECONTAINER2_H

// INCLUDES
#include <coecntrl.h>
 
// FORWARD DECLARATIONS
class CEikLabel;        // for example labels

// CLASS DECLARATION

/**
*  CTestFileContainer2  container control class.
*  
*/
class CTestFileContainer2 : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        ~CTestFileContainer2();

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
