/*
* =============================================================================
*  Name     : CAknExSettingListView
*  Part of  : 
*  Copyright (c) 2000-2005 PurpleACE. All rights reserved.
* ============================================================================
*/



#ifndef AKNEXSETTINGLISTVIEW_H
#define AKNEXSETTINGLISTVIEW_H

// INCLUDES
#include <aknview.h>
#include <d32dbms.h>
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <badesca.h>

class CRippleVaultAppUi;

// CONSTANTS
//const TUid KViewId = { 1 }; // UID of View.

// FORWARD DECLARATIONS
class CAknExSettingListContainer;
class CAknExSettingListListbox;
class CAknExSettingListItemData;


// CLASS DECLARATION

/**
* CAknExSettingListView view class.
*/
class CAknExSettingListView : public CAknView
    {
    public: // Constructors and destructor
        CAknExSettingListView();
        /**
        * Destructor.
        */
        virtual ~CAknExSettingListView();

    public: // New function
    	
    public: // From CAknView

        /**
        * From CAknView, Id.
        * Returns ID of View.
        * @return ID of View.
        */
        TUid Id() const;

        /**
        * From CAknView, HandleCommandL.
        * Handles the commands. If the command is command which is required to
        * display outline-screen, the command is reported to container class.
        * @param aCommand Command to be handled.
        */
        void HandleCommandL( TInt aCommand );

        /**
        * Returns current outline id
        * @return current outline id
        */

        CRippleVaultAppUi* iPhotosAppUi;

		void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

    private: // New functions
        
        void CreateListBoxL(TInt aResourceId);

    private: // From CAknView

        /**
        * From CAknView, DoActivateL.
        * Creates the Container class object.
        * @param aPrevViewId aPrevViewId is not used.
        * @param aCustomMessageId aCustomMessageId is not used.
        * @param aCustomMessage aCustomMessage is not used.
        */
        void DoActivateL(
            const TVwsViewId& aPrevViewId,
            TUid aCustomMessageId,
            const TDesC8& aCustomMessage );

        /**
        * From CAknView, DoDeactivate.
        * Deletes the Container class object.
        */
        void DoDeactivate();

    private: // Data
        CAknExSettingListContainer* iContainer;
        CAknExSettingListListbox* iListBox;
        CAknExSettingListItemData* iData;
        friend class CAknExSettingListListbox;

    };

#endif

// End of File
