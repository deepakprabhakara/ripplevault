/*
* ============================================================================
*  Name     : CMainScreenListView from MainScreenListView.h
*  Part of  : 
*  Created  : 19/05/2005 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef MAINSCREENLISTVIEW_H
#define MAINSCREENLISTVIEW_H


// INCLUDES
#include "RippleVaultappui.h"
#include <aknview.h>

// CONSTANTS
// UID of view
//const TUid KView15Id = {15};

// FORWARD DECLARATIONS
class CMainScreenListContainer;

// CLASS DECLARATION
/**
*  CMainScreenListView view class.
* 
*/

class CMainScreenListView : public CAknView
    {
   public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */

        void ConstructL();

        /**
        * Destructor.
        */

        ~CMainScreenListView();
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
        CMainScreenListContainer* iContainer;
	public:
			CRippleVaultAppUi* iPhotosAppUi;			
			void DisplayFormL();
	};
#endif



// End of File

