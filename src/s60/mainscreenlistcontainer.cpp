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
 Module Name				: MainScreenListContainer.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2004
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     To Show Main screen list for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// INCLUDE FILES
#include "MainScreenListContainer.h"
#include "RippleVaultAppUi.h"
#include <akntitle.h>
#include <akncontext.h>
#include "icons.mbg"
#include  <ripplevault.rsg>

#include <aknnotewrappers.h> 
#include "rippleimage.mbg"

#include <aknnavi.h>

const TInt KAknExListGraphicGulIconIndex = 10;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMainScreenListContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------

void CMainScreenListContainer::ConstructL(const TRect& aRect)
{	
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
//////////
	TBuf<10> message;
	message.Copy(_L(""));

	CAknNavigationControlContainer* aNaviPane = (CAknNavigationControlContainer *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));

	CAknNavigationDecorator* aNaviDecorator = aNaviPane->CreateMessageLabelL(message);

    aNaviPane->PushL(*aNaviDecorator);
///////////////
	CAknContextPane* cp=(CAknContextPane *)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidContext));

	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KRippleImageMbmFileName);

	CFbsBitmap* bitmap = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimage);
	CleanupStack::PushL(bitmap);
	CFbsBitmap* bitmapmask = iEikonEnv->CreateBitmapL(path, EMbmRippleimageRippleimagem);
	CleanupStack::PushL(bitmapmask);
	cp->SetPicture(bitmap, bitmapmask);
	CleanupStack::Pop(); // bitmapmask
	CleanupStack::Pop(); // bitmap
	DrawNow();


	CAknTitlePane* tp=(CAknTitlePane*)statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)); 
    tp->SetTextL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH))); // Set the text string.

	
	iContactCount = 6;

	ShowListBox();	

	if( iPhotosAppUi->iSmsValue == 0)	
		iPhotosAppUi->ImsiCheck();
	
}

// Destructor
CMainScreenListContainer::~CMainScreenListContainer()
{
	items->Reset();
	delete iListBox;
}

// ---------------------------------------------------------
// CMainScreenListContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------

void CMainScreenListContainer::SizeChanged()
    {
        // TODO: Add here control resize code etc.
	iListBox->SetExtent(TPoint(0,0),TSize(176,150));
    }

// ---------------------------------------------------------
// CMainScreenListContainer::CountComponentControls() const
// ---------------------------------------------------------

TInt CMainScreenListContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CMainScreenListContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------

CCoeControl* CMainScreenListContainer::ComponentControl(TInt aIndex) const
    {
   switch ( aIndex )
        {
        case 0:
            return iListBox;
        default:
            return NULL;
        }        
    }

// ---------------------------------------------------------
// CMainScreenListContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CMainScreenListContainer::Draw(const TRect& aRect ) const
 {
	   CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbGray);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);

	/*if( iPhotosAppUi->iSmsValue == 1)
	{
		if(iPhotosAppUi->iWaitDialog == NULL)
		{
			iPhotosAppUi->ExitApplication();
			//iPhotosAppUi->iSmsValue = 2;
			//iPhotosAppUi->ActivateView(KViewIdEmptyScreen);
			//iPhotosAppUi->ActivateView(KViewIdMainScreen);
		}
	}*/
}    

// ---------------------------------------------------------
// CMainScreenListContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CMainScreenListContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

TKeyResponse CMainScreenListContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    // See if we have a selection
    TInt code = aKeyEvent.iCode;

    switch(code)
        {
		case EKeyOK:
			{
				ShowScreen();
			}
		break;
        default:
            // Let Listbox take care of its key handling
            return iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;
        }
   		return (TKeyResponse)0;
    }

void CMainScreenListContainer::ShowListBox()
{
	CreateWindowL();

	iListBox = new (ELeave) CAknSingleLargeStyleListBox;

	iListBox->SetContainerWindowL( *this );
	iListBox->ConstructL( this, EAknListBoxSelectionList);

	// Create the scroll indicator
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	SetFileList();	
	
	SizeChanged();

	SetGraphicIconL(iListBox);

	iListBox->ActivateL();	

	TRect Please(TPoint(0,42),TPoint(176,186));
	SetRect(Please);		

	ActivateL();
}

