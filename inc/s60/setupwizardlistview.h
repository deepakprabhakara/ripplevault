/*
* =============================================================================
*  Name     : CSetupWizardListView
*  Part of  : 
*  Copyright (c) 2000-2005 PurpleACE. All rights reserved.
* ============================================================================
*/



#ifndef SETUPWIZARDLISTVIEW_H
#define SETUPWIZARDLISTVIEW_H

// INCLUDES
#include <aknview.h>
#include <d32dbms.h>
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <badesca.h>

class CRippleVaultAppUi;

// CONSTANTS
//const TUid KView17Id = { 17 }; // UID of View.

// FORWARD DECLARATIONS
class CSetupWizardListContainer;
class CSetupWizardListListbox;
class CSetupWizardListItemData;


// CLASS DECLARATION

/**
* CSetupWizardListView view class.
*/
class CSetupWizardListView : public CAknView
    {
    public: // Constructors and destructor
        CSetupWizardListView();
        /**
        * Destructor.
        */
        virtual ~CSetupWizardListView();

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
        CSetupWizardListContainer* iContainer;
        CSetupWizardListListbox* iListBox;
        CSetupWizardListItemData* iData;
        friend class CSetupWizardListListbox;

    };

#endif

// End of File
