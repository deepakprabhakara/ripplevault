/*
* ============================================================================
*  Name     : CTestFileView2 from TestFileView2.h
*  Part of  : 
*  Created  : 3/29/2004 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef TESTFILEVIEW2_H
#define TESTFILEVIEW2_H

// INCLUDES
#include <aknview.h>


// CONSTANTS
// UID of view
//const TUid KView2Id = {2};


// FORWARD DECLARATIONS
class CTestFileContainer2;

// CLASS DECLARATION

/**
*  CTestFileView2 view class.
* 
*/
class CTestFileView2 : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CTestFileView2();

    public: // Functions from base classes
        
        /**
        * From ?base_class ?member_description
        */
        TUid Id() const;

        /**
        * From ?base_class ?member_description
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From ?base_class ?member_description
        */
        void HandleClientRectChange();

    private:

        /**
        * From AknView, ?member_description
        */
        void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,
            const TDesC8& aCustomMessage);

        /**
        * From AknView, ?member_description
        */
        void DoDeactivate();

    private: // Data
        CTestFileContainer2* iContainer;
    };

#endif

// End of File