void CMainScreenListContainer::SetGraphicIconL( CEikColumnListBox* aListBox )
{
    if ( aListBox )
    {
        // Creates gul icon.
        CArrayPtr<CGulIcon>* icons = new( ELeave ) CAknIconArray(KAknExListGraphicGulIconIndex);

        CleanupStack::PushL( icons );

        GraphicIconL( icons ); // Appends graphic data.

        // Sets graphics as ListBox icon.
        aListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );

        CleanupStack::Pop();
    }
}
void CMainScreenListContainer::GraphicIconL( CArrayPtr<CGulIcon>* aIcons )
    {
	
	TBuf<128> path;
	path = CRippleVaultAppUi::ApplicationDriveAndPath();
	path.Append(KIconsMbmFileName);

	if ( aIcons )
        {
				// Appends icon.
                //aIcons->AppendL(iEikonEnv->CreateIconL(path,EMbmIconsSecure,EMbmIconsSecurem));  
				aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmIconsStatus,EMbmIconsStatusm));
                // Appends icon.
                aIcons->AppendL(iEikonEnv->CreateIconL(path,EMbmIconsContacts,EMbmIconsContactsm));                
				// Appends icon.
                aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmIconsPhotos,EMbmIconsPhotosm));
				// Appends icon.
                aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmIconsVideos,EMbmIconsVideosm));
				// Appends icon.
                aIcons->AppendL(iEikonEnv->CreateIconL(path,EMbmIconsSms,EMbmIconsSmsm));                
				// Appends icon.
                //aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmIconsCsp,EMbmIconsCspm));
				// Appends icon.
                //aIcons->AppendL(iEikonEnv->CreateIconL( path,EMbmIconsStatus,EMbmIconsStatusm));
				aIcons->AppendL(iEikonEnv->CreateIconL(path,EMbmIconsSecure,EMbmIconsSecurem));  
        }
    }

void CMainScreenListContainer::SetFileList()
 {
	_LIT(KStringFolder,"%d\t");
	_LIT(KStringNil,"123");

	TBuf<50> bufb(KStringNil);
	TBuf<50> bufc;
	TFileName buf1;

	// Set the listbox to use the the file list model
	items = static_cast<CDesCArray*>(iListBox->Model()->ItemTextArray());
	
    iListBox->HandleItemRemovalL();
	
	for (TInt i=0; i < iContactCount; i++)
	{
		bufc.Copy(KStringFolder);
		
		if(i == 0)
			//bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_SECURE)));
			bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_STATUS)));
		else if(i == 1)
			bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_CONTACTS)));
		else if(i == 2)
			bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_PHOTOS)));
		else if(i == 3)
			bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_VIDEOS)));
		else if(i == 4)
			bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_SMS)));
		//else if(i == 4)
			//bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_CHANGEPLAN)));
		else if(i == 5)
			//bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_STATUS)));
			bufc.Append(*(CEikonEnv::Static()->AllocReadResourceL(R_LIST_MAINMENU_SECURE)));
	
		buf1.Format(bufc,i,&bufb);

		items->AppendL(buf1);	
	}
 }

void CMainScreenListContainer::ShowScreen()
{
	if(iListBox->CurrentItemIndex() == 0)//status
	{
		iPhotosAppUi->ActivateView(KViewIdStatusScreen);
	}
/*	else if(iListBox->CurrentItemIndex() == 4)//csp
	{
		//change subscription plan
		CAknInformationNote* note = new(ELeave) CAknInformationNote;
		TBuf<40> text;
		text.Copy(*(CEikonEnv::Static()->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
		note->ExecuteLD(text);
	}*/
	else if(iListBox->CurrentItemIndex() == 4)//sms
	{
		iPhotosAppUi->iMainScreenValue = 14;		
		iPhotosAppUi->ActivateView(KViewIdSubScreen);
	}
	else if(iListBox->CurrentItemIndex() == 3)//videos
	{
		iPhotosAppUi->iMainScreenValue = 13;		
		iPhotosAppUi->ActivateView(KViewIdSubScreen);
	}
	else if(iListBox->CurrentItemIndex() == 2)//photos
	{
		iPhotosAppUi->iMainScreenValue = 12;
		iPhotosAppUi->ActivateView(KViewIdSubScreen);
	}
	else if(iListBox->CurrentItemIndex() == 1)//contacts
	{
		iPhotosAppUi->iMainScreenValue = 11;		
		iPhotosAppUi->ActivateView(KViewIdSubScreen);
	}
	else if(iListBox->CurrentItemIndex() == 5)//secure
	{
		//iPhotosAppUi->iSettingsScreenValue = 4;
		//iPhotosAppUi->ActivateView(1);
		iPhotosAppUi->iMainScreenValue = 15;
		iPhotosAppUi->ActivateView(KViewIdSubScreen);
	}
}
// End of File  
