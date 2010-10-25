/*
* ============================================================================
*  Name     : AknExSettingListListBox.h
*  Part of  : 
*  Copyright (c) 2000-2005 PurpleACE. All rights reserved.
* ============================================================================
*/

#ifndef AKNEXSETTINGLISTLISTBOX_H
#define AKNEXSETTINGLISTLISTBOX_H

// INCLUDES
#include <AknSettingItemList.h>

// FORWARD DECLARATIONS
class CAknExSettingListItemData;
class CAknExSettingListView;
class CRippleVaultAppUi;

// CLASS DECLARATION

/**
* CAknExSettingListContainer  container control class.
*/
class CAknExSettingListListbox : public CAknSettingItemList 
    {
    public:
        CAknSettingItem* CreateSettingItemL( TInt identifier );
        void SetData(CAknExSettingListItemData* aData);
        void SetView(CAknExSettingListView* aView);
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
        CAknExSettingListItemData* iData;
        CAknExSettingListView* iView;
		CAknSettingItem* settingItem;
    };


#endif

// End of File
