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
 Module Name				: MyListBoxAppView.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     To Show Main screen list for UIQ
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    : 


***************************************************************/

// --------------------------------------------------------------------------
// MyListBoxAppView.cpp
//
//
// Main screen list for UIQ.
// --------------------------------------------------------------------------

// INCLUDE FILES
#include <cknenv.h>
#include <eikenv.h>
#include <eikappui.h>
#include <eiktxlbx.h>   // CEikTextListBox
#include <eiktxlbm.h>   // CTestListBoxModel
#include <badesca.h>    // for array of descriptors
#include "icons.mbg"    // for the icon
#include "subicons.mbg"    // for the icon
#include <eikmenub.h>
#include <gulicon.h>
#include <ripplevault.rsg>

#include "MyListBoxAppView.h"
#include "CustomListBox.h"
#include "RippleVaultAppUi.h"
#include "RippleVaultUtils.h"

/*
// MEMBER FUNCTIONS
void CMyListBoxAppView::ConstructL(const TRect& aRect)
	{
	iIsDoubleLine = 0;
	//iIsDoubleLine = EFalse;
	CreateWindowL();
	CreateListBoxL();
	SetRect(aRect);
	ActivateL();
	}

CMyListBoxAppView::~CMyListBoxAppView()
	{
	delete iListBox;
	}
*/

CMyListBoxAppView::CMyListBoxAppView()
{
}

//Static NewL() function to start the standard two
//phase construction.
CMyListBoxAppView* CMyListBoxAppView::NewL(const TRect& aRect)
{
	CMyListBoxAppView* self = new(ELeave) CMyListBoxAppView();
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
}

//Destructor for the view.
CMyListBoxAppView::~CMyListBoxAppView()
{	
	delete iListBox;	             
}

void CMyListBoxAppView::ConstructL(const TRect& aRect)
{
	iIsDoubleLine = 0;
	
	CreateWindowL();

	CreateListBoxL();

	iTitle = CCknAppTitle::NewL(CCknAppTitle::EList);
    iTitle->SetContainerWindowL(*this);
    HBufC* text = iCoeEnv->AllocReadResourceLC(R_APP_TITLE);
    iTitle->SetTextL(*text, CCknAppTitle::EMainTitle);
    CleanupStack::PopAndDestroy(text);
	TRect titleRect(1, 1, aRect.Width()-KShadowMargin, iTitle->MinimumSize().iHeight);
	iTitle->SetRect(titleRect);
	
	// Indicate that the control is blank
    SetBlank();

    // Set the control's border
    //SetBorder(TGulBorder::EFlatContainer);

    // Set the correct application view (Note: ESkinAppViewWithCbaNoToolband is the default)
    CknEnv::Skin().SetAppViewType(ESkinAppViewWithCbaNoToolband);


	SetRect(aRect);
	ActivateL();
}

