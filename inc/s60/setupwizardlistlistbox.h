/*
* ============================================================================
*  Name     : SetupWizardListListBox.h
*  Part of  : 
*  Copyright (c) 2000-2005 PurpleACE. All rights reserved.
* ============================================================================
*/

#ifndef SETUPWIZARDLISTLISTBOX_H
#define SETUPWIZARDLISTLISTBOX_H

// INCLUDES
#include <AknSettingItemList.h>

// FORWARD DECLARATIONS
class CSetupWizardListItemData;
class CSetupWizardListView;
class CRippleVaultAppUi;

// CLASS DECLARATION

/**
* CSetupWizardListContainer  container control class.
*/
class CSetupWizardListListbox : public CAknSettingItemList 
    {
    public:
        CAknSettingItem* CreateSettingItemL( TInt identifier );
        void SetData(CSetupWizardListItemData* aData);
        void SetView(CSetupWizardListView* aView);
		TInt PopUp();
//		void SelectAccess();
//		TInt SelectFolder();
        void SaveSettingsToDB();
        CRippleVaultAppUi* iPhotosAppUi;
		TInt SelectFolder();
        
    private:        
		TKeyResponse OfferKeyEventL(
            const TKeyEvent& aKeyEvent, TEventCode aType );
        void SizeChanged();


    private:
        CSetupWizardListItemData* iData;
        CSetupWizardListView* iView;
		CAknSettingItem* settingItem;
    };


#endif

// End of File
