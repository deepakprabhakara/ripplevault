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
 Module Name				: MainScreenView_Uiq.cpp
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
// MainScreenView.cpp
//
//
// Main screen list for UIQ.
// --------------------------------------------------------------------------

// INCLUDE FILES
#include <eikenv.h>
#include <eikappui.h>
#include <eiktxlbx.h>   // CEikTextListBox
#include <eiktxlbm.h>   // CTestListBoxModel
#include <badesca.h>    // for array of descriptors
#include "icons.mbg"    // for the icon
#include "subicons.mbg"    // for the icon
#include <QuartzKeys.h> // keys for jog dial
#include <eikmenub.h>
#include <gulicon.h>
#include <ripplevault_uiq.rsg>

#include "MainScreenView_Uiq.h"
#include "MainScreenListBox.h"
#include "RippleVaultAppUi.h"

//Static NewL() function to start the standard two phase construction.
CMainScreenView* CMainScreenView::NewL(const TRect& aRect, CRippleVaultAppUi& aAppUi)
{
	CMainScreenView* self = new(ELeave) CMainScreenView(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
}

CMainScreenView::CMainScreenView(CRippleVaultAppUi& aAppUi)
: iAppUi(aAppUi)
{
}


//Destructor for the view.
CMainScreenView::~CMainScreenView()
{	
	delete iListBox;	             
}

void CMainScreenView::ConstructL(const TRect& aRect)
{
	iIsDoubleLine = 0;
	CreateWindowL();
	CreateListBoxL();
	SetRect(aRect);
	ActivateL();
}

void CMainScreenView::UpdateListBoxL(TInt aIsDoubleLine)
{
	TBuf<50> buffer;

	iIsDoubleLine = aIsDoubleLine;

	iListBox->HandleItemRemovalL(); 

	SetIconsArray();

	CDesCArrayFlat* array = new (ELeave) CDesCArrayFlat(6);

	CleanupStack::PushL(array);
	if (iIsDoubleLine == 0)
		{				
	
		buffer.Copy(_L("0\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_STATUS)));
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
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_MAINMENU_SECURE)));
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

		buffer.Copy(_L("3\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
		array->AppendL(buffer);

		buffer.Copy(_L("4\t"));
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

		buffer.Copy(_L("3\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_PUBLISH_UNSHARE)));
		array->AppendL(buffer);

		buffer.Copy(_L("4\t"));
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
	else if (iIsDoubleLine == 5)
		{		
		buffer.Copy(_L("0\t"));
		buffer.Append(*(iEikonEnv->AllocReadResourceL(R_LIST_SUBMENU_SECURESTATUS)));
		array->AppendL(buffer);

		buffer.Copy(_L("1\t"));
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

void CMainScreenView::SizeChanged()
	{
	if (iListBox)
		{
		iListBox->SetRect(Rect());
		}
	}

TInt CMainScreenView::CountComponentControls() const
	{
	if (iListBox)
		{
		return 1;
		}
	return 0;
	}

CCoeControl* CMainScreenView::ComponentControl(TInt aIndex) const
	{
	switch (aIndex)
		{
		case 0:
			return iListBox;
		default:
			return NULL;
		}
	}

void CMainScreenView::Draw(const TRect& aRect) const
	{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushColor(KRgbWhite);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);

	}

TKeyResponse CMainScreenView::OfferKeyEventL(const TKeyEvent& aKeyEvent,
											   TEventCode aType)
	{
	if (iListBox)
		{
		if ((EEventKeyDown == aType)
			&& (EStdQuartzKeyConfirm == aKeyEvent.iScanCode))
			{
			HandleListBoxEventL(
				iListBox, MEikListBoxObserver::EEventEnterKeyPressed);
			}
		}
	return EKeyWasNotConsumed;
	}

void CMainScreenView::HandleListBoxEventL(CEikListBox* aListBox,
											TListBoxEvent aEventType)
	{
	if ((MEikListBoxObserver::EEventEnterKeyPressed == aEventType)
		|| (MEikListBoxObserver::EEventItemClicked == aEventType))
        {
			TInt index;

			if(iAppUi.iMainScreenValue == 0)//Main Screen
			{
				if(aListBox->CurrentItemIndex() == 5)//Secure
				{				
					//iAppUi.SecureDialog();
					iAppUi.iMainScreenValue = 5;
					iAppUi.ShowToolBar();			
				}
				else if(aListBox->CurrentItemIndex() == 1)//Contacts
				{
					iAppUi.iMainScreenValue = 1;
					iAppUi.ShowToolBar();			
				}
				else if(aListBox->CurrentItemIndex() == 2)//Photos
				{
					iAppUi.iMainScreenValue = 2;
					iAppUi.ShowToolBar();			
				}
				else if(aListBox->CurrentItemIndex() == 3)//Videos
				{
					iAppUi.iMainScreenValue = 3;
					iAppUi.ShowToolBar();			
				}
				else if(aListBox->CurrentItemIndex() == 4)//Sms
				{
					iAppUi.iMainScreenValue = 4;
					iAppUi.ShowToolBar();			
				}
				/*else if(aListBox->CurrentItemIndex() == 4)//Change Sub Paln
				{	
					//iAppUi.iMainScreenValue = 5;
					TBuf<100> buffer;
					buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
					CEikonEnv::Static ()->AlertWin(buffer);
				}*/
				else if(aListBox->CurrentItemIndex() == 0)//Status
				{
					iAppUi.iMainScreenValue = 6;
					iAppUi.ShowWindow(KViewIdStatusScreen); //status screen
					iAppUi.DeleteWindow(KViewIdRegisterScreen);
					iAppUi.ShowStatusScreen();

				}
			}
			else if(iAppUi.iMainScreenValue == 1)//Contacts
			{
				if(aListBox->CurrentItemIndex() == 0)//View
				{
					index = aListBox->CurrentItemIndex();
					//TBuf<100> buffer;
					//buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_CONTACTS)));
					//CEikonEnv::Static ()->AlertWin(buffer);
					iAppUi.ShowMessageBoxFromResource(R_TEXT_FEATURE_CONSTRUCTION_VIEW_CONTACTS);
				}
				else if(aListBox->CurrentItemIndex() == 1)//send to vault
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Contacts send to vault"));
					iAppUi.iConnectionStatus = 2;
					iAppUi.CheckSubscriptionStatus(iAppUi.iConnectionStatus);
					iAppUi.ShowMessageBoxFromResource(R_TEXT_CONTACTS_SENDTOWEB);
					iAppUi.iMainScreenValue = 6;
					iAppUi.ShowWindow(KViewIdStatusScreen); //status screen
					iAppUi.DeleteWindow(KViewIdRegisterScreen);
					iAppUi.ShowStatusScreen();
				}
				else if(aListBox->CurrentItemIndex() == 2)//settings
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Contacts Settings"));
					iAppUi.ShowSettingsScreen(index);
				}
			}
			else if(iAppUi.iMainScreenValue == 2)//Photos
			{
				if(aListBox->CurrentItemIndex() == 0)//View Gallery
				{
					TInt ret=0;
					
					if(iAppUi.iSecureGallery)
						//ret = iAppUi.CheckUserId();
						ret = iAppUi.PasswordDialog();
					else
						ret = 1;

					if(ret == 1)
						iAppUi.OpenBrowser();
					/*else if(ret == 0)
					{
						iAppUi.ShowMessageBoxFromResource(R_TEXT_INVALID_USERID_PASSWORD);
					}*/
				}
				else if(aListBox->CurrentItemIndex() == 1)//Send to Vault
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Send to Vault"));					
					iAppUi.ShowSettingsScreen(index);
				}
				/*else if(aListBox->CurrentItemIndex() == 2)//Get From Vault
				{					
					index = aListBox->CurrentItemIndex();
					TBuf<100> buffer;
					buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
					CEikonEnv::Static ()->AlertWin(buffer);
				}*/
				else if(aListBox->CurrentItemIndex() == 2)//Share Album
				{	
					index = aListBox->CurrentItemIndex();
					index = 3;
					//CEikonEnv::Static ()->AlertWin(_L("Share Album"));
					iAppUi.ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 3)//UnShare Album
				{					
					index = aListBox->CurrentItemIndex();
					index = 4;
					//CEikonEnv::Static ()->AlertWin(_L("UnShare Album"));
					iAppUi.ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 4)//settings
				{					
					index = aListBox->CurrentItemIndex();
					index = 5;
					//CEikonEnv::Static ()->AlertWin(_L("Photos Settings"));
					iAppUi.ShowSettingsScreen(index);
				}
			}
			else if(iAppUi.iMainScreenValue == 3)//Videos
			{
				if(aListBox->CurrentItemIndex() == 0)//View Gallery
				{
					TInt ret = 0;
					
					if(iAppUi.iSecureGallery)
						//ret = iAppUi.CheckUserId();
						ret = iAppUi.PasswordDialog();
					else
						ret = 1;

					if(ret == 1)
						iAppUi.OpenBrowser();
					/*else if(ret == 0)
					{
						iAppUi.ShowMessageBoxFromResource(R_TEXT_INVALID_USERID_PASSWORD);
					}*/

				}
				else if(aListBox->CurrentItemIndex() == 1)//Send to Vault
				{					
					index = aListBox->CurrentItemIndex();
					//CEikonEnv::Static ()->AlertWin(_L("Send to Vault"));
					iAppUi.ShowSettingsScreen(index);
				}
				/*else if(aListBox->CurrentItemIndex() == 2)//Get From Vault
				{					
					index = aListBox->CurrentItemIndex();
					TBuf<100> buffer;
					buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
					CEikonEnv::Static ()->AlertWin(buffer);
				}*/
				else if(aListBox->CurrentItemIndex() == 2)//Share Album
				{					
					index = aListBox->CurrentItemIndex();
					index = 3;
					//CEikonEnv::Static ()->AlertWin(_L("Share Album"));
					iAppUi.ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 3)//UnShare Album
				{					
					index = aListBox->CurrentItemIndex();
					index = 4;
					//CEikonEnv::Static ()->AlertWin(_L("UnShare Album"));
					iAppUi.ShowSettingsScreen(index);
				}
				else if(aListBox->CurrentItemIndex() == 4)//settings
				{					
					index = aListBox->CurrentItemIndex();
					index = 5;
					//CEikonEnv::Static ()->AlertWin(_L("Videos Settings"));
					iAppUi.ShowSettingsScreen(index);
				}
			}
			else if(iAppUi.iMainScreenValue == 4)//Sms
			{
				if(aListBox->CurrentItemIndex() == 0)//View Gallery
				{
					index = aListBox->CurrentItemIndex();
					//TBuf<100> buffer;
					//buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION_VIEW_MESSAGES)));
					//CEikonEnv::Static ()->AlertWin(buffer);
					iAppUi.ShowMessageBoxFromResource(R_TEXT_FEATURE_CONSTRUCTION_VIEW_MESSAGES);
				}
				else if(aListBox->CurrentItemIndex() == 1)//Send to Vault
				{
					if(iAppUi.PendingSMS())
					{
						iAppUi.ShowMessageBoxFromResource(R_SMS_SENDTOWEB_BUSY);
						iAppUi.iMainScreenValue = 6;
						iAppUi.ShowWindow(KViewIdStatusScreen); //status screen
						iAppUi.DeleteWindow(KViewIdRegisterScreen);
						iAppUi.ShowStatusScreen();
					}
					else
					{
						index = aListBox->CurrentItemIndex();
						//CEikonEnv::Static ()->AlertWin(_L("Send to Vault"));
						iAppUi.ShowSettingsScreen(index);
					}
				}
				/*else if(aListBox->CurrentItemIndex() == 2)//Get From Vault
				{					
					index = aListBox->CurrentItemIndex();
					TBuf<100> buffer;
					buffer.Copy(*(iEikonEnv->AllocReadResourceL(R_TEXT_FEATURE_CONSTRUCTION)));
					CEikonEnv::Static ()->AlertWin(buffer);
				}*/				
				else if(aListBox->CurrentItemIndex() == 2)//settings
				{					
					index = aListBox->CurrentItemIndex();
					index = 3;
					//CEikonEnv::Static ()->AlertWin(_L("Sms Settings"));
					iAppUi.ShowSettingsScreen(index);
				}
			}
			else if(iAppUi.iMainScreenValue == 5)//Secure
			{
				if(aListBox->CurrentItemIndex() == 0)//status
				{
					index = aListBox->CurrentItemIndex();
					TInt ret;
					ret = iAppUi.CheckSecureApp();
					if(ret)
					{				
						iAppUi.ShowSecure();
						iAppUi.ShowSecure();
					}
					else
					{
						if(CEikonEnv::Static()->QueryWinL(*(iEikonEnv->AllocReadResourceL(R_EXAMPLE_RIPPLE_PUBLISH)),*(iEikonEnv->AllocReadResourceL(R_SECURE_STATUS))))
						{
							iAppUi.ShowSecure();
							iAppUi.ShowSecure();
						}
					}
				}
				else if(aListBox->CurrentItemIndex() == 1)//settings
				{					
					iAppUi.SecureDialog();
				}
			}

			//iEikonEnv->InfoMsg(buffer);
		}
	}

void CMainScreenView::CreateListBoxL()
	{
	// Creates a new MainScreen list box.
	iListBox = new (ELeave) CMainScreenListBox();
	iListBox->ConstructL(this,CEikListBox::ENoFirstLetterMatching| CEikListBox::ENoExtendedSelection);
	iListBox->SetListBoxObserver(this);
	
	// Fills the list box with some items.
	UpdateListBoxL(iIsDoubleLine);	
	}

void CMainScreenView::SetIconsArray()
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
			EMbmIconsStatus, EMbmIconsStatusm));		
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
			EMbmIconsSecure, EMbmIconsSecurem));
	}
	else if( iIsDoubleLine == 1)//Contacts Screen
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		iconArray->AppendL(iEikonEnv->CreateIconL(path,
			EMbmSubiconsContactsview, EMbmSubiconsContactsviewm));
		iconArray->AppendL(iEikonEnv->CreateIconL( path,
			EMbmSubiconsPhotossendtovault,EMbmSubiconsPhotossendtovaultm));
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
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsPhotosunsharealbums,EMbmSubiconsPhotosunsharealbumsm));
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
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsVideosunsharealbum,EMbmSubiconsVideosunsharealbumm));
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
	else if( iIsDoubleLine == 5)//Secure
	{
		TBuf<128> path;
		path = CRippleVaultAppUi::ApplicationDriveAndPath();
		path.Append(KSubIconsMbmFileName);

		TBuf<128> mainpath;
		mainpath = CRippleVaultAppUi::ApplicationDriveAndPath();
		mainpath.Append(KIconsMbmFileName);

		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( mainpath,EMbmIconsStatus,EMbmIconsStatusm)); 
		// Appends icon.
		iconArray->AppendL(iEikonEnv->CreateIconL( path,EMbmSubiconsListsettings,EMbmSubiconsListsettingsm)); 
	}

	CleanupStack::Pop(); // iconArray

	// Sets the icon array.
	CMainScreenListItemDrawer* itemDrawer = static_cast<CMainScreenListItemDrawer*>
		(iListBox->View()->ItemDrawer());
	itemDrawer->SetIconArray(iconArray); // transfer ownership

	// Sets the height of the list box item
	// (= 2 * maximum height of the icon).
	TSize size = itemDrawer->MaxIconSize();
	iListBox->SetItemHeightL(1.5 * size.iHeight);
}

// End of File  