void CMyListBoxAppView::UpdateListBoxL(TInt aIsDoubleLine)
	{
	TBuf<50> buffer;

	iIsDoubleLine = aIsDoubleLine;

	iListBox->HandleItemRemovalL(); 

	SetIconsArray();

	//iIsDoubleLine = EFalse;
	CDesCArrayFlat* array = new (ELeave) CDesCArrayFlat(6);

	CleanupStack::PushL(array);
	if (iIsDoubleLine == 0)
		{
		buffer.Copy(_L("0\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_SECURE)));
		array->AppendL(buffer);

		buffer.Copy(_L("1\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_CONTACTS)));
		array->AppendL(buffer);

		buffer.Copy(_L("2\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_PHOTOS)));
		array->AppendL(buffer);

		buffer.Copy(_L("3\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_VIDEOS)));
		array->AppendL(buffer);

		buffer.Copy(_L("4\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_SMS)));
		array->AppendL(buffer);

		//buffer.Copy(_L("4\t"));
		//buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_CHANGEPLAN)));
		//array->AppendL(buffer);

		buffer.Copy(_L("5\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_STATUS)));
		array->AppendL(buffer);
		}
	else if (iIsDoubleLine == 1)
		{
		buffer.Copy(_L("0\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_VIEW)));
		array->AppendL(buffer);

		buffer.Copy(_L("1\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_SENDTOVAULT)));
		array->AppendL(buffer);

		buffer.Copy(_L("2\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_CONTACTS_SETTINGS)));
		array->AppendL(buffer);

		}
	else if (iIsDoubleLine == 2)
		{		
		buffer.Copy(_L("0\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_VIEW)));
		array->AppendL(buffer);

		buffer.Copy(_L("1\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)));
		array->AppendL(buffer);

		//buffer.Copy(_L("2\t"));
		//buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_GETFROMVAULT)));
		//array->AppendL(buffer);

		buffer.Copy(_L("2\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SHARE)));
		array->AppendL(buffer);

		//buffer.Copy(_L("4\t"));
		//buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
		//array->AppendL(buffer);

		buffer.Copy(_L("3\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SETTINGS)));
		array->AppendL(buffer);
		}
	else if (iIsDoubleLine == 3)
		{		
		buffer.Copy(_L("0\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_VIEW)));
		array->AppendL(buffer);

		buffer.Copy(_L("1\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SENDTOVAULT)));
		array->AppendL(buffer);

		//buffer.Copy(_L("2\t"));
		//buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_GETFROMVAULT)));
		//array->AppendL(buffer);

		buffer.Copy(_L("2\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SHARE)));
		array->AppendL(buffer);

		//buffer.Copy(_L("4\t"));
		//buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
		//array->AppendL(buffer);

		buffer.Copy(_L("3\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_SETTINGS)));
		array->AppendL(buffer);
		}
	else if (iIsDoubleLine == 4)
		{		
		buffer.Copy(_L("0\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_SMS_VIEW)));
		array->AppendL(buffer);

		buffer.Copy(_L("1\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_SMS_SENDTOVAULT)));
		array->AppendL(buffer);

		//buffer.Copy(_L("2\t"));
		//buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_SMS_GETFROMVAULT)));
		//array->AppendL(buffer);

		buffer.Copy(_L("2\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_SMS_SETTINGS)));
		array->AppendL(buffer);
		}

	CleanupStack::Pop();

	iListBox->Model()->SetItemTextArray(array);

	// We need to set the current item index and call DrawNow()
	// so that when the user changes the list box to single/double,
	// it will be updated properly.
	iListBox->HandleItemAdditionL(); 
	iListBox->SetCurrentItemIndex(0);
	iListBox->DrawNow();	
	//iListBox->SetCurrentItemIndexAndDraw(0);
	//iListBox->UpdateCurrentItem(0); 
	}

void CMyListBoxAppView::SizeChanged()
	{
	if (iListBox)
		{
		iListBox->SetRect(Rect());
		}
	}

TInt CMyListBoxAppView::CountComponentControls() const
	{
	if (iListBox)
		{
		return 1;
		}
	return 0;
	}

CCoeControl* CMyListBoxAppView::ComponentControl(TInt aIndex) const
	{
	switch (aIndex)
		{
		case 0:
			return iListBox;
		default:
			return NULL;
		}
	}

void CMyListBoxAppView::Draw(const TRect& aRect) const
	{
	CWindowGc& gc = SystemGc();
	gc.Clear();
	//gc.SetPenStyle(CGraphicsContext::ENullPen);
	//gc.SetBrushColor(KRgbWhite);
	//gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	//gc.DrawRect(aRect);

	if (IsFocused())
		{
		RippleVaultUtils::DrawShadowL( gc, Rect() );
		}
	}


void CMyListBoxAppView::SetFocus(TBool aFocus, TDrawNow aDrawNow)
{
	if (iTitle) 
		{
		iTitle->SetFocus( aFocus, aDrawNow );
		}
	CCoeControl::SetFocus(aFocus, aDrawNow);
}

TKeyResponse CMyListBoxAppView::OfferKeyEventL(const TKeyEvent& aKeyEvent,
											   TEventCode aType)
	{	
	if (iListBox)
		{
			if (EEventKeyDown == aType)
			{
			HandleListBoxEventL(
				iListBox, MEikListBoxObserver::EEventEnterKeyPressed);
			}
		}
	return EKeyWasNotConsumed;
	}

void CMyListBoxAppView::HandleListBoxEventL(CEikListBox* aListBox,
											TListBoxEvent aEventType)
	{
	if ((MEikListBoxObserver::EEventEnterKeyPressed == aEventType)
 	|| (MEikListBoxObserver::EEventItemClicked == aEventType))
        {
//			TBuf<30> buffer;
//			buffer.Format(KMessage, aListBox->CurrentItemIndex());
			TInt index;

			//if(iAppUi->iMainScreenValue == 0)//Main Screen///
			if(1)
			{
				if(aListBox->CurrentItemIndex() == 0)//Secure
				{				
					//iAppUi->SecureDialog();
				}
				if(aListBox->CurrentItemIndex() == 1)//Contacts
				{
					//iAppUi->iMainScreenValue = 1;
					//iAppUi->ShowToolBar();			
				}
				else if(aListBox->CurrentItemIndex() == 2)//Photos
				{
					//iAppUi->iMainScreenValue = 2;
					//iAppUi->ShowToolBar();			
				}
				else if(aListBox->CurrentItemIndex() == 3)//Videos
				{
					//iAppUi->iMainScreenValue = 3;
					//iAppUi->ShowToolBar();			
				}
				else if(aListBox->CurrentItemIndex() == 4)//Sms
				{
					//iAppUi->iMainScreenValue = 4;
					//iAppUi->ShowToolBar();			
				}
				/*else if(aListBox->CurrentItemIndex() == 4)//Change Sub Paln
				{	
					//iAppUi->iMainScreenValue = 5;
					TBuf<100> buffer;
					buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
					CEikonEnv::Static ()->AlertWin(buffer);
				}*/
				else if(aListBox->CurrentItemIndex() == 5)//Status
				{
					//iAppUi->iMainScreenValue = 6;
					//iAppUi->ShowStatusScreen();
				}
			}/*
			else if(iAppUi->iMainScreenValue == 1)//Contacts
			{
				if(aListBox->CurrentItemIndex() == 0)//View
				{
					index = aListBox->CurrentItemIndex();
					TBuf<100> buffer;
					buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_CONTACTS)));
					CEikonEnv::Static ()->AlertWin(buffer);
				}
				else if(aListBox->CurrentItemIndex() == 1)//synchronize
				{					
					index = aListBox->CurrentItemIndex();
					iAppUi->ShowSettingsScreen(index);				
				}
				else if(aListBox->CurrentItemIndex() == 2)//settings
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Contacts Settings"));
					iAppUi->ShowSettingsScreen(index);
				}
			}
			else if(iAppUi->iMainScreenValue == 2)//Photos
			{
				if(aListBox->CurrentItemIndex() == 0)//View Gallery
				{
					iAppUi->OpenBrowser();
				}
				else if(aListBox->CurrentItemIndex() == 1)//Send to Vault
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Send to Vault"));					
					iAppUi->ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 2)//Share Album
				{	
					index = aListBox->CurrentItemIndex();
					index = 3;
					//CEikonEnv::Static ()->AlertWin(_L("Share Album"));
					iAppUi->ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 3)//settings
				{					
					index = aListBox->CurrentItemIndex();
					index = 5;
					//CEikonEnv::Static ()->AlertWin(_L("Photos Settings"));
					iAppUi->ShowSettingsScreen(index);
				}
			}
			else if(iAppUi->iMainScreenValue == 3)//Videos
			{
				if(aListBox->CurrentItemIndex() == 0)//View Gallery
				{
					iAppUi->OpenBrowser();
				}
				else if(aListBox->CurrentItemIndex() == 1)//Send to Vault
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Send to Vault"));
					iAppUi->ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 3)//settings
				{					
					index = aListBox->CurrentItemIndex();
					index = 5;
					//CEikonEnv::Static ()->AlertWin(_L("Videos Settings"));
					iAppUi->ShowSettingsScreen(index);
				}
			}
			else if(iAppUi->iMainScreenValue == 4)//Sms
			{
				if(aListBox->CurrentItemIndex() == 0)//View Gallery
				{
					index = aListBox->CurrentItemIndex();
					TBuf<100> buffer;
					buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_MESSAGES)));
					CEikonEnv::Static ()->AlertWin(buffer);
				}
				else if(aListBox->CurrentItemIndex() == 1)//Send to Vault
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Send to Vault"));
					iAppUi->ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 2)//settings
				{					
					index = aListBox->CurrentItemIndex();
					index = 3;
					//CEikonEnv::Static ()->AlertWin(_L("Sms Settings"));
					iAppUi->ShowSettingsScreen(index);
				}
			}*/
			//iEikonEnv->InfoMsg(buffer);
		}
	}

void CMyListBoxAppView::CreateListBoxL()
	{
	// Creates a new custom list box.
	iListBox = new (ELeave) CCustomListBox();
	iListBox->ConstructL(this,CEikListBox::ENoFirstLetterMatching| CEikListBox::ENoExtendedSelection);
	iListBox->SetListBoxObserver(this);
	
	// Fills the list box with some items.
	UpdateListBoxL(iIsDoubleLine);	
	}

void CMyListBoxAppView::SetIconsArray()
{
	// Creates a new icon array.
	CArrayPtr<CGulIcon>* iconArray = new (ELeave)
		CArrayPtrFlat<CGulIcon>(6);
	CleanupStack::PushL(iconArray);

	if( iIsDoubleLine == 0)//Main Screen
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL(path,
			EMbmIconsSecure, EMbmIconsSecurem));
		iconArray->AppendL(iEikonEnv->CreateIconL(path,
			EMbmIconsContacts, EMbmIconsContactsm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmIconsPhotos, EMbmIconsPhotosm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmIconsVideos, EMbmIconsVideosm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmIconsSms, EMbmIconsSmsm));
		//iconArray->AppendL(iEikonEnv->CreateIconL(path, 
		//	EMbmIconsCsp, EMbmIconsCspm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmIconsStatus, EMbmIconsStatusm));
	}
	else if( iIsDoubleLine == 1)//Contacts Screen
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL(path,
			EMbmSubiconsContactsview, EMbmSubiconsContactsviewm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmSubiconsPhotossendtovault, EMbmSubiconsPhotossendtovaultm));
		iconArray->AppendL(iEikonEnv->CreateIconL(path, 
			EMbmSubiconsListsettings, EMbmSubiconsListsettingsm));		
	}
	else if( iIsDoubleLine == 2)//Photos Screen
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotosviewgallery,EMbmSubiconsPhotosviewgallerym));                
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotossendtovault,EMbmSubiconsPhotossendtovaultm));
		// Appends icon.
		//iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotosgetfromvault,EMbmSubiconsPhotosgetfromvaultm));
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotossharealbum,EMbmSubiconsPhotossharealbumm));                
		// Appends icon.
		//iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotosunsharealbums,EMbmSubiconsPhotosunsharealbumsm));
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm));
	}
	else if( iIsDoubleLine == 3)//Videos Screen
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideosviewgallery,EMbmSubiconsVideosviewgallerym));                
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideossendtovault,EMbmSubiconsVideossendtovaultm));
		// Appends icon.
		//iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideosgetfromvault,EMbmSubiconsVideosgetfromvaultm));
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideossharealbum,EMbmSubiconsVideossharealbumm));                
		// Appends icon.
		//iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideosunsharealbum,EMbmSubiconsVideosunsharealbumm));
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm));
	}
	else if( iIsDoubleLine == 4)//Sms Screen
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsSmsview,EMbmSubiconsSmsviewm));                
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsSmssendtovault,EMbmSubiconsSmssendtovaultm));
		// Appends icon.
		//iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsSmsgetfromvault,EMbmSubiconsSmsgetfromvaultm));
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm)); 
	}

	CleanupStack::Pop(); // iconArray

	// Sets the icon array.
	CCustomListItemDrawer* itemDrawer = static_cast<CCustomListItemDrawer*>
		(iListBox->View()->ItemDrawer());
	itemDrawer->SetIconArray(iconArray); // transfer ownership

	// Sets the height of the list box item
	// (= 2 * maximum height of the icon).
	TSize size = itemDrawer->MaxIconSize();
	iListBox->SetItemHeightL(1.5 * size.iHeight);
}

TVwsViewId CMyListBoxAppView::ViewId() const
{
	TUid uid = { 5 };
	TUid app = { 0x121F6165 };
	return TVwsViewId(app, uid);
}

void CMyListBoxAppView::ViewActivatedL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage)    
{
	/*
    CEikMenuBar* menuBar = iEikonEnv->AppUiFactory()->MenuBar();
    //menuBar->ChangeMenuBarL(0, R_EXAMPLE_MENUBAR, EFalse);
	if(iAppUi->iSubscriptionState == 0)
		menuBar->ChangeMenuBarL(0, R_EXAMPLE_MENUBAR, ETrue);
	else
		menuBar->ChangeMenuBarL(0, R_EXAMPLE_MENUBAR1, ETrue);
	*/
    Window().SetOrdinalPosition(0);
}

void CMyListBoxAppView::ViewDeactivated()
{
    MakeVisible(EFalse);
}
void CMyListBoxAppView::SetAppIns(CRippleVaultAppUi *aAppUi)
{
	iAppUi = aAppUi;
}
// End of File  
