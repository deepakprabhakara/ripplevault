/*
* ============================================================================
*  Name     : CManualSmsListView from PhotoListView.h
*  Part of  : 
*  Created  : 17/06/2004 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/


#ifndef MANUALSMSLISTVIEW_H
#define MANUALSMSLISTVIEW_H


// INCLUDES
#include "RippleVaultappui.h"
#include <aknview.h>


// CONSTANTS
// UID of view

//const TUid KView12Id = {12};


// FORWARD DECLARATIONS
class CManualSmsListContainer;

// CLASS DECLARATION

/**
*  CManualSmsListView view class.
* 
*/
class CManualSmsListView : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();
        
		/**
        * Destructor.
        */
        ~CManualSmsListView();

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
		
		void SendPhotoToServer();

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
        CManualSmsListContainer* iContainer;
	public:
		CRippleVaultAppUi* iPhotosAppUi;
    };
#endif

// End of File

