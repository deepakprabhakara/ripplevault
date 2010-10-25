/*
* ============================================================================
*  Name     : CMainSubScreenListView from MainSubScreenListView.h
*  Part of  : 
*  Created  : 19/05/2005 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef MAINSUBSCREENLISTVIEW_H
#define MAINSUBSCREENLISTVIEW_H


// INCLUDES
#include "RippleVaultappui.h"
#include <aknview.h>

// CONSTANTS
// UID of view
//const TUid KView16Id = {16};

// FORWARD DECLARATIONS
class CMainSubScreenListContainer;

// CLASS DECLARATION
/**
*  CMainSubScreenListView view class.
* 
*/

class CMainSubScreenListView : public CAknView
    {
   public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */

        void ConstructL();

        /**
        * Destructor.
        */

        ~CMainSubScreenListView();
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
        CMainSubScreenListContainer* iContainer;
	public:
			CRippleVaultAppUi* iPhotosAppUi;			
    };
#endif



// End of File

