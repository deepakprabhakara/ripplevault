/*******************************************************************

 Copyright(c) 2000-2005 

 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.

 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..

 Project Name               : Ripple Vault Symbian Client
 Module Name				: AknExSettingListContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-03-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  
				Settings screen for S60 - Contacts, Publish, SMS and General Settings

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

// INCLUDE FILES
#include <barsread.h>
#include <eiklabel.h>
#include <avkon.hrh>
#include <aknlists.h>
#include <aknquerydialog.h>
#include <akncontext.h>
#include <akntitle.h>
#include <RippleVault.rsg>

#include "RippleVault.hrh"
#include "RippleVaultAppUi.h"
#include "aknexsettinglistview.h"
#include "aknexsettinglistcontainer.h"
#include "Settings.mbg"

#include <aknnavi.h>

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CAknExSettingListContainer::CAknExSettingListContainer( CAknExSettingListView* )
// Copy constructor.
// ----------------------------------------------------------------------------

CAknExSettingListContainer::CAknExSettingListContainer( CAknExSettingListView* aView ) :
   iView( aView )    
    {
    }

// ----------------------------------------------------------------------------
// void CAknExSettingListContainer::ConstructL( const TRect& )
// EPOC constructor.
// ----------------------------------------------------------------------------
//
void CAknExSettingListContainer::ConstructL( const TRect& aRect )
{
    CreateWindowL(); // Creates window.
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	
	TBuf<10> message;
	message.Copy(_L(""));

	CAknNavigationControlContainer* aNaviPane = (CAknNavigationControlContainer *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));

	CAknNavigationDecorator* aNaviDecorator = aNaviPane->CreateMessageLabelL(message);

    aNaviPane->PushL(*aNaviDecorator);

	//CEikStatusPane * statusPane = iAvkonAppUi->StatusPane();     
/*	iNaviPane = static_cast<CAknNavigationControlContainer*>(statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi))); 

	iNaviDeco = iNaviPane->CreateTabGroupL(); 
	iNaviPane->PushL(static_cast<CAknNavigationDecorator &>(*iNaviDeco)); 
	//iNaviPane->PushL(static_cast<CAknNavigationDecorator &>(*iNaviDeco)); 

	iTabGroup = static_cast<CAknTabGroup*> (iNaviDeco->DecoratedControl()); 
	
	TBuf<20> buf;
	
	buf.Copy(*(iEikonEnv->AllocReadResourceL(R_TAB_GENERAL)));
	iTabGroup->AddTabL(  0, buf ); 

	buf.Copy(*(iEikonEnv->AllocReadResourceL(R_TAB_PHOTOS)));
	iTabGroup->AddTabL(  1, buf ); 

	buf.Copy(*(iEikonEnv->AllocReadResourceL(R_TAB_CONTACTS)));
	iTabGroup->AddTabL(  2, buf ); 
	
	buf.Copy(*(iEikonEnv->AllocReadResourceL(R_TAB_CONTACTS)));
	iTabGroup->AddTabL(  3, _L("Sms") ); 

	iTabGroup->SetActiveTabById(0); 
*/
	CAknContextPane* cp=(CAknContextPane *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidContext));

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KSettingsMbmFileName);

	CFbsBitmap* bitmap = iEikonEnv->CreateBitmapL(path, EMbmSettingsSettings);
	CleanupStack::PushL(bitmap);
	CFbsBitmap* bitmapmask = iEikonEnv->CreateBitmapL(path, EMbmSettingsSettingsm);
	CleanupStack::PushL(bitmapmask);

	cp->SetPicture(bitmap, bitmapmask);

	CleanupStack::Pop(); // bitmapmask
	CleanupStack::Pop(); // bitmap
	
	DrawNow();
	
	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 

	switch(iPhotosAppUi->iSettingsScreenValue){
		case 0:	
			tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_SETTINGS))); // Set the text string.
			break;
		case 1:	
			tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_CONTACTS_SETTINGS))); // Set the text string.
			break;
		case 2:	
			tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_PHOTOS_SETTINGS))); // Set the text string.
			break;
		case 3:	
			tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_MESSAGES_SETTINGS))); // Set the text string.
			break;
		case 4:	
			tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_SECURE_SETTINGS))); // Set the text string.
			break;
		default:
			break;
	}

    SetRect( aRect ); // Sets rectangle of frame.
    ActivateL(); // Activates the window. (Ready to draw)
}

// ----------------------------------------------------------------------------
// CAknExSettingListContainer::~CAknExSettingListContainer()
// Destructor.
// ----------------------------------------------------------------------------
//
CAknExSettingListContainer::~CAknExSettingListContainer()
    {
		//delete iNaviDeco ;	
    }

// ----------------------------------------------------------------------------
// void CAknExSettingListContainer::DisplayLabelL( const TInt )
// Displays the label by ID of resource.
// ----------------------------------------------------------------------------
//

void CAknExSettingListContainer::DisplayLabelL()
{
   SizeChanged();
   DrawNow();
}

// void CAknExSettingListContainer::SizeChanged()
// Called by framework when the view size is changed.
// ----------------------------------------------------------------------------
//
void CAknExSettingListContainer::SizeChanged()
{
}

// ----------------------------------------------------------------------------
// TInt CAknExSettingListContainer::CountComponentControls() const
// Returns number of component.
// ----------------------------------------------------------------------------
//
TInt CAknExSettingListContainer::CountComponentControls() const
{
    return 0; // Returns number of component.
}

// ----------------------------------------------------------------------------
// CCoeControl* CAknExSettingListContainer::ComponentControl( TInt ) const
// Returns pointer to particular component.
// ----------------------------------------------------------------------------
//

CCoeControl* CAknExSettingListContainer::ComponentControl( TInt aIndex ) const
{
	aIndex=0;
    return NULL; // Exception: Returns NULL value.
}

// ----------------------------------------------------------------------------
// void CAknExSettingListContainer::Draw( const TRect& ) const
// Fills the window's rectangle.
// ----------------------------------------------------------------------------
//

void CAknExSettingListContainer::Draw( const TRect& aRect ) const
{
    // (Re)draw the rectangle of frame.
    CWindowGc& gc = SystemGc();
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbGray );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
	gc.DrawRect( aRect );
}


// End of File

