/*
* ============================================================================
*  Name     : CFindBoxView from FindBoxView.h
*  Part of  : 
*  Created  : 6/15/2004 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleAce
* ============================================================================
*/

#ifndef FINDBOXVIEW_H
#define FINDBOXVIEW_H

// INCLUDES
#include <aknview.h>


// CONSTANTS
// UID of view
//const TUid KView6Id = {6};

// FORWARD DECLARATIONS
class CFindBoxContainer;
class CRippleVaultAppUi;

// CLASS DECLARATION

/**
*  CFindBoxView view class.
* 
*/
class CFindBoxView : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

		CRippleVaultAppUi* iPhotosAppUi;	

        /**
        * Destructor.
        */
        ~CFindBoxView();

    public: // Functions from base classes
        
        /**
        * From ?base_class ?member_description
        */
        TUid Id() const;

        /**
        * From ?base_class ?member_description
        */
        void HandleCommandL(TInt aCommand);

		void DisplayFormL();

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
        CFindBoxContainer* iContainer;
    };

#endif

// End of File
