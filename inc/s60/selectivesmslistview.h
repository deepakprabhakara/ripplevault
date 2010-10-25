/*
* ============================================================================
*  Name     : CSelectiveSmsListView from PhotoListView.h
*  Part of  : 
*  Created  : 17/06/2004 by Ezhil
*  Description:
*     Declares view for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/


#ifndef SELECTIVESMSLISTVIEW_H
#define SELECTIVESMSLISTVIEW_H


// INCLUDES
#include "RippleVaultappui.h"
#include <aknview.h>


// CONSTANTS
// UID of view

//const TUid KView14Id = {14};


// FORWARD DECLARATIONS
class CSelectiveSmsListContainer;

// CLASS DECLARATION

/**
*  CSelectiveSmsListView view class.
* 
*/
class CSelectiveSmsListView : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();
        
		/**
        * Destructor.
        */
        ~CSelectiveSmsListView();

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
        CSelectiveSmsListContainer* iContainer;
	public:
		CRippleVaultAppUi* iPhotosAppUi;
    };
#endif

// End of File

